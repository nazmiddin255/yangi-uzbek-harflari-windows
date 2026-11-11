/*
 * kbd_min.h — Windows klaviatura jadvallari uchun minimal tuzilmalar.
 *
 * Bu Microsoft WDK dagi kbd.h faylining eng kerakli qismi.
 * Odatda bu faylni O'ZGARTIRISH KERAK EMAS.
 *
 * Muhim: 64-bitli Windows'da 32-bitli (SysWOW64) DLL ham ko'rsatkichlarni
 * 8 baytlik qilib saqlashi kerak. Shuning uchun har bir ko'rsatkich
 * KP(...) ichiga o'ralgan — u ikkala arxitekturada ham 8 bayt bo'ladi.
 */
#ifndef KBD_MIN_H
#define KBD_MIN_H

typedef unsigned char  BYTE;
typedef unsigned short USHORT;
typedef unsigned short WCHAR;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

/* 8 baytlik ko'rsatkich (32-bit va 64-bit uchun bir xil) */
#define KP(T) union { T p; unsigned long long _pad; } __attribute__((aligned(8)))

/* ---- Belgi xususiyatlari (Attributes) ---- */
#define CAPLOK       0x01   /* Caps Lock harfni katta qiladi             */
#define SGCAPS       0x02
#define CAPLOKALTGR  0x04   /* Caps Lock AltGr harflariga ham ta'sir qiladi */
#define WCH_NONE     0xF000 /* bu tugmada bu holatda hech narsa yo'q     */

/* ---- Modifikator bitlari ---- */
#define KBDSHIFT  0x01
#define KBDCTRL   0x02
#define KBDALT    0x04
#define SHFT_INVALID 0x0F

/* ---- Skan-kod bayroqlari ---- */
#define KBDEXT      0x0100
#define KBDMULTIVK  0x0200
#define KBDSPECIAL  0x0400
#define KBDNUMPAD   0x0800

/* ---- KBDTABLES bayroqlari ---- */
#define KLLF_ALTGR   0x0001  /* o'ng Alt = AltGr (Ctrl+Alt) */
#define KBD_VERSION  1
#define KEYBOARD_TYPE_GENERIC_101 4

/* ---- Virtual tugmalar (VK) ---- */
#define VK_CANCEL     0x03
#define VK_BACK       0x08
#define VK_TAB        0x09
#define VK_RETURN     0x0D
#define VK_SHIFT      0x10
#define VK_CONTROL    0x11
#define VK_MENU       0x12
#define VK_ESCAPE     0x1B
#define VK_SPACE      0x20
#define VK_NUMPAD0    0x60
#define VK_MULTIPLY   0x6A
#define VK_ADD        0x6B
#define VK_SUBTRACT   0x6D
#define VK_DECIMAL    0x6E
#define VK_DIVIDE     0x6F
#define VK_OEM_1      0xBA  /* ;:  */
#define VK_OEM_PLUS   0xBB  /* =+  */
#define VK_OEM_COMMA  0xBC  /* ,<  */
#define VK_OEM_MINUS  0xBD  /* -_  */
#define VK_OEM_PERIOD 0xBE  /* .>  */
#define VK_OEM_2      0xBF  /* /?  */
#define VK_OEM_3      0xC0  /* `~  */
#define VK_OEM_4      0xDB  /* [{  */
#define VK_OEM_5      0xDC  /* \|  */
#define VK_OEM_6      0xDD  /* ]}  */
#define VK_OEM_7      0xDE  /* '"  */
#define VK_OEM_102    0xE2  /* \|  (ba'zi klaviaturalarda qo'shimcha tugma) */

/* ---- Tuzilmalar ---- */
typedef struct { BYTE Vk; BYTE ModBits; } VK_TO_BIT;

typedef struct {
    KP(const VK_TO_BIT *) pVkToBit;
    WORD  wMaxModBits;
    BYTE  ModNumber[];
} MODIFIERS;

typedef struct { BYTE VirtualKey; BYTE Attributes; WCHAR wch[1]; } VK_TO_WCHARS1;
typedef struct { BYTE VirtualKey; BYTE Attributes; WCHAR wch[5]; } VK_TO_WCHARS5;

typedef struct {
    KP(const void *) pVkToWchars;
    BYTE nModifications;
    BYTE cbSize;
} VK_TO_WCHAR_TABLE;

typedef struct { BYTE vsc; KP(const WCHAR *) pwsz; } VSC_LPWSTR;
typedef struct { BYTE Vsc; USHORT Vk; } VSC_VK;

typedef struct {
    KP(const MODIFIERS *)         pCharModifiers;
    KP(const VK_TO_WCHAR_TABLE *) pVkToWcharTable;
    KP(const void *)              pDeadKey;
    KP(const VSC_LPWSTR *)        pKeyNames;
    KP(const VSC_LPWSTR *)        pKeyNamesExt;
    KP(const void *)              pKeyNamesDead;
    KP(const USHORT *)            pusVSCtoVK;
    BYTE                          bMaxVSCtoVK;
    KP(const VSC_VK *)            pVSCtoVK_E0;
    KP(const VSC_VK *)            pVSCtoVK_E1;
    DWORD                         fLocaleFlags;
    BYTE                          nLgMax;
    BYTE                          cbLgEntry;
    KP(const void *)              pLigature;
    DWORD                         dwType;
    DWORD                         dwSubType;
} KBDTABLES;

#endif
