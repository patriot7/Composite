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

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
        int len, tor_flags_t tflags, long evtid) 
{
        printc("called ccv_read\n");
        td_t ret = -1;
        struct torrent *t;
        ccv_dense_matrix_t *mat = 0;
        char *buf;

        if (tor_isnull(td)) return -EINVAL;
        assert(td_root == td);

        /*cbuf_t cb_src = atoi(param);*/
        /*printc("converted: %d\n", cb_src);*/
        /*printc("len: %d\n", len);*/

        /*buf = cbuf2buf(cb_src, len);*/
        /*assert(buf);*/
        /*void *file_data = malloc(len);*/
        /*memcpy(file_data, buf, len);*/
        ccv_read(param, &mat, CCV_IO_GRAY | CCV_IO_ANY_FILE);
        assert(mat);
        printc("orig: rows = %d, cols = %d\n", mat->rows, mat->cols);
        t = tor_alloc(mat, tflags);
        ret = t->td;

        evt_trigger(cos_spd_id(), evtid);

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

	t = tor_lookup(td);
	if (!t) goto done;
        ccv_matrix_free((ccv_dense_matrix_t *)t->data);
	tor_free(t);
done:
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

        if (!t) assert(0);
        if (!t) ERR_THROW(-EINVAL, done);
        assert(!tor_is_usrdef(td) || t->data);
        if (!(t->flags & TOR_READ)) ERR_THROW(-EACCES, done);
        if (!t->data) ERR_THROW(0, done);

        buf = cbuf2buf(cbid, sizeof(ccv_dense_matrix_t));
        assert(buf);
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

        memcpy(buf, t->data, sizeof(ccv_dense_matrix_t));
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
