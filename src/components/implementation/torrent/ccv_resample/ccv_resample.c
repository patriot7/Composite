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
#include "../../../lib/libccv/ccv.h"

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
       ccv_dense_matrix_t *input, *output;

       if (tor_isnull(td)) return -EINVAL;

       output = NULL;
       if (td_root != td) {
              t = tor_lookup(td);
              if (!t) ERR_THROW(-EINVAL, done);
              input = t->data;
              ccv_resample(input, &output, 0, input->rows / len, input->cols / len, (int)param);
              if (!output) return -ENOENT;
       }
       nt = tor_alloc(output, tflags);
       if (!nt) ERR_THROW(-ENOMEM, done);
       ret = nt->td;

       evt_trigger(cos_spd_id(), evtid);
done:
       return ret;
}

int 
tmerge(spdid_t spdid, td_t td, td_t td_into, char *param, int len)
{
       printc("tmerge is not defined for this torrent\n");
       return -EINVAL;
}

void
trelease(spdid_t spdid, td_t td)
{
	struct torrent *t;

	if (!tor_is_usrdef(td)) return;

	LOCK();
	t = tor_lookup(td);
	if (!t) goto done;
        ccv_matrix_free((ccv_dense_matrix_t *)t->data);
	tor_free(t);
done:
	UNLOCK();
	return;
}

int 
tread(spdid_t spdid, td_t td, int cbid, int sz)
{
       int ret = -1;
       struct torrent *t;
       char *buf;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       if (!t) ERR_THROW(-EINVAL, done);
       assert(!tor_is_usrdef(td) || t->data);
       if (!(t->flags & TOR_READ)) ERR_THROW(-EACCES, done);
       if (!t->data) ERR_THROW(0, done);

       buf = cbuf2buf(cbid, sizeof(ccv_dense_matrix_t));
       if (!buf) ERR_THROW(-EINVAL, done);

       memcpy(buf, t->data, sizeof(ccv_dense_matrix_t));
       ret = sizeof(ccv_dense_matrix_t);
done:	
       return ret;
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       int ret = -1;
       struct torrent *t;
       char *buf;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       if (!t) ERR_THROW(-EINVAL, done);
       assert(t->data);
       if (!(t->flags & TOR_WRITE)) ERR_THROW(-EACCES, done);

       buf = cbuf2buf(cbid, sz);
       if (!buf) ERR_THROW(-EINVAL, done);

       memcpy(buf, tp>data, sizeof(ccv_dense_matrix_t));
       ret = sizeof(ccv_dense_matrix_t);
done:
       return ret;
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
