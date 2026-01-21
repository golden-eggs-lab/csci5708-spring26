//
// Created by Chang Ge on 1/19/26.
//

#include "../include/csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


static void chomp(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) { s[n-1] = 0; n--; }
}

static int split_commas(char *line, char **out, int max_fields) {
    int k = 0;
    char *p = line;
    while (p && *p && k < max_fields) {
        char *comma = strchr(p, ',');
        if (comma) { *comma = 0; out[k++] = p; p = comma + 1; }
        else { out[k++] = p; break; }
    }
    return k;
}

Table *load_csv(const char *path, const Schema *schema) {
    assert(path && schema);
    FILE *f = fopen(path, "r");
    if (!f) { perror("fopen"); exit(1); }

    char buf[4096];

    // header
    if (!fgets(buf, sizeof(buf), f)) { fprintf(stderr, "Empty CSV: %s\n", path); exit(1); }
    chomp(buf);

    char *fields[256];
    int nf = split_commas(buf, fields, 256);
    if (nf != schema->ncols) {
        fprintf(stderr, "Header col count mismatch in %s\n", path);
        exit(1);
    }
    for (int i = 0; i < schema->ncols; i++) {
        if (strcmp(fields[i], schema->col_names[i]) != 0) {
            fprintf(stderr, "Header name mismatch in %s at col %d: got '%s' expected '%s'\n",
                    path, i, fields[i], schema->col_names[i]);
            exit(1);
        }
    }

    Table *t = table_create(schema, 64);

    // row
    while (fgets(buf, sizeof(buf), f)) {
        chomp(buf);
        if (buf[0] == 0) continue;

        char *cols[256];
        int nc = split_commas(buf, cols, 256);
        if (nc != schema->ncols) {
            fprintf(stderr, "Row col count mismatch in %s\n", path);
            exit(1);
        }

        Value row[256];
        for (int c = 0; c < schema->ncols; c++) {
            if (schema->types[c] == T_INT) {
                row[c] = value_make_int((int)strtol(cols[c], NULL, 10));
            } else {
                row[c] = value_make_string(cols[c]);
            }
        }
        table_append_row(t, row);

        for (int c = 0; c < schema->ncols; c++) value_free(&row[c]);
    }

    fclose(f);
    return t;
}
