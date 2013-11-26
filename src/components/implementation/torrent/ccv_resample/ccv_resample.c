/**
 * Copyright 2011 by Gabriel Parmer, gparmer@gwu.edu
 *
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 */

#include <cos_component.h>
#include <torrent.h>
#include <torlib.h>

#include <cbuf.h>
#include <print.h>
#include <cos_synchronization.h>
#include <evt.h>
#include <cos_alloc.h>
#include <cos_map.h>
#include "../../../lib/libccv/ccv.h"

/**
 * td     : input dense matrix
 * return : output dense matrix
 * param  : CCV_INTER_AREA(downsampling), CCV_INTER_CUBIC(upsampling) // TODO: or use simpler notation?
 * len	  : multiples of upsamling or downsampling // TODO: accept decimal?
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
       t = tor_lookup(td);
       if (!t) ERR_THROW(-EINVAL, done);

       ccv_dense_matrix_t *output = NULL;
       ccv_dense_matrix_t *input = (ccv_dense_matrix_t *)t->data;
       ccv_resample(input, &output, 0, input->rows / len, input->cols / len, (int)param);
       if (!output) return -ENOENT;

       nt = tor_alloc(output, tflags);
       if (!nt) ERR_THROW(-ENOMEM, done);
       ret = nt->td;

       /* If we created the torrent, then trigger an event as we have data! */
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
       return -EINVAL;
}

int 
twrite(spdid_t spdid, td_t td, int cbid, int sz)
{
       return -EINVAL;
}

int 
cos_init(void)
{
       torlib_init();

       return 0;
}
