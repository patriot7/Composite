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

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
       td_t ret = -1;
       struct torrent *t, *nt;
       ccv_dense_matrix_t *mat;

       if (tor_isnull(td)) return -EINVAL;

       mat = NULL;
       if (td_root != td) {
              t = tor_lookup(td);
              if (!t) ERR_THROW(-EINVAL, done);
              input = t->name;
              ccv_read(file, &output, 0, input->rows / len, input->cols / len, (int)param);
              if (!output) return -ENOENT;
       }
       nt = tor_alloc(mat, tflags);
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
done:	
       return sizeof(ccv_dense_matrix_t);
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       struct torrent *t;
       char *buf;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       if (!t) ERR_THROW(-EINVAL, done);
       assert(t->data);
       if (!(t->flags & TOR_WRITE)) ERR_THROW(-EACCES, done);

       buf = cbuf2buf(cbid, sz);
       if (!buf) ERR_THROW(-EINVAL, done);

       memcpy(buf, t->data, sizeof(ccv_dense_matrix_t));
done:
       return sizeof(ccv_dense_matrix_t);
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
