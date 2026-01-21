//
// Created by Chang Ge on 1/19/26.
//

#ifndef LAB0_DB_H
#define LAB0_DB_H

#endif //LAB0_DB_H

#pragma once
#include <stdbool.h>

typedef enum { T_INT = 0, T_STRING = 1 } ColType;

typedef struct {
    ColType type;
    union {
        int i;
        char *s;
    } v;
} Value;

typedef struct {
    int ncols;
    ColType *types;
    char **col_names;
} Schema;

typedef struct {
    Schema schema;
    int nrows;
    Value **rows;
} Table;


// these are deep copy functions
Value value_make_int(int x);
Value value_make_string(const char *s);
Value value_copy(const Value *in);
void  value_free(Value *v);

bool  value_eq(const Value *a, const Value *b);

// the folllwing are schema related functions.
// note that table_free may be called at the end if intermediate tables are created
Schema schema_create(int ncols);
void   schema_set_col(Schema *s, int idx, const char *name, ColType t);
void   schema_free(Schema *s);

Table *table_create(const Schema *schema, int nrows_hint);
void   table_append_row(Table *t, const Value *row_values);
void   table_free(Table *t);

int    schema_col_index(const Schema *s, const char *name);

void table_print_csv(const Table *t, int max_rows);
