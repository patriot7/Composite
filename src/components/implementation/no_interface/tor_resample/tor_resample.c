#include <cos_component.h>
#include <stdio.h>
#include "../../../lib/libccv/ccv.h"
#include <evt.h>
#include <torrent.h>

extern td_t tor_lookup(td_t);
extern td_t tor_alloc(void *data, tor_flags_t);

void
cos_init(void)
{
        ccv_dense_matrix_t* image = 0;
        ccv_read("photo.bmp", &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);

        td_t input = tor_alloc(image, TOR_ALL);
        long evtid = evt_split(cos_spd_id(), 0, 0);
        td_t output = tsplit(cos_spd_id(), input, CCV_INTER_AREA, 2, TOR_ALL, evtid);

        ccv_matrix_free(image);  /* what about torrent for this resources? */
        
        image = tor_lookup(output);
        assert(image->rows == image->rows / 2);
        assert(image->cols == image->cols / 2);
}
