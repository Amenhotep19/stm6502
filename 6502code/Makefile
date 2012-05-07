CA=ca65
LD=ld65
OBJCOPY=arm-none-eabi-objcopy 

all:	supermon.o figforth uchess ehbasic wozmon

clean:
	rm -f *.o *.lst *.bin bin nullmem

.a.o:
	$(CA) -o $@ $<
	$(LD) -t none $@ -o bin
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd <bin of=nullmem bs=1 conv=notrunc seek=63488 #$F800
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm nullmem $@
	rm -f bin nullmem
	echo;echo

voodoo:	voodooloader.o
	$(CA) -o voodoo.o voodooloader.a
	$(LD) -t none voodoo.o -o bin
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd if=bin bs=1 conv=notrunc seek=61440 of=nullmem
	dd if=voodoocastle.prg bs=1 conv=notrunc skip=2 seek=16384 of=nullmem
	$(OBJCOPY) -I binary --O elf32-littlearm -B arm nullmem voodoo.o
	rm -f bin nullmem
	echo "Probably not ready yet";echo;echo

gpascal_isnt_ready:
	ca65 gpascal-mini.a
	$(LD) -o gpascal.bin -t none -S 50688 gpascal-mini.o
	rm -f gpascal.o
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm gpascal.bin gpascal.o
	echo "Not working yet...";echo;echo

figforth:
	$(CA) -l figforth.a
	$(LD) -o figforth.bin -t none -S 512 figforth.o
	rm -f figforth.o
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd <figforth.bin of=nullmem bs=1 conv=notrunc seek=512
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm nullmem figforth.o
	echo; echo "Check for JMP()s/labels to 6502 code on xxFF..."
	grep "FF  1  [0-9A-F]" figforth.lst
	echo; echo "load 6502code/figforth.o 0x20000000"
	echo "g 0200";echo;echo

uchess:
	$(CA) -l uchess.a
	$(LD) -o uchess.bin -t none -S 512 uchess.o
	rm -f uchess.o
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd <uchess.bin of=nullmem bs=1 conv=notrunc seek=512
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm nullmem uchess.o
	echo "load 6502code/uchess.o 0x20000000"
	echo "g 0200";echo;echo

ehbasic:
	$(CA) -l basic.a
	$(LD) -o basic.bin -t none -S 51200 basic.o
	rm -f basic.o
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd <basic.bin of=nullmem bs=1 conv=notrunc seek=51200
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm nullmem basic.o
	echo "load 6502code/basic.o 0x20000000"
	echo "g c800";echo;echo

wozmon:
	$(CA) -l wozmon.a
	$(LD) -o wozmon.bin -t none -S 61440 wozmon.o
	rm -f wozmon.o
	dd if=/dev/zero bs=64k count=1 >nullmem
	dd <wozmon.bin of=nullmem bs=1 conv=notrunc seek=61440
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm nullmem wozmon.o
	echo "load 6502code/wozmon.o 0x20000000"
	echo "g f000";echo;echo
