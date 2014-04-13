C_OBJS=ccv_resample.o
ASM_OBJS=
COMPONENT=ccv_rs.o
INTERFACES=torrent
DEPENDENCIES=printc cbufp cbuf_c evt lock mem_mgr_large valloc torrent
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS += $(POSIX_LIB)
FN_PREPEND=fs_ ccv_rs_nxt_

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir

