MEMORY {
  flash_main (rx) : ORIGIN = 0x0, LENGTH = 0x1C000
}

SECTIONS {
  .text :{ *(VECTOR_TABLE) *(.text) }
}
