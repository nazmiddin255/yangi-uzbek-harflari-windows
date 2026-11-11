/* Windows klaviatura DLL uchun linker skripti.
   Windows yadrosi faqat ".data" bo'limini o'qiydi, shuning uchun
   kod, jadvallar va eksport ro'yxati — hammasi .data ichiga yig'iladi. */
SECTIONS
{
  . = SIZEOF_HEADERS;
  . = ALIGN(__section_alignment__);
  .data __image_base__ + __section_alignment__ :
  {
    *(.text) *(.text.*) *(.text$*)
    *(.rdata) *(.rdata.*) *(.rdata$*)
    *(.data) *(.data.*) *(.data$*)
    *(.bss) *(.bss.*) *(COMMON)
    . = ALIGN(16);
    __edata_start__ = .;
    *(.edata)
    __edata_end__ = .;
  }
  .rsrc BLOCK(__section_alignment__) : { *(.rsrc) *(.rsrc$*) }
  .reloc BLOCK(__section_alignment__) : { *(.reloc) }
  /DISCARD/ : { *(.pdata*) *(.xdata*) *(.eh_frame*) *(.comment) *(.note*) *(.debug*) *(.drectve) *(.idata*) *(.CRT*) *(.tls*) *(.ctors*) *(.dtors*) }
}
