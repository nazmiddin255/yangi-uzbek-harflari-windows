#!/usr/bin/env bash
# build.sh — klaviatura DLL va setup.exe ni Linux'da yig'ish.
#
# Kerakli dasturlar (Debian/Ubuntu):
#   sudo apt install mingw-w64 nsis python3
#
# Ishlatish:
#   ./build.sh            # versiya: VERSION faylidan
#   ./build.sh 1.0.3      # versiyani qo'lda berish
set -euo pipefail
cd "$(dirname "$0")"

VER="${1:-$(cat VERSION)}"
IFS=. read -r V1 V2 V3 <<< "$VER"
OUT=dist
mkdir -p "$OUT/x64" "$OUT/x86" build

build_dll() {   # $1 = gcc prefiksi, $2 = chiqish papkasi, $3 = kirish nuqtasi, $4 = qo'shimcha
  local cc="$1-gcc" rc="$1-windres" dir="$2"
  "$cc" -O2 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-ident \
        -Wall -o "build/$dir.o" src/uzaltgr.c
  "$rc" -DV1="$V1" -DV2="$V2" -DV3="$V3" -DVERSTR="\\\"$VER\\\"" \
        src/uzaltgr.rc -O coff -o "build/$dir.res.o"
  "$cc" -shared -nostdlib -s \
        -Wl,-T,src/kbd.ld -Wl,-e,"$3" -Wl,--kill-at -Wl,--enable-stdcall-fixup \
        -Wl,--image-base,0x5ffe0000 -Wl,--subsystem,native:6.0 \
        -Wl,--major-os-version,6 -Wl,--minor-os-version,0 \
        -Wl,--dynamicbase,--nxcompat,--no-insert-timestamp ${4:-} \
        -Wl,-Map,"build/$dir.map" \
        -o "$OUT/$dir/uzaltgr.dll" "build/$dir.o" "build/$dir.res.o" src/uzaltgr.def
  python3 tools/finalize.py "$OUT/$dir/uzaltgr.dll" "build/$dir.map"
}

echo "== 64-bit DLL (System32)"
build_dll x86_64-w64-mingw32 x64 DllEntry "-Wl,--high-entropy-va"
echo "== 32-bit DLL (SysWOW64)"
build_dll i686-w64-mingw32 x86 _DllEntry@12

echo "== setup.exe"
makensis -V2 -DVER="$VER" -DOUTDIR="../$OUT" installer/setup.nsi

( cd "$OUT" && sha256sum "uzbek-yangi-alifbo-setup-$VER.exe" > SHA256SUMS.txt )
echo "Tayyor: $OUT/uzbek-yangi-alifbo-setup-$VER.exe"
cat "$OUT/SHA256SUMS.txt"
