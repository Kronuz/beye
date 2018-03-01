# Beye

BEYE (Binary EYE) is a multiplatform, portable viewer of binary files with a
built-in editor that functions in binary, hexadecimal and disassembler modes.
It uses native Intel syntax for disassembly. Features include an
AVR/Java/x86-i386-AMD64/ARM-XScale/PPC64 disassemblers, a Russian code pages
converter, full preview of MZ, NE, PE, NLM, COFF32, ELF formats, partial preview
of a.out, LE and LX, Phar Lap formats, and a code navigator.

## Building in three steps

1. Prepare build using:

	```sh
	mkdir build
	cd build
	cmake -GNinja ..
	```

2. build and install using:

	```sh
	ninja
	ninja install
	```

3. Start using beye
