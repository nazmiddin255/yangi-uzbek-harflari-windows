#!/usr/bin/env python3
"""
finalize.py — DLL ni yakunlash (qo'shimcha kutubxona kerak emas).

GNU ld eksport ro'yxatini .data ichiga joylaganda, PE sarlavhasidagi
"Export Directory" manzilini yozmaydi. Bu skript:
  1) linker xaritasidan (map) __edata_start__/__edata_end__ manzillarini oladi,
  2) Export Directory ni shu manzilga yozadi,
  3) PE checksum ni qayta hisoblaydi.

Ishlatish:  python3 finalize.py uzaltgr.dll uzaltgr.map
"""
import re
import struct
import sys


def pe_checksum(data: bytes, csum_off: int) -> int:
    total = 0
    n = len(data)
    padded = data + (b'\0' if n % 2 else b'')
    for i in range(0, len(padded), 2):
        if i == csum_off or i == csum_off + 2:
            continue
        total += padded[i] | (padded[i + 1] << 8)
        total = (total & 0xFFFF) + (total >> 16)
    total = (total & 0xFFFF) + (total >> 16)
    return (total & 0xFFFF) + n


def main(dll_path: str, map_path: str) -> None:
    text = open(map_path, encoding='utf-8', errors='replace').read()

    def sym(name: str) -> int:
        m = re.search(r'0x([0-9a-fA-F]+)\s+' + re.escape(name) + r'\b', text)
        if not m:
            sys.exit(f"xato: map faylida {name} topilmadi")
        return int(m.group(1), 16)

    start, end = sym('__edata_start__'), sym('__edata_end__')
    data = bytearray(open(dll_path, 'rb').read())
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    opt = pe + 24
    magic = struct.unpack_from('<H', data, opt)[0]
    is64 = magic == 0x20B
    image_base = struct.unpack_from('<Q' if is64 else '<I', data, opt + (24 if is64 else 28))[0]
    dd = opt + (112 if is64 else 96)          # DataDirectory[0] = Export
    rva, size = start - image_base, end - start
    if size < 40:
        sys.exit("xato: eksport ro'yxati bo'sh")
    struct.pack_into('<II', data, dd, rva, size)

    # ld eksport ichidagi manzillarni bo'lim boshiga nisbatan hisoblaydi.
    # Ularni haqiqiy joyga (rva) suramiz. Funksiya manzillari to'g'ri, ularga tegmaymiz.
    nsec = struct.unpack_from('<H', data, pe + 6)[0]
    sec_tab = opt + struct.unpack_from('<H', data, pe + 20)[0]
    def rva2off(r: int) -> int:
        for i in range(nsec):
            # IMAGE_SECTION_HEADER: VirtualSize@8, VirtualAddress@12, SizeOfRawData@16, PointerToRawData@20
            vs, va, raw_sz, raw_ptr = struct.unpack_from('<IIII', data, sec_tab + 40 * i + 8)
            if va <= r < va + max(vs, raw_sz):
                return raw_ptr + (r - va)
        sys.exit(f"xato: RVA {r:#x} hech bir bo'limda emas")
    e = rva2off(rva)
    struct.pack_into('<I', data, e + 4, 0)                 # TimeDateStamp = 0
    addr_funcs = struct.unpack_from('<I', data, e + 28)[0]
    delta = (rva + 40) - addr_funcs
    if delta:
        for o in (12, 28, 32, 36):                         # Name, Funcs, Names, Ordinals
            v = struct.unpack_from('<I', data, e + o)[0]
            struct.pack_into('<I', data, e + o, v + delta)
        nnames = struct.unpack_from('<I', data, e + 24)[0]
        names = rva2off(struct.unpack_from('<I', data, e + 32)[0])
        for i in range(nnames):
            v = struct.unpack_from('<I', data, names + 4 * i)[0]
            struct.pack_into('<I', data, names + 4 * i, v + delta)
    csum_off = opt + 64
    struct.pack_into('<I', data, csum_off, 0)
    struct.pack_into('<I', data, csum_off, pe_checksum(bytes(data), csum_off))
    open(dll_path, 'wb').write(data)
    print(f"{dll_path}: Export Directory = {rva:#x} ({size} bayt), checksum yangilandi")


if __name__ == '__main__':
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    main(sys.argv[1], sys.argv[2])
