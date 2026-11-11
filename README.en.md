# Uzbek – new alphabet (AltGr) keyboard for Windows

An **unofficial**, open-source keyboard layout for the new Uzbek Latin alphabet letters **ş ç ö ğ** and the apostrophe **ʼ**.
It is a standard **US (QWERTY)** layout with extra characters on the **right Alt (AltGr)** key:

| Keys | Result | With Shift |
|---|:---:|:---:|
| AltGr + S | ş | Ş |
| AltGr + C | ç | Ç |
| AltGr + O | ö | Ö |
| AltGr + G | ğ | Ğ |
| AltGr + ` | ʼ (U+02BC) | — |

**Requirements:** Windows 10/11, 64-bit (x64). ARM64 and 32-bit Windows are not supported yet.

**Install:** download `uzbek-yangi-alifbo-setup-X.Y.Z.exe` from [Releases](../../releases/latest), run it, reboot, then press **Win + Space** and pick *“Uzbek - yangi alifbo (AltGr)”*.
The installer is not code-signed, so SmartScreen may warn you (*More info → Run anyway*). Verify the file with `SHA256SUMS.txt`.

**After a major Windows upgrade** the layout may disappear from the list. Just run the installer again.

**Uninstall:** Settings → Apps → Installed apps → *Uzbek yangi alifbo AltGr*.

**Build from source** (Linux/WSL): `sudo apt install mingw-w64 nsis python3 && ./build.sh`. See [docs/TAHRIRLASH.md](docs/TAHRIRLASH.md) (Uzbek) for how to change the layout.

Provided **as is**, without warranty. Not affiliated with any government body. License: [MIT](LICENSE).
