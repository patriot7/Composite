C_OBJS=tor_resample.o
ASM_OBJS=
COMPONENT=tor_res.o
INTERFACES=
DEPENDENCIES=sched printc timed_blk torrent cbufp cbuf_c evt mem_mgr_large valloc lock
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS+=$(POSIX_LIB)
FN_PREPEND=file_ ccv_res_

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
