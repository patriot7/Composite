#include "cbuf_ccv.h"

extern int printc(char *fmt, ...);

void *cbuf_ccv_alloc(unsigned int sz);
void cbuf_ccv_free(void *ptr);

void
cbuf_ccv_alloc_creator(ccv_alloc_fn_t *fn)
{
	fn = cbuf_ccv_alloc;
	return;
}

void
cbuf_ccv_free_creator(ccv_free_fn_t *fn)
{
	fn = cbuf_ccv_free;
	return;
}

void *
cbuf_ccv_alloc(unsigned int sz)
{
       /*
        * sz = ccv_header + data
        * cbuf_size = cbuf_header +data
	*/
	unsigned int cbuf_size = sz - sizeof(ccv_dense_matrix_t) + sizeof(cbuf_matrix_t);
	cbufp_t cb;
	ccv_dense_matrix_t *mat = NULL;
	cbuf_matrix_t *cbuf_mat = NULL;

	printc("cbuf_ccv: before cbufp_alloc\n");
	cbuf_mat = (cbuf_matrix_t *)cbufp_alloc(cbuf_size, &cb);
	printc("cbuf_ccv: after cbufp_alloc\n");
	assert(cbuf_mat);

	mat = (ccv_dense_matrix_t *)malloc(sizeof(ccv_dense_matrix_t));
	assert(mat);

	cbuf_mat->cbid = cb;
	cbuf_mat->size = cbuf_size;
	mat->data.u8 = cbuf_mat->data;

	return mat;
}

void
set_cbuf_header(ccv_dense_matrix_t *mat)
{
	cbuf_matrix_t *cbuf_mat = (cbuf_matrix_t *)(mat->data.u8 - sizeof(cbuf_matrix_t));
	cbuf_mat->type = mat->type;
	cbuf_mat->rows = mat->rows;
	cbuf_mat->cols = mat->cols;
	cbuf_mat->step = mat->step;

	return;
}

void
cbuf_ccv_free(void *ptr)
{
	printc("called cbuf_ccv_free\n");
	ccv_dense_matrix_t *mat = NULL;
	cbuf_matrix_t *cbufp_mat = NULL;

	mat = (ccv_dense_matrix_t *)ptr;
	cbufp_mat = (cbuf_matrix_t *)(mat - sizeof(cbuf_matrix_t));

	printc("ccv_cbufp_free cbid %d\n", cbufp_mat->cbid);
	cbufp_deref(cbufp_mat->cbid);
	free(mat);

	return;
}

cbuf_matrix_t *
ccv2cbufmat(ccv_dense_matrix_t *mat)
{
	cbuf_matrix_t *cbuf_mat = NULL;
	cbuf_mat = (cbuf_matrix_t *)(mat->data.u8 - sizeof(cbuf_matrix_t));

	return cbuf_mat;
}

ccv_dense_matrix_t *
cbuf2ccvmat(cbuf_matrix_t *cbuf_mat)
{
	ccv_dense_matrix_t *mat = NULL;

	mat = malloc(sizeof(ccv_dense_matrix_t));
	mat->type = cbuf_mat->type;
	mat->rows = cbuf_mat->rows;
	mat->cols = cbuf_mat->cols;
	mat->step = cbuf_mat->step;
	mat->data.u8 = cbuf_mat->data;

	return mat;
}
