/**
 * Copyright 2013 by GAO Xiang, gaox@gwu.edu
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 */

#include <cos_component.h>
#include <torrent.h>
#include <torlib.h>

#include <cbuf.h>
#include <print.h>
#include <evt.h>
#include <cos_alloc.h>
#include <cos_map.h>
#include <stdlib.h>
#include "../../../lib/libccv/cbuf_ccv.h"

cbuf_matrix_t *cbuf_mat_tmp;

extern td_t ccv_rs_nxt_tsplit(spdid_t spdid, td_t td, char *param, int len, tor_flags_t tflags, long evtid);
extern int ccv_rs_nxt_twritep(spdid_t spdid, td_t td, int cbid, int sz);

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
       td_t ret = -1;
       struct torrent *t, *nt;

       if (td != td_root) return -EINVAL;

       nt = tor_alloc(NULL, tflags);
       nt->data = (void *)atoi(param);
       if (!nt) ERR_THROW(-ENOMEM, done);
       ret = nt->td;

       evt_trigger(cos_spd_id(), evtid);
done:
       return ret;
}

int 
tmerge(spdid_t spdid, td_t td, td_t td_into, char *param, int len)
{
       return -EINVAL;
}

void
trelease(spdid_t spdid, td_t td)
{
	return;
}

int 
tread(spdid_t spdid, td_t td, int cbid, int sz)
{
        return -ENOSYS;
}

int
treadp(spdid_t spdid, td_t td, int *off, int *len)
{
       *off = 0;
       *len = cbuf_mat_tmp->size;
       /*cbufp_send_deref(cbuf_mat_tmp->cbid);*/
       cbufp_send(cbuf_mat_tmp->cbid);

       return cbuf_mat_tmp->cbid;
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       return -ENOSYS;
}

int
twritep(spdid_t spdid, td_t td, int cbid, int sz)
{
       int ret = -1;

       cbufp_t cb;
       char *buf;
       int ratio;
       struct torrent *t;
       ccv_dense_matrix_t *ccv_mat_input = NULL;
       ccv_dense_matrix_t *ccv_mat_output = NULL;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       assert(t);

       ratio = (int)t->data;

       buf = cbufp2buf(cbid, sz);
       assert(buf);
       ccv_mat_input = cbuf2ccvmat((cbuf_matrix_t *)buf);
       ccv_resample(ccv_mat_input, &ccv_mat_output, 0, ccv_mat_input->rows / ratio, ccv_mat_input->cols / ratio, CCV_INTER_AREA);
       printc("resample done, send to next stage\n");
       cbuf_mat_tmp = ccv2cbufmat(ccv_mat_output); /* write to the global temp buf for test */

       /* send the matrix to next stage */ 
       /* even though we could use arbitry "next_stage" torrent, we still to use particular parameter here */ 
       long evt_ed_tsplit = evt_split(cos_spd_id(), 0, 0);
       long evt_fd_tsplit = evt_split(cos_spd_id(), 0, 0);
        td_t td_ed = ccv_rs_nxt_ed_tsplit(cos_spd_id(), td_root, "3", strlen("3"), TOR_ALL, evt_ed_tsplit);
        td_t td_fd = ccv_rs_nxt_fd_tsplit(cos_spd_id(), td_root, "face", strlen("face"), TOR_ALL, evtid);
       printc("next tsplit done\n");

       /* as we also ref the cbuf using the global cbuf_mat_tmp for test, we don't use cbufp_send_deref here */
       /*cbufp_send_deref(cbuf_mat_tmp->cbid); */
       cbufp_send(cbuf_mat_tmp->cbid);
       printc("cbufp_send done\n");
       ccv_rs_nxt_fd_twritep(cos_spd_id(), next_td, cbuf_mat_tmp->cbid, cbuf_mat_tmp->size);
       ccv_rs_nxt_ed_twritep(cos_spd_id(), next_td, cbuf_mat_tmp->cbid, cbuf_mat_tmp->size);
       printc("next twritep done\n");

       return ret; /*TODO: ret value */ 
}


int 
cos_init(void)
{
       torlib_init();

       return 0;
}
