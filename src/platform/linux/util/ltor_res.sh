#!/bin/sh

./cos_loader \
"c0.o, ;llboot.o, ;*fprr.o, ;mm.o, ;print.o, ;boot.o, ;\
\
!initfs.o,a3;!mpool.o,a3;!trans.o,a6;!sm.o,a4;!l.o,a1;!te.o,a3;!eg.o,a4;!buf.o,a5;!bufp.o, ;!tor_res.o,a8;!va.o,a2;!vm.o,a1;!ccv_rs.o, ;!rotar.o, :\
c0.o-llboot.o;\
fprr.o-print.o|[parent_]mm.o|[faulthndlr_]llboot.o;\
l.o-fprr.o|mm.o|print.o;\
te.o-sm.o|print.o|fprr.o|mm.o|va.o;\
mm.o-[parent_]llboot.o|print.o;\
boot.o-print.o|fprr.o|mm.o|llboot.o;\
eg.o-sm.o|fprr.o|print.o|mm.o|l.o|va.o;\
initfs.o-fprr.o|print.o|buf.o|bufp.o|va.o|l.o|mm.o;\
sm.o-print.o|fprr.o|mm.o|boot.o|l.o|mpool.o|va.o;\
buf.o-boot.o|sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o;\
bufp.o-sm.o|fprr.o|print.o|l.o|mm.o|va.o|mpool.o|buf.o;\
mpool.o-print.o|fprr.o|mm.o|boot.o|va.o|l.o;\
vm.o-fprr.o|print.o|mm.o|l.o|boot.o;\
va.o-fprr.o|print.o|mm.o|l.o|boot.o|vm.o;\
rotar.o-sm.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|eg.o|va.o|initfs.o;\
ccv_rs.o-sm.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|eg.o|va.o|initfs.o;\
tor_res.o-sm.o|fprr.o|print.o|mm.o|buf.o|bufp.o|l.o|eg.o|va.o|initfs.o|rotar.o|[ccv_res_]ccv_rs.o;\
trans.o-sm.o|fprr.o|l.o|buf.o|bufp.o|mm.o|va.o|eg.o|print.o\
" ./gen_client_stub
