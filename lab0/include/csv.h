//
// Created by Chang Ge on 1/19/26.
//

#ifndef LAB0_CSV_H
#define LAB0_CSV_H

#endif //LAB0_CSV_H

#pragma once
#include "db.h"

// simple csv loader for int and string only
Table *load_csv(const char *path, const Schema *schema);
