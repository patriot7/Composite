#include <cos_component.h>
#include <stdio.h>
#include "../../../lib/libccv/ccv.h"
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>


/*typedef union {*/
	/*unsigned char* u8;*/
	/*int* i32;*/
	/*float* f32;*/
	/*int64_t* i64;*/
	/*double* f64;*/
/*} ccv_matrix_cell_t;*/

ccv_dense_matrix_t *cbufp_ccv_dense_matrix_new(int rows, int cols, int type, void* data)
{
	ccv_dense_matrix_t* mat;

        /* the default allocation */
        /*mat = (ccv_dense_matrix_t*)(data ? data : ccmalloc(ccv_compute_dense_matrix_size(rows, cols, type)));*/
        /*mat->type = (CCV_GET_CHANNEL(type) | CCV_GET_DATA_TYPE(type) | CCV_MATRIX_DENSE) & ~CCV_GARBAGE;*/
        /*mat->type |= data ? CCV_UNMANAGED : CCV_REUSABLE;*/
        /*mat->data.u8 = (unsigned char*)(mat + 1);*/
        
        mat = (ccv_dense_matrix_t*)ccmalloc(sizeof(ccv_dense_matrix_t));
        mat->type = (CCV_GET_CHANNEL(type) | CCV_GET_DATA_TYPE(type) | CCV_MATRIX_DENSE | CCV_NO_DATA_ALLOC) & ~CCV_GARBAGE;
        mat->data.u8 = data;

	mat->rows = rows;
	mat->cols = cols;
	mat->step = (cols * CCV_GET_DATA_TYPE_SIZE(type) * CCV_GET_CHANNEL(type) + 3) & -4;

	return mat;
}






typedef struct {
        struct {
                int type;
                int rows;
                int cols;
                int step;
                int datasize;
        } header;
	ccv_matrix_cell_t data; /* use u8 for bmp */ 
} cbufp_ccv_dense_matrix_t;

cbufp_ccv_dense_matrix_t *
cbufp_mat_new(ccv_dense_matrix_t *ccv_mat)
{
        assert(ccv_mat != NULL);
        cbufp_ccv_dense_matrix_t *cbufp_mat = malloc(sizeof(cbufp_ccv_dense_matrix_t));

        cbufp_mat->header.type = ccv_mat->type;
        cbufp_mat->header.rows = ccv_mat->rows;
        cbufp_mat->header.cols = ccv_mat->cols;
        cbufp_mat->header.step = ccv_mat->step;
        cbufp_mat->header.datasize = (ccv_mat->cols * 3 + 3) & -4; /* from ccv_io_bmp, only for 24bpp bitmap */

        ccv_matrix_cell_t data;
        data.u8 = (unsigned char *)malloc(cbufp_mat->header.datasize);

        memcpy((void *)data.u8, (void *)ccv_mat->data.u8, cbufp_mat->header.datasize);
        cbufp_mat->data.u8 = data.u8;

        return cbufp_mat; 
}

void
cos_init(void)
{
        ccv_disable_cache();
        ccv_dense_matrix_t *ccv_mat = 0;
        ccv_read("photo.bmp", &ccv_mat, CCV_IO_ANY_FILE | CCV_IO_GRAY);

        cbufp_ccv_dense_matrix_t *cbufp_mat;
        cbufp_mat = cbufp_mat_new(ccv_mat);

        cbufp_t cb;
        char *buf;
        struct cbuf_alloc_desc *d;

        d = &cb;
        buf = cbufp_alloc(cbufp_mat->header.datasize, &cb);
        cbufp_send_deref(cb);

        return;
}
