C_OBJS=ccv_facedetect.o
ASM_OBJS=
COMPONENT=ccv_fd.o
INTERFACES=torrent
DEPENDENCIES=printc cbufp cbuf_c evt lock mem_mgr_large valloc
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
OBJLIBS += $(POSIX_LIB)

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
