mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
PROG = $(shell basename $(shell dirname $(shell realpath $(current_dir))))

CC = gcc
CFLAGS += -frecord-gcc-switches -fdiagnostics-color=always
CFLAGS += -save-temps=obj -fverbose-asm -Wall -Werror
CFLAGS += -Wall -Werror -std=gnu11 -D_GNU_SOURCE
CFLAGS += -O2 -DNDEBUG -march=native -ftree-vectorize -fvect-cost-model=very-cheap -Wl,--strip-all
CFLAGS += -z noexecstack

all:
	yasm -f x64 -f elf64 -X gnu -g dwarf2 -D LINUX -o crc_iscsi_v_pcl.o crc_iscsi_v_pcl.asm
	$(CC) -c $(CFLAGS) main.c
	$(CC) -o $(PROG) $(CFLAGS) *.o

clean:
	rm -f $(PROG) *.o *.i *.o *.s
