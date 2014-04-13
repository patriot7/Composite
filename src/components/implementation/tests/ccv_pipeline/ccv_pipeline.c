#include <cos_component.h>
#include <stdio.h>
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>
#include <string.h>
#include "../../../lib/libccv/cbuf_ccv.h"

extern td_t ccv_rs_tsplit(spdid_t spdid, td_t td, char *param, int len, tor_flags_t tflags, long evtid);
extern int ccv_rs_twritep(spdid_t spdid, td_t td, int cbid, int sz);
/*extern int ccv_rs_treadp(spdid_t spdid, td_t td, int *off, int *len);*/

void
cos_init(void)
{
        ccv_disable_cache();

        cbufp_t cbid;
        char *buf;
        int mat_size;
        long evtid;
        td_t td_rs_orig, td_rs_scale_down;

        ccv_dense_matrix_t *ccv_mat_input = NULL;
        ccv_dense_matrix_t *ccv_mat_output = NULL;

        /* this is wired ... we have to pu tsplit before ccv_read (which calls cbufp_alloc), or we cannot even enter tsplit */ 
        evtid = evt_split(cos_spd_id(), 0, 0);
        td_rs_scale_down = ccv_rs_tsplit(cos_spd_id(), td_root, "5", strlen("5"), TOR_ALL, evtid);
        td_rs_orig = ccv_rs_tsplit(cos_spd_id(), td_root, "1", strlen("1"), TOR_ALL, evtid);

        ccv_read("photo.bmp", &ccv_mat_input, CCV_IO_ANY_FILE | CCV_IO_GRAY);
        assert(ccv_mat_input);

        cbuf_matrix_t *cbuf_mat = ccv2cbufmat(ccv_mat_input);
        /*cbufp_send_deref(cbuf_mat->cbid);*/
        cbufp_send(cbuf_mat->cbid);
        ccv_rs_twritep(cos_spd_id(), td_rs_orig, cbuf_mat->cbid, cbuf_mat->size);

        /*int off, len;*/
        /*cbid = ccv_rs_treadp(cos_spd_id(), td, &off, &len);*/

        /*buf = cbufp2buf(cbid, len);*/
        /*cbuf_mat = (cbuf_matrix_t *)buf; */

        return;
}
