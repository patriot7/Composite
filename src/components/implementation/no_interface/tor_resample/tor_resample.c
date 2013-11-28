#include <cos_component.h>
#include <stdio.h>
#include "../../../lib/libccv/ccv.h"
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>

extern td_t file_tsplit(spdid_t spdid, td_t tid, char *param, int len, tor_flags_t tflags, long evtid);
extern int file_tread(spdid_t spdid, td_t td, int cbid, int sz);
extern int file_twmeta(spdid_t spdid, td_t td, const char *key, unsigned int klen, const char *val, unsigned int vlen);

/*extern td_t ccv_io_tsplit(spdid_t spdid, td_t tid, char *param, int len, tor_flags_t tflags, long evtid);*/
/*extern td_t ccv_res_tsplit(spdid_t spdid, td_t tid, char *param, int len, tor_flags_t tflags, long evtid);*/
/*ccv_res_tread*/

void
cos_init(void)
{
        ccv_dense_matrix_t* image = 0;
        ccv_read("photo_xs.bmp", &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);
        assert(image);

        cbuf_t input_mat_cb;
        void *d;
        printc("sizeof(ccv_dense_matrix_t) = %d\n", sizeof(ccv_dense_matrix_t));
        d = cbuf_alloc(sizeof(ccv_dense_matrix_t), &input_mat_cb);
        assert(d);
        memcpy(d, image, sizeof(ccv_dense_matrix_t));

        char val[8];
        long evt_res = evt_split(cos_spd_id(), 0, 0);
        snprintf(val, 8, "%d", input_mat_cb);
        printc("cbid: %d\n", input_mat_cb);
        ccv_res_tsplit(cos_spd_id(), td_root, val, strlen(val), TOR_ALL, evt_res);
}

void
cos_init_old(void)
{
        int ret; /* used for test tread return value */
        char *d;
        long evt_resample;
        td_t output_mat_td;
        char val[8];
        
        evt_resample = evt_split(cos_spd_id(), 0, 0);

        /* open file */
        td_t file_td;
        long evt_open = evt_split(cos_spd_id(), 0, 0);
        cbuf_t file_sz_cb, file_cb;
        int file_size;
        char *file_data = NULL;

        file_td = file_tsplit(cos_spd_id(), td_root, "photo_xs.bmp", strlen("photo_xs.bmp"), TOR_ALL, evt_open);
        assert(file_td > 0);

        snprintf(val, 8, "%ld", 2);
        file_twmeta(cos_spd_id(), file_td, "offset", strlen("offset"), val, strlen(val)); /* file size info starts at 2 bytes */
        d = cbuf_alloc(4, &file_sz_cb);
        assert(d);
        ret = file_tread(cos_spd_id(), file_td, file_sz_cb, 4); /* read out the file size */
        assert(ret > 0);
        memcpy(&file_size, d, 4);
        /*cbuf_free(d);*/
        snprintf(val, 8, "%ld", 0);
        file_twmeta(cos_spd_id(), file_td, "offset", strlen("offset"), val, strlen(val)); /* reset the offset */

        d = cbuf_alloc(file_size, &file_cb);
        assert(d);
        ret = file_tread(cos_spd_id(), file_td, file_cb, file_size);
        assert(ret > 0);
        printc("file_size = %d\n", file_size);
        /*cbuf_free(d);*/

        /* read file into matrix */
        td_t input_mat_td;
        long evt_read = evt_split(cos_spd_id(), 0, 0);
        cbuf_t input_mat_cb; 
        char *input_mat_data = NULL;

        printc("orig: %d\n", file_cb);
        snprintf(val, 8, "%d", file_cb);

        input_mat_td = ccv_rd_tsplit(cos_spd_id(), td_root, val, file_size, TOR_ALL, evt_read);
        assert(input_mat_td > 0);
        /*d = cbuf_alloc(sizeof(ccv_dense_matrix_t), &input_mat_cb);*/
        /*ret = ccv_rd_tread(cos_spd_id(), input_mat_td, input_mat_cb, sizeof(ccv_dense_matrix_t));*/
        /*assert(ret > 0);*/
        /*input_mat_data = (char *)malloc(sizeof(ccv_dense_matrix_t));*/
        /*memcpy(input_mat_data, d, sizeof(ccv_dense_matrix_t)); [> FIXME: cp only mat info, not mat->data <]*/
        /*printc("rows: %d, cols: %d\n", ((ccv_dense_matrix_t *)input_mat_data)->rows, ((ccv_dense_matrix_t *)input_mat_data)->cols);*/

        /* resmaple matrix */ 
        /*char param2[2];*/
        /*sprintf(param2, "%d\n", CCV_INTER_AREA);*/
        /*printc("%s", param2);*/
        /*output_mat_td = ccv_res_tsplit(cos_spd_id(), input_mat_td, param2, 2, TOR_ALL, evt_resample);*/

	/*ret = ccv_res_tread(cos_spd_id(), output_mat_td, cb, sizeof(ccv_dense_matrix_t));*/
        /*printc("%d\n", ret);*/
        /*assert(ret >= 0);*/
	/*memcpy(mat, d, sizeof(ccv_dense_matrix_t));*/

        /*printc("rows: %d, cols: %d\n", mat->rows, mat->cols);*/

        return;
}
