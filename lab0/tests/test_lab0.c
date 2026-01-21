//
// Created by Chang Ge on 1/19/26.
//

#include "../include/db.h"
#include "../include/operators.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define ASSERT_TRUE(x) do { if (!(x)) { \
  fprintf(stderr, "ASSERT_TRUE failed at %s:%d\n", __FILE__, __LINE__); \
  exit(1); } } while (0)

static bool pred_int_ge(const Value *row, const Schema *schema, void *ctx) {
  (void)schema;
  int col = *(int*)ctx;
  return row[col].type == T_INT && row[col].v.i >= 5;
}

static Table *mk_table_1col_int(const char *colname, int *vals, int n) {
  Schema s = schema_create(1);
  schema_set_col(&s, 0, colname, T_INT);
  Table *t = table_create(&s, n);
  for (int i = 0; i < n; i++) {
    Value row[1] = { value_make_int(vals[i]) };
    table_append_row(t, row);
  }
  schema_free(&s);
  return t;
}

static void test_filter_basic(void) {
  int vals[] = {1, 5, 6, 2, 10};
  Table *t = mk_table_1col_int("x", vals, 5);
  int col = 0;
  Table *f = filter(t, pred_int_ge, &col);
  ASSERT_TRUE(f != NULL);
  ASSERT_TRUE(f->nrows == 3);
  ASSERT_TRUE(f->rows[0][0].v.i == 5);
  ASSERT_TRUE(f->rows[1][0].v.i == 6);
  ASSERT_TRUE(f->rows[2][0].v.i == 10);
  table_free(f);
  table_free(t);
}

static void test_project_basic(void) {
  Schema s = schema_create(2);
  schema_set_col(&s, 0, "a", T_INT);
  schema_set_col(&s, 1, "b", T_STRING);
  Table *t = table_create(&s, 2);

  Value r0[2] = { value_make_int(7), value_make_string("hi") };
  Value r1[2] = { value_make_int(9), value_make_string("yo") };
  table_append_row(t, r0);
  table_append_row(t, r1);
  value_free(&r0[1]); value_free(&r1[1]);

  int cols[] = {1};
  Table *p = project(t, cols, 1);
  ASSERT_TRUE(p != NULL);
  ASSERT_TRUE(p->schema.ncols == 1);
  ASSERT_TRUE(strcmp(p->schema.col_names[0], "b") == 0);
  ASSERT_TRUE(p->rows[0][0].type == T_STRING);
  ASSERT_TRUE(strcmp(p->rows[0][0].v.s, "hi") == 0);

  table_free(p);
  table_free(t);
  schema_free(&s);
}

static void test_join_multiplicity_and_order(void) {
  // L has keys: 1,1 ; R has keys: 1,1,2 => output should have 2*2 = 4 matches for key=1 in left order
  Schema sl = schema_create(1);
  schema_set_col(&sl, 0, "k", T_INT);
  Table *L = table_create(&sl, 2);

  Value l0[1] = { value_make_int(1) };
  Value l1[1] = { value_make_int(1) };
  table_append_row(L, l0);
  table_append_row(L, l1);

  Schema sr = schema_create(1);
  schema_set_col(&sr, 0, "k2", T_INT);
  Table *R = table_create(&sr, 3);
  Value r0[1] = { value_make_int(1) };
  Value r1[1] = { value_make_int(1) };
  Value r2[1] = { value_make_int(2) };
  table_append_row(R, r0);
  table_append_row(R, r1);
  table_append_row(R, r2);

  Table *J = nested_loop_join(L, R, 0, 0);
  ASSERT_TRUE(J != NULL);
  ASSERT_TRUE(J->schema.ncols == 2);
  ASSERT_TRUE(J->nrows == 4);

  // Order: for each L row in order, scan R in order
  // So first two rows correspond to L[0] joined with R[0],R[1]
  ASSERT_TRUE(J->rows[0][0].v.i == 1 && J->rows[0][1].v.i == 1);
  ASSERT_TRUE(J->rows[1][0].v.i == 1 && J->rows[1][1].v.i == 1);
  // Next two rows correspond to L[1] joined with R[0],R[1]
  ASSERT_TRUE(J->rows[2][0].v.i == 1 && J->rows[2][1].v.i == 1);
  ASSERT_TRUE(J->rows[3][0].v.i == 1 && J->rows[3][1].v.i == 1);

  table_free(J);
  table_free(L);
  table_free(R);
  schema_free(&sl);
  schema_free(&sr);
}

int main(void) {
  test_filter_basic();
  test_project_basic();
  test_join_multiplicity_and_order();
  printf("All Lab0 unit tests passed.\n");
  return 0;
}
