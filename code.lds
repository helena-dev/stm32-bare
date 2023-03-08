MEMORY {
  flash_main (rx) : ORIGIN = 0x0, LENGTH = (0x1C000)/2
  ram (rw) : ORIGIN = 0x20000000 +(0x1C000)/2, LENGTH = (0x1C000)/2
}

SECTIONS {
  .text :{ *(VECTOR_TABLE) *(.text) }
  .bss :{ *(.bss) }
}
