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

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
       td_t ret = -1;
       struct torrent *t, *nt;

       if (td != td_root) return -EINVAL;

       nt = tor_alloc(NULL, tflags);
       nt->data = atoi(param);
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
       return -1;
}

int
treadp(spdid_t spdid, td_t td, int *off, int *len)
{
       *off = 0;
       *len = cbuf_mat_tmp->size;
       cbufp_send_deref(cbuf_mat_tmp->cbid);

       return cbuf_mat_tmp->cbid;
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       return -1;
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

       cbuf_mat_tmp = ccv2cbufmat(ccv_mat_output);

       /* send the matrix to facedetect torrent */ 
       long evtid = evt_split(cos_spd_id(), 0, 0);
       /*next_stage_tsplit(cos_spd_id(), td_root, "", strlen(""), TOR_ALL, evtid);*/
       /*cbufp_send_deref(cbuf_mat->cbid);*/
       /*ccv_fd_twritep(cos_spd_id(), td, cbuf_mat->cbid, cbuf_mat->size);*/

       return ret; /*TODO: ret value */ 
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
