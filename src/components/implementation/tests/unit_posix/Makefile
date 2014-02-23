C_OBJS=posix_test.o
ASM_OBJS=
COMPONENT=posix_t.o
INTERFACES=
DEPENDENCIES=sched printc timed_blk torrent cbufp cbuf_c evt mem_mgr_large valloc lock
<<<<<<< HEAD:src/components/implementation/no_interface/posix_test/Makefile
IF_LIB=./cos_libccv.a
#POSIX_LIB = ../../../lib/posix/posix.o
=======
IF_LIB=./cos_libccv.a $(DIETDIR)/bin-i386/libm.a
>>>>>>> origin/warning_fix:src/components/implementation/tests/unit_posix/Makefile
OBJLIBS += $(POSIX_LIB)

pre: first all
first:
	$(info |     [CP]   copying cos_libccv.a from libccv)
	@cp $(CCVOBJ) .

include ../../Makefile.subsubdir
