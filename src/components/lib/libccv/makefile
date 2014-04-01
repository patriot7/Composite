CC :=../dietlibc-0.29/bin-i386/diet gcc 
AR :=ar
CFLAGS :=-Wall -fno-stack-protector -ffast-math -nostdinc -nostdlib -Os
COS_INC :=-I../../include -I../../../kernel/include/shared -I../../interface/cbuf_c -I../../interface/evt -I../../interface/cbufp

.PHONY: all clean

all: cos_libccv.a

clean:
	rm -f *.o cos_libccv.a

ccv_io.o: ccv_io.c ccv.h ccv_internal.h io/*.c
	$(CC) $< -o $@ -c $(CFLAGS)

%.o: %.c ccv.h ccv_internal.h cbuf_ccv.h
	$(CC) $(COS_INC) $< -o $@ -c $(CFLAGS)

cos_libccv.a:ccv_basic.o ccv_classic.o ccv_bbf.o ccv_util.o ccv_memory.o ccv_cache.o ccv_resample.o ccv_io.o 3rdparty/sha1/sha1.o cbuf_ccv.o ccv_math.o
	$(AR) rcs $@ $^
