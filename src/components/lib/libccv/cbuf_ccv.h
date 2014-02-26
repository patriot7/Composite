#ifndef CBUF_CCV_H
#define CBUF_CCV_H

#include <cbuf.h>
#include "ccv.h"

typedef struct {
	cbufp_t cbid;
	int size; /* header + data */ 
	int type;
	/*uint64_t sig;*/
	/*int refcount;*/
	int rows;
	int cols;
	int step;
	char data[0];
} cbuf_matrix_t;

typedef void *(*ccv_alloc_fn_t)(unsigned int sz);
typedef void *(*ccv_free_fn_t)(void *ptr);

void cbuf_ccv_alloc_creator(ccv_alloc_fn_t *fn);
void cbuf_ccv_free_creator(ccv_free_fn_t *fn);
void set_cbuf_header(ccv_dense_matrix_t *mat);

cbuf_matrix_t *ccv2cbufmat(ccv_dense_matrix_t *mat);
ccv_dense_matrix_t *cbuf2ccvmat(cbuf_matrix_t *cbuf_mat);


#endif
