/*
 * uzaltgr.c — "Uzbek - yangi alifbo (AltGr)" klaviatura terilmasi.
 *
 * Asos: oddiy US (QWERTY) klaviatura.
 * Qo'shimcha: o'ng Alt (AltGr) bilan yangi o'zbek alifbosi harflari:
 *
 *     AltGr + S  ->  ş      AltGr + Shift + S  ->  Ş
 *     AltGr + C  ->  ç      AltGr + Shift + C  ->  Ç
 *     AltGr + O  ->  ö      AltGr + Shift + O  ->  Ö
 *     AltGr + G  ->  ğ      AltGr + Shift + G  ->  Ğ
 *     AltGr + `  ->  ʼ (tutuq belgisi, U+02BC)
 *
 * ------------------------------------------------------------------
 *  O'ZGARTIRMOQCHIMISIZ?  Faqat pastdagi "vk2wch5" jadvaliga qarang.
 *  Har bir qatorda 5 ta ustun bor:
 *
 *    [0] oddiy   [1] Shift   [2] Ctrl   [3] AltGr   [4] AltGr+Shift
 *
 *  Masalan, AltGr + Z ga "ʻ" (U+02BB) qo'ymoqchi bo'lsangiz,
 *  'Z' qatorini toping va 4-ustundagi WCH_NONE o'rniga 0x02BB yozing.
 *  Attributes ustunini CAPLOK | CAPLOKALTGR qiling (Caps Lock ishlashi uchun).
 *  Keyin build.sh ni ishga tushiring.
 * ------------------------------------------------------------------
 */
#include "kbd_min.h"

#define W(s) { (const WCHAR *)L##s }

/* ===================== 1. Modifikatorlar ===================== */
/* Shift, Ctrl, Alt tugmalari qaysi bitlarga mos kelishi */
static const VK_TO_BIT vk2bits[] = {
    { VK_SHIFT,   KBDSHIFT },
    { VK_CONTROL, KBDCTRL  },
    { VK_MENU,    KBDALT   },
    { 0, 0 }
};

/* Bit kombinatsiyasi -> jadvaldagi ustun raqami */
static const MODIFIERS CharModifiers = {
    { vk2bits },
    7,
    {
        0,            /* 000 = oddiy                 -> ustun 0 */
        1,            /* 001 = Shift                 -> ustun 1 */
        2,            /* 010 = Ctrl                  -> ustun 2 */
        SHFT_INVALID, /* 011 = Shift+Ctrl            -> yo'q    */
        SHFT_INVALID, /* 100 = Alt                   -> yo'q    */
        SHFT_INVALID, /* 101 = Shift+Alt             -> yo'q    */
        3,            /* 110 = Ctrl+Alt (AltGr)       -> ustun 3 */
        4             /* 111 = Shift+Ctrl+Alt         -> ustun 4 */
    }
};

