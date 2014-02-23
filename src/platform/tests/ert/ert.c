#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

typedef unsigned short int u16_t;
typedef unsigned int u32_t;
#define unlikely(x)     __builtin_expect(!!(x), 0)
#define LINUX_TEST
#include <kvtrie.h>

#define NTESTS (4096)

struct pair {
	long id;
	void *val;
};

int in_pairs(struct pair *ps, int len, long id)
{
	for (; len >= 0 ; len--) {
		if (ps[len].id == id) return 1;
	}
	return 0;
}

int alloc_cnt = 0;
#include <sys/mman.h>
static void *
unit_allocfn(void *d, int sz, int leaf) 
{ 
	int *mem = d;
	(void)d; (void)leaf;
	alloc_cnt++;
	if (mem) {
		assert(*mem == 0);
		*mem = 1;
	}
	return malloc(sz);
	//return mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static void 
unit_freefn(void *d, void *m, int sz, int leaf) 
{ 
	(void)d; (void)leaf; (void)sz;
	alloc_cnt--;
	free(m);
	//munmap(m, sz);
}

KVT_CREATE_DEF(unit,  2, 12, 8, unit_allocfn, unit_freefn);
KVT_CREATE_DEF(unit2, 3, 6, 12, unit_allocfn, unit_freefn);
KVT_CREATE_DEF(unit3, 4, 5, 5, unit_allocfn, unit_freefn);

ERT_CREATE_DEF(unit4, 3, 8, 4, 64, unit_allocfn, unit_freefn);
ERT_CREATE_DEF(unit5, 1, 0, 14, 32, unit_allocfn, unit_freefn);

/* 
 * I separate this out so that we can easily confirm that the compiler
 * is doing the proper optimizations. 
 */
void *
do_lookups(struct pair *ps, struct unit_ert *v)
{
	return unit_lkupp(v, ps->id);
}

void *
do_addr_lookups(struct pair *ps, struct unit4_ert *v)
{
	//if (unlikely((unsigned long)ps->id >= unit_maxid())) return NULL;
	int *p = unit4_lkup(v, ps->id);
	return (void*)*p;
}

int
do_add(struct pair *ps, struct unit_ert *v, void *d)
{
	return unit_add(v, ps->id, d);
}

typedef struct ert *(*alloc_fn_t)(void *memctxt);
typedef void (*free_fn_t)(struct ert *v);
typedef void *(*lkupp_fn_t)(struct ert *v, unsigned long id);
typedef int (*add_fn_t)(struct ert *v, long id, void *val);
typedef int (*del_fn_t)(struct ert *v, long id);
typedef void *(*lkup_fn_t)(struct ert *v, unsigned long id);
typedef void *(*lkupa_fn_t)(struct ert *v, unsigned long id, unsigned long *agg);
typedef void *(*lkupan_fn_t)(struct ert *v, unsigned long id, int dlimit, unsigned long *agg);
typedef int (*expandn_fn_t)(struct ert *v, unsigned long id, u32_t dlimit, void *memctxt);
typedef int (*expand_fn_t)(struct ert *v, unsigned long id, void * memctxt);

void 
kv_test(int max, alloc_fn_t a, free_fn_t f, lkupp_fn_t lp, add_fn_t add, del_fn_t d, 
	lkup_fn_t l, lkupa_fn_t la, lkupan_fn_t lan, expandn_fn_t en, expand_fn_t e)
{
	(void)l; (void)la; (void)lan; (void)en; (void)e;
	struct pair pairs[NTESTS];
	int i;
	struct ert *dyn_vect;

	dyn_vect = (struct ert *)a(NULL);
	assert(dyn_vect);
	for (i = 0 ; i < NTESTS ; i++) {
		do {
			pairs[i].id = rand() % max;
		} while (in_pairs(pairs, i-1, pairs[i].id));
		pairs[i].val = malloc(10);
		assert(!add(dyn_vect, pairs[i].id, pairs[i].val));
		assert(lp(dyn_vect, pairs[i].id) == pairs[i].val);
	}
	for (i = 0 ; i < NTESTS ; i++) {
		assert(lp(dyn_vect, pairs[i].id) == pairs[i].val);
	}
	for (i = 0 ; i < NTESTS ; i++) {
		free(pairs[i].val);
		assert(!d(dyn_vect, pairs[i].id));
		pairs[i].id  = 0;
		pairs[i].val = NULL;
	}
	f(dyn_vect);
}

void ert_test(int max, int depth, alloc_fn_t a, lkup_fn_t l, lkupa_fn_t la, lkupan_fn_t lan, expandn_fn_t en, expand_fn_t e)
{
	(void)l; (void)la; (void)lan; (void)e;
	struct pair pairs[NTESTS];
	int i;
	unsigned long mem;
	struct ert *v;
	int testnum = NTESTS;
	
	if (testnum > max) testnum = max;
	mem = 0;
	v = (struct ert *)a(&mem);
	assert(v);
	assert(mem);
	for (i = 0 ; i < testnum ; i++) {
		int j, c;
		unsigned long accum;
		int *val;

		do {
			pairs[i].id = rand() % max;
		} while (in_pairs(pairs, i-1, pairs[i].id));
		val = l(v, pairs[i].id);
		assert(!val || !*val);
		for (j = 2 ; j <= depth ; j++) {
			c   = alloc_cnt;
			mem = 0;
			assert(!en(v, pairs[i].id, j, &mem));
			assert(lan(v, pairs[i].id, j, &accum));
			if (j < depth && mem) {
				assert(!lan(v, pairs[i].id, j+1, &accum));
			}
			assert(alloc_cnt == (mem ? c+1 : c));
		}
		assert((val = l(v, pairs[i].id)));
		assert(*val == 0); /* init to NULL */
		*val = pairs[i].id;
		assert(l(v, pairs[i].id));
	}
}

int 
main(void)
{
	printf("key-value tests:\n");
	kv_test(unit_maxid(), (alloc_fn_t)unit_alloc, (free_fn_t)unit_free, (lkupp_fn_t)unit_lkupp,
		(add_fn_t)unit_add, (del_fn_t)unit_del, (lkup_fn_t)unit_lkup, (lkupa_fn_t)unit_lkupa,
		(lkupan_fn_t)unit_lkupan, (expandn_fn_t)unit_expandn, (expand_fn_t)unit_expand);
	kv_test(unit2_maxid(), (alloc_fn_t)unit2_alloc, (free_fn_t)unit2_free, (lkupp_fn_t)unit2_lkupp,
		(add_fn_t)unit2_add, (del_fn_t)unit2_del, (lkup_fn_t)unit2_lkup, (lkupa_fn_t)unit2_lkupa,
		(lkupan_fn_t)unit2_lkupan, (expandn_fn_t)unit2_expandn, (expand_fn_t)unit2_expand);
	kv_test(unit3_maxid(), (alloc_fn_t)unit3_alloc, (free_fn_t)unit3_free, (lkupp_fn_t)unit3_lkupp,
		(add_fn_t)unit3_add, (del_fn_t)unit3_del, (lkup_fn_t)unit3_lkup, (lkupa_fn_t)unit3_lkupa,
		(lkupan_fn_t)unit3_lkupan, (expandn_fn_t)unit3_expandn, (expand_fn_t)unit3_expand);
	assert(alloc_cnt == 0);
	printf("\tSUCCESS\n");
	
	printf("ert tests:\n");
	ert_test(unit4_maxid(), 3, (alloc_fn_t)unit4_alloc, (lkup_fn_t)unit4_lkup, (lkupa_fn_t)unit4_lkupa, 
		 (lkupan_fn_t)unit4_lkupan, (expandn_fn_t)unit4_expandn, (expand_fn_t)unit4_expand);
	ert_test(unit5_maxid(), 1, (alloc_fn_t)unit5_alloc, (lkup_fn_t)unit5_lkup, (lkupa_fn_t)unit5_lkupa, 
		 (lkupan_fn_t)unit5_lkupan, (expandn_fn_t)unit5_expandn, (expand_fn_t)unit5_expand);
	printf("\tSUCCESS\n");

	return 0;
}
