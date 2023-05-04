/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 0x7e00;
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text :
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	start_rodata = .;
	.rodata : 
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data : 
	{
		*(.data)
		start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss : 
	{
		*(COMMON)
		*(.bss)
	}
	  /DISCARD/ : { *(.fini_array*) *(.comment) }
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}