/* ===================== 2. Asosiy belgilar jadvali ===================== */
/*   VK         Attributes             oddiy   Shift   Ctrl    AltGr   AltGr+Shift */
static const VK_TO_WCHARS5 vk2wch5[] = {
    { '1',          0,                     {'1',    '!',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '2',          0,                     {'2',    '@',    0x0000,   WCH_NONE, WCH_NONE} },
    { '3',          0,                     {'3',    '#',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '4',          0,                     {'4',    '$',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '5',          0,                     {'5',    '%',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '6',          0,                     {'6',    '^',    0x001E,   WCH_NONE, WCH_NONE} },
    { '7',          0,                     {'7',    '&',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '8',          0,                     {'8',    '*',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '9',          0,                     {'9',    '(',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { '0',          0,                     {'0',    ')',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_MINUS, 0,                     {'-',    '_',    0x001F,   WCH_NONE, WCH_NONE} },
    { VK_OEM_PLUS,  0,                     {'=',    '+',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'Q',          CAPLOK,                {'q',    'Q',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'W',          CAPLOK,                {'w',    'W',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'E',          CAPLOK,                {'e',    'E',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'R',          CAPLOK,                {'r',    'R',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'T',          CAPLOK,                {'t',    'T',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'Y',          CAPLOK,                {'y',    'Y',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'U',          CAPLOK,                {'u',    'U',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'I',          CAPLOK,                {'i',    'I',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'O',          CAPLOK | CAPLOKALTGR,  {'o',    'O',    WCH_NONE, 0x00F6,   0x00D6  } }, /* ö Ö */
    { 'P',          CAPLOK,                {'p',    'P',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_4,     0,                     {'[',    '{',    0x001B,   WCH_NONE, WCH_NONE} },
    { VK_OEM_6,     0,                     {']',    '}',    0x001D,   WCH_NONE, WCH_NONE} },
    { 'A',          CAPLOK,                {'a',    'A',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'S',          CAPLOK | CAPLOKALTGR,  {'s',    'S',    WCH_NONE, 0x015F,   0x015E  } }, /* ş Ş */
    { 'D',          CAPLOK,                {'d',    'D',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'F',          CAPLOK,                {'f',    'F',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'G',          CAPLOK | CAPLOKALTGR,  {'g',    'G',    WCH_NONE, 0x011F,   0x011E  } }, /* ğ Ğ */
    { 'H',          CAPLOK,                {'h',    'H',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'J',          CAPLOK,                {'j',    'J',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'K',          CAPLOK,                {'k',    'K',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'L',          CAPLOK,                {'l',    'L',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_1,     0,                     {';',    ':',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_7,     0,                     {'\'',   '"',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_3,     0,                     {'`',    '~',    WCH_NONE, 0x02BC,   WCH_NONE} }, /* ʼ */
    { VK_OEM_5,     0,                     {'\\',   '|',    0x001C,   WCH_NONE, WCH_NONE} },
    { 'Z',          CAPLOK,                {'z',    'Z',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'X',          CAPLOK,                {'x',    'X',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'C',          CAPLOK | CAPLOKALTGR,  {'c',    'C',    WCH_NONE, 0x00E7,   0x00C7  } }, /* ç Ç */
    { 'V',          CAPLOK,                {'v',    'V',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'B',          CAPLOK,                {'b',    'B',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'N',          CAPLOK,                {'n',    'N',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { 'M',          CAPLOK,                {'m',    'M',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_COMMA, 0,                     {',',    '<',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_PERIOD,0,                     {'.',    '>',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_OEM_2,     0,                     {'/',    '?',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_SPACE,     0,                     {' ',    ' ',    ' ',      WCH_NONE, WCH_NONE} },
    { VK_OEM_102,   0,                     {'\\',   '|',    0x001C,   WCH_NONE, WCH_NONE} },
    { VK_DECIMAL,   0,                     {'.',    '.',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_TAB,       0,                     {'\t',   '\t',   WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_ADD,       0,                     {'+',    '+',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_DIVIDE,    0,                     {'/',    '/',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_MULTIPLY,  0,                     {'*',    '*',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_SUBTRACT,  0,                     {'-',    '-',    WCH_NONE, WCH_NONE, WCH_NONE} },
    { VK_BACK,      0,                     {0x0008, 0x0008, 0x007F,   WCH_NONE, WCH_NONE} },
    { VK_ESCAPE,    0,                     {0x001B, 0x001B, 0x001B,   WCH_NONE, WCH_NONE} },
    { VK_RETURN,    0,                     {'\r',   '\r',   '\n',     WCH_NONE, WCH_NONE} },
    { VK_CANCEL,    0,                     {0x0003, 0x0003, 0x0003,   WCH_NONE, WCH_NONE} },
    { 0, 0, {0, 0, 0, 0, 0} }
};

/* Raqamli klaviatura (NumPad) — faqat bitta ustun */
static const VK_TO_WCHARS1 vk2wch1[] = {
    { VK_NUMPAD0 + 0, 0, {'0'} },
    { VK_NUMPAD0 + 1, 0, {'1'} },
    { VK_NUMPAD0 + 2, 0, {'2'} },
    { VK_NUMPAD0 + 3, 0, {'3'} },
    { VK_NUMPAD0 + 4, 0, {'4'} },
    { VK_NUMPAD0 + 5, 0, {'5'} },
    { VK_NUMPAD0 + 6, 0, {'6'} },
    { VK_NUMPAD0 + 7, 0, {'7'} },
    { VK_NUMPAD0 + 8, 0, {'8'} },
    { VK_NUMPAD0 + 9, 0, {'9'} },
    { 0, 0, {0} }
};

static const VK_TO_WCHAR_TABLE vk2wchTables[] = {
    { { vk2wch5 }, 5, sizeof(VK_TO_WCHARS5) },
    { { vk2wch1 }, 1, sizeof(VK_TO_WCHARS1) },
    { { 0 },       0, 0 }
};

/* ===================== 3. Skan-kodlar (O'ZGARTIRMANG) ===================== */
/* Fizik tugma raqami -> virtual tugma. Standart US jadvali. */
static const USHORT ausVK[] = {
    0x00FF, 0x001B, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, /* 00-07 */
    0x0037, 0x0038, 0x0039, 0x0030, 0x00BD, 0x00BB, 0x0008, 0x0009, /* 08-0F */
    0x0051, 0x0057, 0x0045, 0x0052, 0x0054, 0x0059, 0x0055, 0x0049, /* 10-17 */
    0x004F, 0x0050, 0x00DB, 0x00DD, 0x000D, 0x00A2, 0x0041, 0x0053, /* 18-1F */
    0x0044, 0x0046, 0x0047, 0x0048, 0x004A, 0x004B, 0x004C, 0x00BA, /* 20-27 */
    0x00DE, 0x00C0, 0x00A0, 0x00DC, 0x005A, 0x0058, 0x0043, 0x0056, /* 28-2F */
    0x0042, 0x004E, 0x004D, 0x00BC, 0x00BE, 0x00BF, 0x01A1, 0x026A, /* 30-37 */
    0x00A4, 0x0020, 0x0014, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, /* 38-3F */
    0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x0390, 0x0291, 0x0C24, /* 40-47 */
    0x0C26, 0x0C21, 0x006D, 0x0C25, 0x0C0C, 0x0C27, 0x006B, 0x0C23, /* 48-4F */
    0x0C28, 0x0C22, 0x0C2D, 0x0C2E, 0x002C, 0x00FF, 0x00E2, 0x007A, /* 50-57 */
    0x007B, 0x000C, 0x00EE, 0x00F1, 0x00EA, 0x00F9, 0x00F5, 0x00F3, /* 58-5F */
    0x00FF, 0x00FF, 0x00FB, 0x002F, 0x007C, 0x007D, 0x007E, 0x007F, /* 60-67 */
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x00ED, /* 68-6F */
    0x00FF, 0x00E9, 0x00FF, 0x00C1, 0x00FF, 0x00FF, 0x0087, 0x00FF, /* 70-77 */
    0x00FF, 0x00FF, 0x00FF, 0x00EB, 0x0009, 0x00FF, 0x00C2, 0x00EC  /* 78-7F */
};

static const VSC_VK aE0VscToVk[] = {
    {0x10,0x01B1},{0x19,0x01B0},{0x1D,0x01A3},{0x20,0x01AD},{0x21,0x01B7},
    {0x22,0x01B3},{0x24,0x01B2},{0x2E,0x01AE},{0x30,0x01AF},{0x32,0x01AC},
    {0x35,0x016F},{0x37,0x012C},{0x38,0x01A5},{0x47,0x0124},{0x48,0x0126},
    {0x49,0x0121},{0x4B,0x0125},{0x4D,0x0127},{0x4F,0x0123},{0x50,0x0128},
    {0x51,0x0122},{0x52,0x012D},{0x53,0x012E},{0x5B,0x015B},{0x5C,0x015C},
    {0x5D,0x015D},{0x5F,0x015F},{0x65,0x01AA},{0x66,0x01AB},{0x67,0x01A8},
    {0x68,0x01A9},{0x69,0x01A7},{0x6A,0x01A6},{0x6B,0x01B6},{0x6C,0x01B4},
    {0x6D,0x01B5},{0x1C,0x010D},{0x46,0x0103},{0,0}
};

static const VSC_VK aE1VscToVk[] = {
    {0x1D,0x0013},{0,0}
};

/* ===================== 4. Tugma nomlari ===================== */
static const VSC_LPWSTR aKeyNames[] = {
    {0x01,W("Esc")},        {0x0E,W("Backspace")},  {0x0F,W("Tab")},
    {0x1C,W("Enter")},      {0x1D,W("Ctrl")},       {0x2A,W("Shift")},
    {0x36,W("Right Shift")},{0x37,W("Num *")},      {0x38,W("Alt")},
    {0x39,W("Space")},      {0x3A,W("Caps Lock")},  {0x3B,W("F1")},
    {0x3C,W("F2")},         {0x3D,W("F3")},         {0x3E,W("F4")},
    {0x3F,W("F5")},         {0x40,W("F6")},         {0x41,W("F7")},
    {0x42,W("F8")},         {0x43,W("F9")},         {0x44,W("F10")},
    {0x45,W("Pause")},      {0x46,W("Scroll Lock")},{0x47,W("Num 7")},
    {0x48,W("Num 8")},      {0x49,W("Num 9")},      {0x4A,W("Num -")},
    {0x4B,W("Num 4")},      {0x4C,W("Num 5")},      {0x4D,W("Num 6")},
    {0x4E,W("Num +")},      {0x4F,W("Num 1")},      {0x50,W("Num 2")},
    {0x51,W("Num 3")},      {0x52,W("Num 0")},      {0x53,W("Num Del")},
    {0x54,W("Sys Req")},    {0x57,W("F11")},        {0x58,W("F12")},
    {0x7C,W("F13")},        {0x7D,W("F14")},        {0x7E,W("F15")},
    {0x7F,W("F16")},        {0x80,W("F17")},        {0x81,W("F18")},
    {0x82,W("F19")},        {0x83,W("F20")},        {0x84,W("F21")},
    {0x85,W("F22")},        {0x86,W("F23")},        {0x87,W("F24")},
    {0,{0}}
};

static const VSC_LPWSTR aKeyNamesExt[] = {
    {0x1C,W("Num Enter")},  {0x1D,W("Right Ctrl")}, {0x35,W("Num /")},
    {0x37,W("Prnt Scrn")},  {0x38,W("Right Alt")},  {0x45,W("Num Lock")},
    {0x46,W("Break")},      {0x47,W("Home")},       {0x48,W("Up")},
    {0x49,W("Page Up")},    {0x4B,W("Left")},       {0x4D,W("Right")},
    {0x4F,W("End")},        {0x50,W("Down")},       {0x51,W("Page Down")},
    {0x52,W("Insert")},     {0x53,W("Delete")},     {0x54,W("<00>")},
    {0x56,W("Help")},       {0x5B,W("Left Windows")},{0x5C,W("Right Windows")},
    {0x5D,W("Application")},
    {0,{0}}
};

/* ===================== 5. Hammasini bog'lovchi jadval ===================== */
static const KBDTABLES KbdTables = {
    { &CharModifiers },
    { vk2wchTables },
    { 0 },                 /* o'lik tugmalar (dead keys) yo'q */
    { aKeyNames },
    { aKeyNamesExt },
    { 0 },
    { ausVK },
    sizeof(ausVK) / sizeof(ausVK[0]),
    { aE0VscToVk },
    { aE1VscToVk },
    (KBD_VERSION << 16) | KLLF_ALTGR,
    0, 0, { 0 },           /* ligaturalar yo'q */
    KEYBOARD_TYPE_GENERIC_101,
    0
};

/* Windows shu funksiyani topib, jadval manzilini oladi */
const KBDTABLES * __attribute__((stdcall)) KbdLayerDescriptor(void)
{
    return &KbdTables;
}

/* DLL kirish nuqtasi — hech narsa qilmaydi */
int __attribute__((stdcall)) DllEntry(void *h, unsigned long reason, void *r)
{
    (void)h; (void)reason; (void)r;
    return 1;
}
