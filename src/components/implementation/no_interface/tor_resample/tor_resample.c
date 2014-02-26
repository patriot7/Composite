#include <cos_component.h>
#include <stdio.h>
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>
#include <string.h>
#include "../../../lib/libccv/cbuf_ccv.h"

void
cos_init(void)
{
        ccv_disable_cache();

        cbufp_t cb;
        char *buf;
        int mat_size;
        long evtid;
        td_t td;

        evtid = evt_split(cos_spd_id(), 0, 0);
        td = ccv_res_tsplit(cos_spd_id(), td_root, "5", strlen("5"), TOR_ALL, evtid);

        ccv_dense_matrix_t *mat = NULL;
        ccv_read("photo.bmp", &mat, CCV_IO_ANY_FILE | CCV_IO_GRAY);
        assert(mat);

        cbuf_matrix_t *cbuf_mat = ccv2cbufmat(mat);
	printc("address = %p\n", cbuf_mat->data);
        printc("mycomp cbid = %d\n", cbuf_mat->cbid);
        cbufp_send_deref(cbuf_mat->cbid);
        ccv_res_twritep(cos_spd_id(), td, cbuf_mat->cbid, cbuf_mat->size);

        /*cbufp_t cbid;*/
        /*int off, len;*/
        /*cbid = treadp(cos_spd_id(), td, &off, &len);*/

        /*buf = cbufp2buf(cbid, len);*/
        /*cbuf_mat = (cbuf_matrix_t *)buf; */

        return;
}
