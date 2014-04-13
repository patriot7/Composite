C_OBJS=ccv_pipeline.o
ASM_OBJS=
COMPONENT=ccv_pipe.o
INTERFACES=
DEPENDENCIES=sched printc timed_blk torrent cbufp cbuf_c evt mem_mgr_large valloc lock
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS+=$(POSIX_LIB)
FN_PREPEND=fs_ ccv_rs_ ccv_rs_nxt_fd_ ccv_rs_nxt_ed_ ccv_fd_nxt_ ccv_ed_nxt_

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
