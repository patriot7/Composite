#include <cos_component.h>
/*#include <stdio.h>*/
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>
#include <string.h>
#include "../../../lib/libccv/cbuf_ccv.h"

extern td_t ccv_fd_tsplit(spdid_t spdid, td_t td, char *param, int len, tor_flags_t tflags, long evtid);
extern int ccv_fd_twritep(spdid_t spdid, td_t td, int cbid, int sz);

void
cos_init(void)
{
        ccv_disable_cache();

        cbufp_t cb;
        char *buf;
        int mat_size;
        long evt_tsplit;
        td_t td;

        /*int i;*/
        /*void *test[64];*/
        /*for (i = 0; i < 64; i++){*/
                /*printc("alloc/write %d\n", i);*/
                /*test[i] = malloc(1024 * 1024);*/
                /**(int *)test[i] = i;*/
/*}*/
        /*for (i = 0; i < 64; i++)*/
                /*free(test[i]);*/

        evt_tsplit = evt_split(cos_spd_id(), 0, 0);
        td = ccv_fd_tsplit(cos_spd_id(), td_root, "face", strlen("face"), TOR_ALL, evt_tsplit);

        ccv_dense_matrix_t *mat = NULL;
        ccv_read("photo.bmp", &mat, CCV_IO_ANY_FILE | CCV_IO_GRAY);
        assert(mat);
        printc("ccv_read finished\n");

        cbuf_matrix_t *cbuf_mat = ccv2cbufmat(mat);
        cbufp_send_deref(cbuf_mat->cbid);
        ccv_fd_twritep(cos_spd_id(), td, cbuf_mat->cbid, cbuf_mat->size);
        //

        /*cbufp_t cbid;*/
        /*int off, len;*/
        /*cbid = treadp(cos_spd_id(), td, &off, &len);*/

        /*buf = cbufp2buf(cbid, len);*/
        /*cbuf_mat = (cbuf_matrix_t *)buf; */

        return;
}
