#include <cos_component.h>
#include <stdio.h>
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>
#include "../../../lib/libccv/ccv.h"
#include "../../../lib/libccv/cbuf_ccv.h"

void
cos_init(void)
{
        ccv_disable_cache();

        /*cbufp_t cb;*/
        /*char *buf;*/
        /*int mat_size;*/
        /*long evtid;*/
        /*td_t td;*/

        ccv_dense_matrix_t *mat = NULL;
        ccv_read("photo.bmp", &mat, CCV_IO_ANY_FILE | CCV_IO_GRAY);

        printc("mat->rows = %d\nmat->cols = %d\n", mat->rows, mat->cols);

        cbuf_matrix_t *cbuf_mat = ccv2cbufmat(mat);

        printc("cbuf_mat->rows = %d\ncbuf_mat->cols = %d\ncbuf_mat->cbid = %lu\n", cbuf_mat->rows, cbuf_mat->cols, cbuf_mat->cbid);

        free(mat);
        mat = cbuf2ccvmat(cbuf_mat);

        printc("mat->rows = %d\nmat->cols = %d\n", mat->rows, mat->cols);

        /*evtid = evt_split(cos_spd_id(), 0, 0);*/
        /*td = ccv_rs_tsplit(cos_spd_id(), td_root, "", 0, TOR_ALL, evtid);*/

        /*ccv_rs_twritep(cos_spd_id(), td, cbufp_mat->cbid, cbufp_mat->size);*/

        /*cbufp_t cb_out;*/
        /*int off, len;*/
        /*cb_out = ccv_rs_treadp(cos_spd_id(), td, &off, &len);*/
        /*buf = cbufp2buf(cb_out, len);*/

        return;
}
