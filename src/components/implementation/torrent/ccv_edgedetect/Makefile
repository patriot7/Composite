C_OBJS=ccv_edgedetect.o
ASM_OBJS=
COMPONENT=ccv_ed.o
INTERFACES=torrent
DEPENDENCIES=printc cbufp cbuf_c evt lock mem_mgr_large valloc torrent
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS += $(POSIX_LIB)
FN_PREPEND=fs_ ccv_ed_nxt_

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
