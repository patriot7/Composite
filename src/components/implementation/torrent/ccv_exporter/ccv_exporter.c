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

cbuf_matrix_t *cbuf_mat_export;

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
        assert(0);
        return -EINVAL;
}

void
trelease(spdid_t spdid, td_t td)
{
        assert(0);
	return;
}

int 
tread(spdid_t spdid, td_t td, int cbid, int sz)
{
        assert(0);
        return -ENOSYS;
}

int
treadp(spdid_t spdid, td_t td, int *off, int *len)
{
       *off = 0;
       *len = cbuf_mat_export->size;
       cbufp_send_deref(cbuf_mat_export->cbid);

       return cbuf_mat_export->cbid;
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       assert(0);
       return -ENOSYS;
}

int
twritep(spdid_t spdid, td_t td, int cbid, int sz)
{
       int ret = -1;

       char *buf;
       struct torrent *t;
       cbuf_matrix_t *cbuf_mat;

       if (tor_isnull(td)) return -EINVAL;

       t = tor_lookup(td);
       assert(t);

       buf = cbufp2buf(cbid, sz);
       assert(buf);
       cbuf_mat_export = (cbuf_matrix_t *)buf;

       return ret; /*TODO: ret value */
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
