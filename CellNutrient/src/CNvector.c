#include "CNvector.h"

#include <stdlib.h>
#include <stdio.h>

/*#ifdef DEBUG
void debug(vector_t *v)
{
  putchar('|');
  for (int i = 0; i < v->size; i++) {
    if (i >= v->off_begin && i < v->off_end) {
      if (i == v->off_begin) {
	putchar(v->base[i] - 32);
      } else {
	putchar(v->base[i]);
      }
    } else {
      if (i == v->off_end) {
	putchar('*');
      } else {
	putchar('_');
      }
    }
  }
  putchar('|');
  putchar('\n');
}
#else
void debug(vector_t *v){}
#endif

typedef struct celda {
  double nutr;
  double signal;
  } celda_t; */

vector_t *vector_create(int size)
{
  void **base = (void **)malloc(size * sizeof(void *));
  vector_t *v = (vector_t *)malloc(sizeof(vector_t));
  v->size = size;
  v->base = base;
  v->off_begin = size/2;
  v->off_end = v->off_begin;
  return v;
}

void vector_destroy(vector_t *v, int free_contents)
{
  if (v != NULL) {
    if (free_contents) {
      void **base = v->base;
      for (int i = v->off_begin; i < v->off_end; i++) {
	free(base[i]);
      }
    }
    free(v->base);
    free(v);
  }
}

int vector_len(vector_t *v)
{
  if (v != NULL) {
    return v->off_end - v->off_begin;
  } else {
    return -1;
  }
}

void vector_fill(vector_t *v, void *e)
{
  if (v != NULL) {
    for (int i = v->off_begin; i < v->off_end; i++) {
      v->base[i] = e;
    }
  }
}

void *vector_get(vector_t *v, int idx)
{
  if (v != NULL && v->off_begin + idx < v->off_end) {
    return v->base[v->off_begin + idx];
  } else {
    return NULL;
  }
}

void vector_set(vector_t *v, int idx, void *e)
{
  if (v != NULL && v->off_begin + idx < v->off_end) {
    v->base[v->off_begin + idx] = e;
  }
}

void vector_append(vector_t *v, void *e)
{
  if (v != NULL) {
    if (v->off_end == v->size) {
      v->size *= 2;
      v->base = (void **)realloc(v->base,
				 v->size * sizeof(void *));
    }
    v->base[v->off_end] = e;
    v->off_end++;
    //debug(v);
  }
}

void vector_prepend(vector_t *v, void *e)
{
  if (v != NULL) {
    if (v->off_begin == 0) {
      int offset = v->size;
      v->size *= 2;
      v->base = (void **)realloc(v->base,
				 v->size * sizeof(void *));
      for (int i = v->off_begin; i < v->off_end; i++) {
	v->base[i + offset] = v->base[i];
      }
      v->off_begin += offset;
      v->off_end += offset;
    }
    v->off_begin--;
    v->base[v->off_begin] = e;
    //debug(v);
  }
}

/*int main(void)
{
  int dim = 5;
  vector_t *vglob = vector_create(dim);
  for (int i = 0; i < dim; i++) {
    vector_t *vrow = vector_create(dim);
    for (int j = 0; j < dim; j++) {
      celda_t *celda = (celda_t *)malloc(sizeof(celda_t));
      celda->nutr = i*dim+j;
      celda->signal = 0;
      vector_append(vrow, celda);
    }
    vector_append(vglob, vrow);
  }
  for (int i = 0; i < dim; i++) {
    vector_t *vrow = vector_get(vglob, i);
    for (int j = 0; j < dim; j++) {
      celda_t *celda = vector_get(vrow, j);
      printf("%f ", celda->nutr);
    }
    putchar('\n');
    vector_destroy(vrow, 1);
  }
  vector_destroy(vglob, 0);
  return 0;
} */
