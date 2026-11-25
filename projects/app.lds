OUTPUT_FORMAT("binary");

SECTIONS {
  .head 0x0 : {
    LONG(0x00100000)            /* segsiz: data segment size (1MB) */
    LONG(0x69726148)            /* signature "Hari" */
    LONG(0)                     /* mmarea (unused) */
    LONG(0x00040000)            /* esp: initial stack ptr = .data address */
    LONG(SIZEOF(.data))         /* datsiz */
    LONG(0x20 + SIZEOF(.text))  /* dathrb: file offset of .data */
    LONG(0xE9000000)
    LONG(_HariMain - 0x20)
  }
  .text 0x20 : {
    *(.text)
    *(.rodata*)
    *(.eh_frame)
  }
  .data 0x40000 : AT(ADDR(.text) + SIZEOF(.text)) {
    *(.data)
  }
  .bss : {
    *(.bss)
    *(COMMON)
  }
}
