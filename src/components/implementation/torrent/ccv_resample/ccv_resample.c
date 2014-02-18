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

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
       td_t ret = -1;
       struct torrent *t, *nt;

       if (td != td_root) return -EINVAL;

       assert(0);
       nt = tor_alloc(NULL, tflags);
       assert(0);
       nt->data = atoi(param);
       if (!nt) ERR_THROW(-ENOMEM, done);
       ret = nt->td;

       evt_trigger(cos_spd_id(), evtid);
       assert(0);
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
       cbufp_t cb;
       char *buf;
       struct torrent *t;
       cbuf_matrix_t *cbuf_mat;

       t = tor_lookup(td);
       assert(t);

       cbuf_mat = (cbuf_matrix_t *)t->data;
       buf = cbufp_alloc(cbuf_mat->size, &cb);
       cbufp_send_deref(cb);

       return cb;
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

       printc("begin twritep\n");

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       assert(t);

       ratio = (int)t->data;

       buf = cbufp2buf(cbid, sz);
       assert(buf);

       ccv_mat_input = cbuf2ccvmat((cbuf_matrix_t *)buf);

       ccv_resample(ccv_mat_input, &ccv_mat_output, 0, ccv_mat_input->rows / ratio, ccv_mat_input->cols / ratio, CCV_INTER_AREA);

       cbuf_matrix_t *cbuf_mat = ccv2cbufmat(ccv_mat_output);
       /*send the cbuf_mat  to next component  */

       return ret; /*TODO: ret value */ 
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
