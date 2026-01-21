//
// Created by Chang Ge on 1/19/26.
//

#ifndef LAB0_OPERATORS_H
#define LAB0_OPERATORS_H

#endif //LAB0_OPERATORS_H


#pragma once
#include <stdbool.h>
#include "db.h"

typedef bool (*PredicateFn)(const Value *row, const Schema *schema, void *ctx);


// the following 3 functions needs to be implemented in the .c file
Table *filter(const Table *in, PredicateFn pred, void *ctx);

Table *project(const Table *in, const int *col_idx, int k);

Table *nested_loop_join(const Table *L, const Table *R, int keyL, int keyR);
