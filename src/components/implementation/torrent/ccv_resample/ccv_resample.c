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

/**
 * td     : input dense matrix
 * param  : CCV_INTER_AREA(downsampling), CCV_INTER_CUBIC(upsampling) // TODO: or use simpler notation?
 * len	  : multiples of upsamling or downsampling // TODO: accept decimal?
 * return : output dense matrix
 *
 * TODO: param as row or col to achieve finer-grained sampling? 
 */

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

	      assert(t->data);
	      cbufp_mat_t *cbufp_mat = (cbufp_mat_t *)t->data;

	      ccv_dense_matrix_t *ccv_mat = ccv_dense_matrix_new(cbufp_mat->rows, cbufp_mat->cols, cbufp_mat->type, 0, 0);
	      assert(ccv_mat);

	      ccv_mat->data.u8 = cbufp_mat->data;

	      ccv_dense_matrix_t *new_ccv_mat = 0;
	      ccv_resample(ccv_mat, &new_ccv_mat, 0, ccv_mat->rows / 5, ccv_mat->cols / 5, CCV_INTER_AREA);
	      printc("new rows = %d\n", new_ccv_mat->rows);
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
treadp(spdid_t spdid, int sz, int *off, int *len)
{
       return -1;
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

       buf = cbufp2buf(cbid, sz);
       assert(buf);

       t = tor_lookup(td);
       assert(t);

       t->data = buf;
       /* ret = len_of_writes */
done:
       return ret;
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
