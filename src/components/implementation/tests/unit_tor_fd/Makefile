C_OBJS=unit_tor_fd.o
ASM_OBJS=
COMPONENT=tor_fd.o
INTERFACES=
DEPENDENCIES=sched printc timed_blk torrent cbufp cbuf_c evt mem_mgr_large valloc lock
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS+=$(POSIX_LIB)

FN_PREPEND=fs_ ccv_fd_ next_stage_

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
