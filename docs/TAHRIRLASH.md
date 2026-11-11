# Klaviaturani oʻzgartirish va oʻzingiz yigʻish

Bu qoʻllanma yangi boshlovchilar uchun yozilgan. Hech qanday Windows dasturi kerak emas: hammasi **Linux**da
(yoki Windows ichidagi **WSL**da) yoki toʻgʻridan-toʻgʻri **GitHub**ning oʻzida qilinadi.

## 1-yoʻl: GitHub'ning oʻzida (kompyutersiz)

1. Repozitoriyni **Fork** qiling (oʻng yuqoridagi tugma).
2. Oʻz nusxangizda `src/uzaltgr.c` faylini oching va ✏️ (Edit) tugmasini bosing.
3. Kerakli oʻzgarishni kiriting (pastdagi misollarga qarang) va **Commit** qiling.
4. **Actions** boʻlimiga kiring. Bu boʻlimda `setup.exe` avtomatik yigʻiladi.
   Yigʻish tugagach, natija **Artifacts** qismida paydo boʻladi.

Agar fork'ingizda Actions oʻchiq boʻlsa, **Actions → I understand… enable** tugmasini bosing.

## 2-yoʻl: Oʻz kompyuteringizda (Linux / WSL)

```bash
sudo apt install mingw-w64 nsis python3 git
git clone https://github.com/nazmiddin255/yangi-uzbek-harflari.git
cd yangi-uzbek-harflari
./build.sh
```

Tayyor fayl `dist/` papkasida paydo boʻladi.

## Jadvalni qanday oʻqish kerak?

`src/uzaltgr.c` faylidagi `vk2wch5` jadvalida har bir tugmaga bitta qator toʻgʻri keladi:

```c
/*   VK    Attributes             oddiy  Shift  Ctrl      AltGr     AltGr+Shift */
{ 'S',    CAPLOK | CAPLOKALTGR,  {'s',  'S',   WCH_NONE, 0x015F,   0x015E  } },
```

| Ustun | Maʼnosi |
|---|---|
| `'S'` | qaysi tugma |
| `CAPLOK` | Caps Lock harfni katta qiladi |
| `CAPLOKALTGR` | Caps Lock AltGr harfini ham katta qiladi |
| `'s'` | oddiy bosilganda |
| `'S'` | Shift bilan |
| `WCH_NONE` | Ctrl bilan (`WCH_NONE` = hech narsa) |
| `0x015F` | AltGr bilan (ş) |
| `0x015E` | AltGr + Shift bilan (Ş) |

Harf kodlarini (masalan, `0x015F`) [unicode-table.com](https://unicode-table.com) saytidan topishingiz mumkin. U yerda harfni qidirasiz va `U+015F` koʻrinishidagi kodni `0x015F` deb yozasiz.

## Misollar

### AltGr + Z → ʻ (oʻ, gʻ uchun tutuq)

`'Z'` qatorini toping:

```c
{ 'Z', CAPLOK, {'z', 'Z', WCH_NONE, WCH_NONE, WCH_NONE} },
```

Uni quyidagicha oʻzgartiring:

```c
{ 'Z', CAPLOK, {'z', 'Z', WCH_NONE, 0x02BB, WCH_NONE} },
```

### AltGr + N → ñ / Ñ

```c
{ 'N', CAPLOK | CAPLOKALTGR, {'n', 'N', WCH_NONE, 0x00F1, 0x00D1} },
```

## Oʻz versiyangizni tarqatmoqchi boʻlsangiz

Rasmiy versiya bilan toʻqnashmasligi uchun quyidagilarni **albatta** oʻzgartiring:

| Fayl | Nima | Misol |
|---|---|---|
| `installer/setup.nsi` | `KLID` | `a0df0443` |
| `installer/setup.nsi` | `LAYOUTID` | `0a7d` (tizimda yagona boʻlishi kerak) |
| `installer/setup.nsi` | `TIP` | `0443:A0DF0443` (KLID bilan mos) |
| `installer/setup.nsi` | `NAME`, `LAYOUT`, `UNKEY` | oʻz nomingiz |
| `src/uzaltgr.def`, `build.sh`, `setup.nsi` | DLL nomi `uzaltgr.dll` | masalan `uzmy.dll` (8 harfdan oshmasin) |

Yangi versiya chiqarganda `VERSION` faylidagi raqamni oshiring.

## Xatolar boʻlsa

- **`Layout Id` takrorlanmasin.** Agar Windows'da shu raqamli boshqa klaviatura boʻlsa, ikkalasi ham ishlamay qolishi mumkin.
- **`src/kbd.ld` va `tools/finalize.py` ga tegmang.** Windows klaviatura faylidan faqat `.data` boʻlimini oʻqiydi. Shuning uchun bu ikki fayl kod, jadvallar va eksport roʻyxatini aynan shu boʻlimga yigʻadi. Busiz Windows klaviaturani yuklamaydi.
- Oʻzgartirishdan keyin har doim **virtual mashinada** sinab koʻring.
