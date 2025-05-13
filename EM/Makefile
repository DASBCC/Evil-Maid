ARCH     = x86_64
EFI_INC  = /usr/include/efi
EFI_LIB  = /usr/lib/x86_64-linux-gnu
EFI_LDS  = /usr/lib/elf_x86_64_efi.lds

CFLAGS   = -I$(EFI_INC) -I$(EFI_INC)/$(ARCH) -fpic -fshort-wchar -Wall
LDFLAGS  = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFI_LIB) -lefi -lgnuefi

all: BOOTX64.EFI

BOOTX64.EFI: BootkitLoader_EMShell.o
	ld $(LDFLAGS) -o $@ $^

BootkitLoader_EMShell.o: BootkitLoader_EMShell.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.efi
