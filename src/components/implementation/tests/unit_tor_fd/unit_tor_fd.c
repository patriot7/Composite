#include <cos_component.h>
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>
#include <string.h>
#include "../../../lib/libccv/cbuf_ccv.h"

extern td_t ccv_fd_tsplit(spdid_t spdid, td_t td, char *param, int len, tor_flags_t tflags, long evtid);
extern int ccv_fd_twritep(spdid_t spdid, td_t td, int cbid, int sz);
extern int ccv_fd_treadp(spdid_t spdid, td_t td, int *off, int *len);

void
cos_init(void)
{
        ccv_disable_cache();

        cbufp_t cbid;
        char *buf;
        int mat_size;
        long evt_tsplit;
        td_t td;
        ccv_dense_matrix_t *ccv_mat_input = NULL;
        ccv_dense_matrix_t *ccv_mat_output = NULL;

        ccv_read("photo.bmp", &ccv_mat_input, CCV_IO_ANY_FILE | CCV_IO_GRAY);

        evt_tsplit = evt_split(cos_spd_id(), 0, 0);
        td = ccv_fd_tsplit(cos_spd_id(), td_root, "face", strlen("face"), TOR_ALL, evt_tsplit);

        assert(ccv_mat_input);

        cbuf_matrix_t *cbuf_mat = ccv2cbufmat(ccv_mat_input);
        cbufp_send_deref(cbuf_mat->cbid);
        ccv_fd_twritep(cos_spd_id(), td, cbuf_mat->cbid, cbuf_mat->size);

        /* test of treapd, should same with the input*/ 
        int off, len;

        cbid = ccv_fd_treadp(cos_spd_id(), td, &off, &len);
        buf = cbufp2buf(cbid, len);
        cbuf_mat = (cbuf_matrix_t *)buf;
        ccv_mat_output = cbuf2ccvmat(cbuf_mat);

        return;
}
