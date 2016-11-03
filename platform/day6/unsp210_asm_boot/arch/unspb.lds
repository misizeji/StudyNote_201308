SECTIONS { 
  . = 0x33e00000;
  .text :
	{
		arch/s5pv210.o(.text)
		*(.text)
	}
  .data ALIGN(4) : { *(.data) }
  .rodata ALIGN(4) : { *(.rodata) }
  .bss ALIGN(4)  : { *(.bss) }
} 

