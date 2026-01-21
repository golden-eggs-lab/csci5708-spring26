//
// Created by Chang Ge on 1/19/26.
//

#include "../include/operators.h"
#include <assert.h>
#include <stdlib.h>

/* TODO:
 * - output schema == input schema
 * - preserve row order
 * - deep-copy rows into new table
 */
Table *filter(const Table *in, PredicateFn pred, void *ctx) {
    assert(in && pred);

    // your code here

    return NULL;
}

/* TODO:
 * - output schema contains selected columns in given order
 * - preserve row order
 * - deep-copy selected values
 */
Table *project(const Table *in, const int *col_idx, int k) {
    assert(in && col_idx && k > 0);

    // your code here

    return NULL;
}


/* TODO:
 * - inner equijoin only, use simple nested loop join
 * - output schema = L.* then R.*
 * - preserve left-to-right nesting order:
 * for each row in L in order, scan rows in R in order and output matches
 */
Table *nested_loop_join(const Table *L, const Table *R, int keyL, int keyR) {
    assert(L && R);
    assert(keyL >= 0 && keyL < L->schema.ncols);
    assert(keyR >= 0 && keyR < R->schema.ncols);

    // your code here

    return NULL;
}
