#include <cos_component.h>
#include <stdio.h>
#include "../../../lib/libccv/ccv.h"
#include <evt.h>
#include <torrent.h>
#include <print.h>
#include <cbuf.h>
#include <cos_alloc.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct {
        int type;
        int rows;
        int cols;
        int step;
        int datasize;
        unsigned char *data; /* use u8 for bmp */ 
} cbufp_mat_t;

cbufp_mat_t *
cbufp_mat_new(ccv_dense_matrix_t *ccv_mat)
{
        assert(ccv_mat != NULL);
        int bufstep = (ccv_mat->cols * 3 + 3) & -4; /* from ccv_io_bmp, only for 24bpp bitmap */
        printc("bufsetp = %d\n", bufstep);
        cbufp_mat_t* cbufp_mat = malloc(sizeof(cbufp_mat_t) + ccv_mat->rows * bufstep);

        cbufp_mat->type = ccv_mat->type;
        cbufp_mat->rows = ccv_mat->rows;
        cbufp_mat->cols = ccv_mat->cols;
        cbufp_mat->step = ccv_mat->step;
        cbufp_mat->datasize = bufstep * ccv_mat->rows;

        cbufp_mat->data = &(cbufp_mat->data) + 4;

        memcpy((void *)cbufp_mat->data, (void *)ccv_mat->data.u8, cbufp_mat->datasize);
        printc("cbufp_mat->rows = %d\n", cbufp_mat->rows);
        printc("cbufp_mat->cols = %d\n", cbufp_mat->cols);
        printc("cbufp_mat->step = %d\n", cbufp_mat->step);
        printc("cbufp_mat->datasize= %d\n", cbufp_mat->datasize);

        return cbufp_mat; 
}

void
cos_init(void)
{
        ccv_disable_cache();

        cbufp_t cb;
        char *buf;
        int mat_size;
        /*struct cbuf_alloc_desc *d;*/
        long evtid = evt_split(cos_spd_id(), 0, 0);

        ccv_dense_matrix_t *ccv_mat = 0;
        cbufp_mat_t *cbufp_mat = 0;

        ccv_read("photo.bmp", &ccv_mat, CCV_IO_ANY_FILE | CCV_IO_GRAY);
        cbufp_mat = cbufp_mat_new(ccv_mat);
        assert(cbufp_mat);
        /*ccv_matrix_free_immediately(ccv_mat);*/

        td_t t = ccv_res_tsplit(cos_spd_id(), td_root, "", strlen(""), TOR_ALL,  evtid);
        /*d = &cb;*/
        mat_size = sizeof(cbufp_mat_t) + cbufp_mat->datasize; /* header + data */ 
        buf = cbufp_alloc(mat_size, &cb);
        assert(buf);
        memcpy(buf, cbufp_mat, mat_size);
        cbufp_send_deref(cb);

        ccv_res_twritep(cos_spd_id(), t, cb, mat_size);
        td_t nt = ccv_res_tsplit(cos_spd_id(), t, "", strlen(""), TOR_ALL, evtid);

        int off, len;
        cbuf_t cb_new = (cbuf_t)ccv_res_treadp(cos_spd_id(), nt, &off, &len);
        char *buf_new = cbufp2buf(cb_new, sizeof(cbufp_mat_t));
        
        printc("buf_new->cols = %d\n", ((cbufp_mat_t *)buf_new)->cols);

        return;
}
