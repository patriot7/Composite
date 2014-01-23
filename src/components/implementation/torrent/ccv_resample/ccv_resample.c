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
#include "../../../lib/libccv/ccv.h"

typedef struct {
        int type;
        int rows;
        int cols;
        int step;
        int datasize;
        unsigned char *data; /* u8 */ 
} cbufp_mat_t;

cbufp_mat_t *cbufp_mat_new(ccv_dense_matrix_t *ccv_mat);

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
       td_t ret = -1;
       struct torrent *t, *nt;

       if (tor_isnull(td)) return -EINVAL;

       if (td == td_root) { 
	      nt = tor_alloc(NULL, tflags);
	      if (!nt) ERR_THROW(-ENOMEM, done);
	      ret = nt->td;
       } else {
	      t = tor_lookup(td);
	      assert(t);
	      cbufp_mat_t *cbufp_mat = (cbufp_mat_t *)t->data;

              ccv_dense_matrix_t *ccv_mat;
              ccv_mat = (ccv_dense_matrix_t *)malloc(sizeof(ccv_dense_matrix_t));
	      assert(ccv_mat);
              ccv_mat->rows = cbufp_mat->rows;
              ccv_mat->cols = cbufp_mat->cols;
              ccv_mat->step = cbufp_mat->step;
              ccv_mat->type = cbufp_mat->type;
              ccv_mat->data.u8 = malloc(cbufp_mat->datasize);

              memcpy((void *)ccv_mat->data.u8, (void *)cbufp_mat->data, cbufp_mat->datasize);

	      ccv_dense_matrix_t *new_ccv_mat = 0;
              printc("begin resample\n");
	      ccv_resample(ccv_mat, &new_ccv_mat, ccv_mat->type, ccv_mat->rows / 5, ccv_mat->cols / 5, CCV_INTER_AREA);
	      printc("new rows = %d\n", new_ccv_mat->rows);
	      printc("new step = %d\n", new_ccv_mat->step);

	      cbufp_mat_t *new_data = cbufp_mat_new(new_ccv_mat);
              printc("create new mat good\n");

	      nt = tor_alloc(NULL, tflags);
	      if (!nt) ERR_THROW(-ENOMEM, done);
	      nt->data = new_data;
	      ret = nt->td;
       }

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
       cbufp_t cb;
       char *buf;
       struct torrent *t;

       t = tor_lookup(td);
       assert(t);

       /**off = 0;*/
       /**len = sizeof(cbufp_mat_t) + ((cbufp_mat_t *)t->data)->datasize;*/
       int _off, _len;
       _len = sizeof(cbufp_mat_t) + ((cbufp_mat_t *)t->data)->datasize;
       printc("len = %d\n", _len);

       buf = cbufp_alloc(_len, &cb);
       assert(buf);
       memcpy(buf, t->data, _len);
       ((cbufp_mat_t *)buf)->data = ((cbufp_mat_t *)buf)->data + 4;
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

       char *buf;
       struct torrent *t;
       cbufp_mat_t *cbufp_mat;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       assert(t);

       buf = cbufp2buf(cbid, sz);
       assert(buf);

       t->data = buf;

       ((cbufp_mat_t *)t)->data = &(((cbufp_mat_t *)t)->data) + 4;

       return ret; /*TODO: ret value */ 
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}

cbufp_mat_t *
cbufp_mat_new(ccv_dense_matrix_t *ccv_mat)
{
        cbufp_mat_t* cbufp_mat = malloc(sizeof(cbufp_mat_t) + ccv_mat->rows * ccv_mat->cols);
        assert(cbufp_mat);

        cbufp_mat->type = ccv_mat->type;
        cbufp_mat->rows = ccv_mat->rows;
        cbufp_mat->cols = ccv_mat->cols;
        cbufp_mat->step = ccv_mat->step;
        cbufp_mat->datasize = ccv_mat->rows * ccv_mat->cols;
        cbufp_mat->data = &(cbufp_mat->data) + 4;

        printc("cbufp_mat->rows = %d\n", cbufp_mat->rows);
        printc("cbufp_mat->cols = %d\n", cbufp_mat->cols);
        printc("cbufp_mat->step = %d\n", cbufp_mat->step);
        printc("cbufp_mat->datasize= %d\n", cbufp_mat->datasize);

        memcpy((void *)cbufp_mat->data, (void *)ccv_mat->data.u8, cbufp_mat->datasize);

        return cbufp_mat; 
}
