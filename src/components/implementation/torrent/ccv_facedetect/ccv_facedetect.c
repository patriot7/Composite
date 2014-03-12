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


extern td_t 
fs_tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid); 

td_t 
tsplit(spdid_t spdid, td_t td, char *param,
       int len, tor_flags_t tflags, long evtid) 
{
        printc("called ccv_fd_tsplit\n");
        td_t ret = -1;
        struct torrent *nt;

        if (td != td_root) return -EINVAL;

        nt = tor_alloc(NULL, tflags);
        ccv_bbf_classifier_cascade_t* cascade = ccv_bbf_read_classifier_cascade(param);
        printc("finished reading\n");
        /*nt->data = cascade;*/
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
       return -ENOSYS;
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
        ccv_bbf_classifier_cascade_t* cascade;
        struct torrent *t;
        ccv_dense_matrix_t *ccv_mat_input;

        if (tor_isnull(td)) return -EINVAL;

        t = tor_lookup(td);
        assert(t);

        cascade = (ccv_bbf_classifier_cascade_t *)t->data;

        buf = cbufp2buf(cbid, sz);
        assert(buf);

        ccv_mat_input = cbuf2ccvmat((cbuf_matrix_t *)buf);

        ccv_array_t* seq;
        seq = ccv_bbf_detect_objects(ccv_mat_input, &cascade, 1, ccv_bbf_default_params);
        printc("detect %d faces:\n", seq->rnum);
        int i;
        for (i = 0; i < seq->rnum; i++) {
               ccv_comp_t* comp = (ccv_comp_t*)ccv_array_get(seq, i);
               printc("%d %d %d %d %f\n", comp->rect.x, comp->rect.y, comp->rect.width, comp->rect.height, comp->confidence);
        }
        ccv_array_free(seq);

        /*cbuf_matrix_t *cbuf_mat = ccv2cbufmat(ccv_mat_output);*/
        /*cbufp_send_deref(cbuf_mat->cbid);*/

        return ret; /*TODO: ret value */ 
}

int 
cos_init(void)
{
        torlib_init();

        return 0;
}
