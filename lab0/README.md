
# CSCI5708 — Lab 0: Relational Operator Pipeline in C 

## Due Date

**Sunday, Jan 25 @ 11:59 PM CT**

---

## 1. Overview

CSCI5708 studies **how DBMSs are implemented** (we will use PostgreSQL later in the semester). This lab is a short “readiness check” to confirm that you have:

1. **Database fundamentals from CSCI4707** (selection, projection, join)
2. **C programming maturity** (memory safety, pointers, struct-based programming)

You will implement a small relational query pipeline in C using **three operators** in `operators.c``:

* `filter` (selection)
* `project` (projection)
* `nested_loop_join` (inner equijoin)

Plus, the logic in `main.c`

There is **NO SQL parsing** in this lab. The starter code is provided as followws

---

## 2. Starter Code Provided

We provide a starter repository that includes:

### Provided infrastructure (do not rewrite)

* `Schema`, `Table`, and `Value` data structures
* CSV loading utilities:

    * `load_csv(...)`
    * `table_free(...)`
* helper functions for copying/freeing values
* unit tests (autograder)

### Files you will modify

* `src/operators.c`
* `src/main.c`

---

## 3. The Data Model

Tables are stored in memory as rows of typed values.

Supported types:

* `INT`
* `STRING`

Null values are **not required** for Lab 0A.

---

## 4. What You Must Implement

### 4.1 Filter (Selection)

**File:** `src/operators.c`
**Function:**

```c
Table *filter(const Table *in, PredicateFn pred, void *ctx);
```

**Semantics:**
Return a new table containing only rows from `in` where the predicate evaluates to true.

**Requirements**

* output schema must match input schema
* output row order must preserve input order
* output must be a deep copy (no pointers into input table)

---

### 4.2 Project (Projection)

**File:** `src/operators.c`
**Function:**

```c
Table *project(const Table *in, const int *col_idx, int k);
```

**Semantics:**
Return a new table keeping only the columns listed in `col_idx` (length `k`), in that order.

**Requirements**

* output schema contains only the projected columns
* preserve row order
* deep copy values

---

### 4.3 Simple Nested-Loop Inner Join (Equijoin)

**File:** `src/operators.c`
**Function:**

```c
Table *nested_loop_join(const Table *L, const Table *R, int keyL, int keyR);
```

**Semantics:**
Return an **inner join** of `L` and `R` where:

```
L[keyL] == R[keyR]
```

**Requirements**

* inner join only
* equality predicate only
* output schema must be: `L.*` then `R.*`
* must support duplicates and produce correct multiplicity
* output order requirement:

    * for each row in `L` in order, scan `R` in order and output matching pairs

---

## 5. Fixed Operator Pipeline (No SQL)

In `src/main.c`, you must run this query pipeline:

> Find students who took **CSCI4707** and scored **>= 90**, output their **name** and **major**.

We provide CSV files under `data/`:

* `students.csv(sid:int, name:string, major_id:int)`
* `enroll.csv(sid:int, course:string, score:int)`
* `majors.csv(major_id:int, major:string)`

Your program must:

1. Load the three CSVs using `load_csv`
2. Filter enrollments to:

    * `course == "CSCI4707"` AND `score >= 90`
3. Join students with filtered enrollments on `sid`
4. Join the result with majors on `major_id`
5. Project output columns: `(name, major)`
6. Print results with the required format below

---

## 6. Required Output Format

Your program must print:

1. A header line:

```
name,major
```

2. Up to **10 rows** of output data, formatted as CSV:

```
Alice,Computer Science
Bob,Mechanical Engineering
...
```

3. A final summary line:

```
[rows]=<N>
```

Example:

```
name,major
Alice,Computer Science
Bob,Mechanical Engineering
[rows]=2
```

Note: the autograder checks output formatting.

---

## 7. Build & Test

### Build

```bash
cmake -S . -B build
cmake --build build
```

or build inside CLion using the CMake targets.

### Run

```bash
./build/lab0
```

### Run unit tests

```bash
./build/test_lab0
```

(Or use CLion to run the `test_lab0` target.)

---

## 8. Compiler Requirements (Strict)

All submissions must compile under the **C11** standard.

We will enforce strict compiler warnings:

* `-Wall -Wextra -Werror -pedantic`

Warnings are treated as errors.

Your code must be memory-safe (no leaks, no invalid access).

---

## 9. Rules & Constraints

* This is an **individual assignment**, failure to submit will remove enrollment
* Do not use external libraries beyond the C standard library

* Do not hardcode answers based on the specific dataset contents, the autograder on Gradescope will use a different dataset


---

## 10. Submission

Submit the following files to Gradescope:

* `operators.c`
* `main.c`

Do **not** submit anything else.

---

## 11. Tips / Common Mistakes
* Make sure your join handles duplicate keys correctly
* Make sure you deep-copy strings
* Free all intermediate tables created in `main.c`
---

