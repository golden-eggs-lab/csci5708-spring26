//
// Created by Chang Ge on 1/19/26.
//

#include "../include/db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

Value value_make_int(int x) {
  Value v; v.type = T_INT; v.v.i = x; return v;
}

Value value_make_string(const char *s) {
  Value v; v.type = T_STRING;
  size_t n = strlen(s);
  v.v.s = (char*)malloc(n + 1);
  if (!v.v.s) { perror("malloc"); exit(1); }
  memcpy(v.v.s, s, n + 1);
  return v;
}

Value value_copy(const Value *in) {
  assert(in);
  if (in->type == T_INT) return value_make_int(in->v.i);
  return value_make_string(in->v.s ? in->v.s : "");
}

void value_free(Value *v) {
  if (!v) return;
  if (v->type == T_STRING) {
    free(v->v.s);
    v->v.s = NULL;
  }
}

bool value_eq(const Value *a, const Value *b) {
  assert(a && b);
  if (a->type != b->type) return false;
  if (a->type == T_INT) return a->v.i == b->v.i;
  return strcmp(a->v.s, b->v.s) == 0;
}

Schema schema_create(int ncols) {
  Schema s;
  s.ncols = ncols;
  s.types = (ColType*)calloc((size_t)ncols, sizeof(ColType));
  s.col_names = (char**)calloc((size_t)ncols, sizeof(char*));
  if (!s.types || !s.col_names) { perror("calloc"); exit(1); }
  return s;
}

void schema_set_col(Schema *s, int idx, const char *name, ColType t) {
  assert(s && idx >= 0 && idx < s->ncols);
  s->types[idx] = t;
  free(s->col_names[idx]);
  s->col_names[idx] = NULL;
  size_t n = strlen(name);
  s->col_names[idx] = (char*)malloc(n + 1);
  if (!s->col_names[idx]) { perror("malloc"); exit(1); }
  memcpy(s->col_names[idx], name, n + 1);
}

void schema_free(Schema *s) {
  if (!s) return;
  for (int i = 0; i < s->ncols; i++) free(s->col_names[i]);
  free(s->col_names);
  free(s->types);
  s->col_names = NULL;
  s->types = NULL;
  s->ncols = 0;
}

int schema_col_index(const Schema *s, const char *name) {
  assert(s && name);
  for (int i = 0; i < s->ncols; i++) {
    if (s->col_names[i] && strcmp(s->col_names[i], name) == 0) return i;
  }
  return -1;
}

Table *table_create(const Schema *schema, int nrows_hint) {
  (void)nrows_hint;
  assert(schema);
  Table *t = (Table*)calloc(1, sizeof(Table));
  if (!t) { perror("calloc"); exit(1); }

  t->schema = schema_create(schema->ncols);
  for (int i = 0; i < schema->ncols; i++) {
    schema_set_col(&t->schema, i, schema->col_names[i], schema->types[i]);
  }
  t->nrows = 0;
  t->rows = NULL;
  return t;
}

static void table_grow(Table *t, int new_rows) {
  assert(t);
  Value **nr = (Value**)realloc(t->rows, (size_t)new_rows * sizeof(Value*));
  if (!nr) { perror("realloc"); exit(1); }
  t->rows = nr;
}

void table_append_row(Table *t, const Value *row_values) {
  assert(t && row_values);
  int ncols = t->schema.ncols;
  table_grow(t, t->nrows + 1);

  Value *row = (Value*)calloc((size_t)ncols, sizeof(Value));
  if (!row) { perror("calloc"); exit(1); }

  for (int c = 0; c < ncols; c++) row[c] = value_copy(&row_values[c]);
  t->rows[t->nrows] = row;
  t->nrows += 1;
}

void table_free(Table *t) {
  if (!t) return;
  for (int r = 0; r < t->nrows; r++) {
    Value *row = t->rows[r];
    for (int c = 0; c < t->schema.ncols; c++) value_free(&row[c]);
    free(row);
  }
  free(t->rows);
  schema_free(&t->schema);
  free(t);
}

void table_print_csv(const Table *t, int max_rows) {
  if (!t) return;
  // header
  for (int c = 0; c < t->schema.ncols; c++) {
    printf("%s%s", t->schema.col_names[c], (c + 1 == t->schema.ncols) ? "\n" : ",");
  }
  int n = t->nrows < max_rows ? t->nrows : max_rows;
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < t->schema.ncols; c++) {
      const Value *v = &t->rows[r][c];
      if (v->type == T_INT) printf("%d", v->v.i);
      else printf("%s", v->v.s);
      printf("%s", (c + 1 == t->schema.ncols) ? "\n" : ",");
    }
  }
}
