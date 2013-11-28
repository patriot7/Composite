#include <cos_component.h>
#include <stdio.h>
#include "../../../lib/libccv/ccv.h"
#include <evt.h>
#include <torrent.h>

extern td_t tor_lookup(td_t t);
extern td_t tor_alloc(void *data, tor_flags_t flags);

void
cos_init(void)
{
        long evt_open = evt_split(cos_spd_id(), 0, 0);
        long evt_resample = evt_split(cos_spd_id(), 0, 0); /* use the same evtid ?*/ 

        ccv_dense_matrix_t* image = 0;
        ccv_read("photo.bmp", &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);

        td_t input = file_tsplit(cos_spd_id(), td_root, "photo.bmp", strlen("photo.bmp"), TOR_ALL, evt_open);

        td_t output = tsplit(cos_spd_id(), input, CCV_INTER_AREA, 2, TOR_ALL, evtid);

        ccv_matrix_free(image);  /* what about torrent for this resources? */
        
        image = tor_lookup(output);
        assert(image->rows == image->rows / 2);
        assert(image->cols == image->cols / 2);
}
