# CSCI 5708 — HW3: Query Operators and Planning

**Due:** April 9 @ 11:59 PM CT

**Submission:** Upload a single file named `submission.json` to Gradescope.  

- **All correct → 5 pts**
- **Partially correct (at least one correct selected AND no wrong selections) → 3 pts**
- **Any wrong selection or blank → 0 pts**

---

## Part 1 — External Sorting

### Q1

Consider a **2-way external merge sort** (simple merge sort). The data file has $N$ pages. Which of the following statements are **correct**?

- **(A)** Pass 0 reads each page, sorts it in memory, and writes it back, producing $N$ sorted runs of 1 page each.
- **(B)** Each subsequent pass merges pairs of runs, requiring 2 input buffer pages and 1 output buffer page (3 pages total).
- **(C)** The total number of passes is $1 + \lceil \log_2 N \rceil$.
- **(D)** Increasing the buffer pool size from 3 pages to 100 pages significantly reduces the number of passes in a 2-way merge sort because each pass can merge more runs.

---

### Q2

In the **general (multi-way) external merge sort** with $B$ buffer pool pages and a data file of $N$ pages:

- **(A)** Pass 0 produces $\lceil N/B \rceil$ sorted runs, each of size $B$ pages (except possibly the last run).
- **(B)** The total I/O cost is $N \times (\text{number of passes})$ because pages only need to be read (not written) during each pass.
- **(C)** Each subsequent pass performs a $(B-1)$-way merge, using $B-1$ input buffers and 1 output buffer.
- **(D)** The total number of passes is $1 + \lceil \log_{B-1} \lceil N/B \rceil \rceil$.

---

### Q3

A data file has $N = 108$ pages and the buffer pool has $B = 5$ pages. Using **general external merge sort**, which of the following are **correct**?

- **(A)** Pass 0 produces $\lceil 108/4 \rceil = 27$ sorted runs (reserving 1 buffer for output).
- **(B)** The number of passes is $1 + \lceil \log_4 22 \rceil = 1 + 3 = 4$.
- **(C)** The total I/O cost is $2 \times 108 \times 4 = 864$ page I/Os.
- **(D)** Pass 0 produces $\lceil 108/5 \rceil = 22$ sorted runs, because all $B = 5$ pages are used for sorting in Pass 0.

---

### Q4

**Double buffering** is an optimization for external merge sort. Which of the following are **correct**?

- **(A)** Double buffering prefetches the next block of a run in the background while the current block is being processed, overlapping CPU and I/O.
- **(B)** Double buffering reduces the total number of I/O operations compared to standard external merge sort.
- **(C)** Double buffering increases the effective merge fan-in because each input stream can process data faster with prefetching.
- **(D)** Double buffering effectively halves the number of available buffer pages for merging, since each input stream needs two buffers (one active, one prefetching).

---

## Part 2 — Join Algorithms

Use the following table statistics for Q5–Q10 unless stated otherwise:

| | Sailors ($S$) | Reserves ($R$) |
|---|---|---|
| #pages | $N = 500$ | $M = 1{,}000$ |
| #tuples/page | $p_S = 80$ | $p_R = 100$ |
| #tuples | 40,000 | 100,000 |

Assume the buffer pool has $B = 102$ pages. Ignore the cost of writing the final join result.

### Q5

For a **Simple (Tuple) Nested Loop Join** with $R$ as the outer relation and $S$ as the inner relation, which of the following are **correct**?

- **(A)** The I/O cost is $M + p_R \times M \times N = 1{,}000 + 100 \times 1{,}000 \times 500 = 50{,}001{,}000$.
- **(B)** Switching the outer to $S$ (the smaller relation) reduces cost to $N + p_S \times N \times M = 500 + 80 \times 500 \times 1{,}000 = 40{,}000{,}500$.
- **(C)** Simple nested loop join always performs better than page-oriented nested loop join because it has finer-grained tuple-level processing.
- **(D)** The buffer pool size $B$ has a significant impact on the cost of simple nested loop join — doubling $B$ roughly halves the cost.

---

### Q6

For a **Page-Oriented Nested Loop Join** and **Block Nested Loop Join**, which of the following are **correct**?

- **(A)** Page-oriented NLJ is always faster than block NLJ because it avoids the overhead of managing multiple buffer pages for the outer relation.
- **(B)** With page-oriented NLJ and $R$ as outer: cost = $M + M \times N = 1{,}000 + 1{,}000 \times 500 = 501{,}000$ I/Os.
- **(C)** With block NLJ ($R$ as outer, $B = 102$): cost = $M + \lceil M/(B-2) \rceil \times N = 1{,}000 + 10 \times 500 = 6{,}000$ I/Os.
- **(D)** For block NLJ, choosing the **smaller** relation as the outer generally gives lower cost because it results in fewer scans of the inner relation.

---

### Q7

For a **Block Nested Loop Join** with $B = 102$ pages, which of the following are **correct**?

- **(A)** With $S$ as outer and $R$ as inner: cost = $N + \lceil N/(B-2) \rceil \times M = 500 + 5 \times 1{,}000 = 5{,}500$ I/Os.
- **(B)** If $B$ were increased so that $B - 2 \geq M$ (the entire outer fits in one block), the inner relation is scanned only once, giving cost $M + N$.
- **(C)** Increasing the buffer pool size always reduces the cost of block nested loop join, with no diminishing returns.
- **(D)** Block NLJ with $R$ as outer and $S$ as inner costs $M + \lceil M/(B-2) \rceil \times N = 1{,}000 + 10 \times 500 = 6{,}000$ I/Os.

---

### Q8

For an **Index Nested Loop Join**, which of the following are **correct**?

- **(A)** With $R$ as outer and a hash index (Alt. 2) on `sid` of $S$, the cost is approximately $M + |R| \times (1.2 + 1) = 1{,}000 + 100{,}000 \times 2.2 = 221{,}000$ I/Os.
- **(B)** An index nested loop join requires indexes on **both** the inner and outer relations to function correctly.
- **(C)** Whether the index on the inner relation is clustered or unclustered does not matter when each outer tuple matches exactly one inner tuple (the retrieval cost is 1 I/O either way).
- **(D)** Index nested loop join is always the fastest join algorithm because index lookups are $O(1)$.

---

### Q9

For a **Sort-Merge Join** on attribute `sid`, where sorting each relation requires 2 passes, which of the following are **correct**?

- **(A)** The cost of sorting $R$ is $2 \times 2 \times M = 4{,}000$ I/Os, and sorting $S$ is $2 \times 2 \times N = 2{,}000$ I/Os.
- **(B)** The merge phase costs $M + N = 1{,}500$ I/Os (assuming no duplicates in the join key).
- **(C)** The total cost is $4{,}000 + 2{,}000 + 1{,}500 = 7{,}500$ I/Os.
- **(D)** An advantage of sort-merge join over hash join is that the output is already sorted on the join key, which can benefit subsequent operators (e.g., ORDER BY, GROUP BY).

---

### Q10

For a **Hash Join**, which of the following are **correct**?

- **(A)** The total cost is $2(M + N) = 3{,}000$ I/Os, because the matching phase is free after partitioning.
- **(B)** The partitioning phase reads and writes both relations: $2(M + N) = 3{,}000$ I/Os, and the matching phase reads both: $M + N = 1{,}500$ I/Os, totaling $3(M+N) = 4{,}500$.
- **(C)** Hash join is always superior to sort-merge join, regardless of data distribution, because hashing is $O(1)$ per tuple.
- **(D)** If many keys hash to the same bucket (skew), the DBMS can apply **recursive partitioning** using a different hash function $h_2 \neq h_1$ on the overflowing partition.

---

## Part 3 — Query Plan Structure & Relational Algebra Transformations

### Q11

Regarding **query plans** and **operator evaluation** approaches, which of the following are **correct**?

- **(A)** A query plan is a tree structure where operators are nodes and data flows along the edges from leaves (table scans) to the root (final result).
- **(B)** In the relational model, the input and output of every operator are relations, so operators can be freely composed (i.e., the output of one operator can be the input to another).
- **(C)** The three shared approaches for operator evaluation are **indexing** (examine only tuples satisfying a condition), **iteration** (always applicable), and **partitioning** (using sorting or hashing).
- **(D)** Sorting is only needed for explicit `ORDER BY` queries; it is never used internally by other operators such as joins, `GROUP BY`, or duplicate elimination.

---

### Q12

Regarding **relational algebra equivalence** and **plan transformations**, which of the following are **correct**?

- **(A)** Two query plans are semantically equivalent if they produce the same output for the same input, even though they may use different operator orderings.
- **(B)** Selection predicates can be decomposed and pushed down independently — e.g., $\sigma_{p_1 \wedge p_2}(R) = \sigma_{p_1}(\sigma_{p_2}(R))$.
- **(C)** Predicate pushdown rewrites a plan so that selections are applied as early as possible (closer to the leaves), reducing the amount of data flowing through intermediate operators.
- **(D)** Predicate rewriting can simplify conditions — for example, given `course1 = 5708 AND course1 = course2`, the optimizer can infer `course1 = 5708 AND course2 = 5708`.

---

### Q13

Consider the query: `SELECT i.name FROM Instructors i, Teach t, Courses c WHERE i.id = t.i_id AND t.c_id = c.id AND c.name = 'Databases';`

The optimizer transforms this query step by step. Which of the following are **correct**?

- **(A)** The initial (naïve) plan uses Cartesian products ($\times$) between all three tables, followed by a single combined selection and projection at the top.
- **(B)** After decomposing predicates and pushing them down, the Cartesian products ($\times$) can be converted into equi-joins ($\bowtie$), dramatically reducing intermediate result sizes.
- **(C)** Projection pushdown adds $\pi$ operators below the joins so that only the columns needed by upper operators are carried through (e.g., dropping unnecessary columns from `Instructors` and `Courses` early).
- **(D)** After all transformations, the selection `c.name = 'Databases'` remains at the top of the plan tree because selections cannot be pushed below joins.

---

## Part 4 — Query Planning & Optimization

### Q14

Regarding the **query optimizer architecture**, which of the following are **correct**?

- **(A)** A **logical plan** specifies *what* to compute using relational algebra operators (select, project, join), without specifying physical algorithms or access methods.
- **(B)** A **physical plan** specifies *how* to compute, including the choice of algorithm (e.g., hash join vs. merge join) and access path (e.g., sequential scan vs. index scan).
- **(C)** The query optimizer always finds the **globally optimal** plan because modern cost models are perfectly accurate.
- **(D)** Join ordering is an **NP-hard** problem, so practical optimizers use heuristics and dynamic programming to search a subset of the plan space.

---

### Q15

Regarding **plan transformations** (logical rewrites), which of the following are **correct**?

- **(A)** **Predicate pushdown** moves selection ($\sigma$) operators closer to the leaf (table scan) nodes so that fewer tuples flow through the plan.
- **(B)** Predicate and projection pushdown **always** improve query performance, with no exceptions.
- **(C)** **Projection pushdown** is unnecessary because projecting out columns early never reduces the amount of data flowing through intermediate operators.
- **(D)** Converting a Cartesian product ($\times$) followed by a selection on a join condition into an equi-join ($\bowtie$) is a standard transformation that can dramatically reduce cost.

---

### Q16

Regarding the **difficulty of query optimization** and why it is considered one of the hardest parts of a DBMS, which of the following are **correct**?

- **(A)** The search space is enormous: join order alone grows factorially with the number of tables (e.g., 10 tables yield ~3.6 million possible join orderings), and each join can use different algorithms and access paths.
- **(B)** Cost estimation is inherently inaccurate because optimizers rely on statistics (not exact data) and make simplifying assumptions such as attribute independence and uniform distributions.
- **(C)** Cascading errors are a major concern: a wrong cardinality estimate can lead to a wrong join order, which leads to a wrong operator choice, potentially causing orders-of-magnitude slowdowns.
- **(D)** The best query plan is independent of physical design choices (indexes, partitioning, materialized views) and runtime system states (buffer pool warmth, storage type), so the optimizer can ignore these factors.

---

### Q17

Regarding **left-deep plans** and **join order search**, which of the following are **correct**?

- **(A)** Bushy plans (where both children of a join can be intermediate results) are never useful because left-deep plans are always optimal.
- **(B)** In a left-deep plan tree, the right child of every join operator is always a base relation (table scan), and joins are composed only on the left side.
- **(C)** Restricting the search to left-deep plans reduces the search space while still allowing fully pipelined execution without intermediate materialization.
- **(D)** For $n$ tables, the number of possible left-deep join orders is $2^n$, which is smaller than the number of bushy trees.

---

## Part 5 — Cost Estimation & Statistics

### Q18

The DBMS uses **statistics** stored in the system catalog to estimate query costs. Which of the following are **correct**?

- **(A)** Statistics are maintained at multiple granularities: table-level (number of rows, number of pages), column-level (number of distinct values, number of nulls, histograms), and index-level (tree height, clustering).
- **(B)** System catalogs are updated only when the DBA manually runs `ANALYZE`; there is no automatic background mechanism to keep statistics current.
- **(C)** Cost models across different DBMSs vary, but most rely on statistics maintained in system catalogs to predict the runtime cost of alternative query plans, choosing the one with the lowest predicted cost.
- **(D)** Extended (multi-column) statistics can capture dependencies between columns, which is important because the standard assumptions of attribute independence and uniform distributions often do not hold in practice.

---

### Q19

Regarding **histograms and sketches** for cardinality estimation, which of the following are **correct**?

- **(A)** **Histograms** are a counting-based summarization technique used as column-level statistics to help the optimizer estimate how values are distributed.
- **(B)** A **Count-Min Sketch** is a probabilistic data structure that uses a set of hash functions and a 2D array of counters. To **insert** a value, it hashes the value with each hash function and increments the corresponding counter in each row.
- **(C)** To **count** (query) a value's frequency in a Count-Min Sketch, the DBMS hashes the value with each hash function, looks up the corresponding counter in each row, and returns the **maximum** of those counters.
- **(D)** Count-Min Sketches offer a theoretical guarantee on the space-accuracy tradeoff: they may **overestimate** the true frequency (due to hash collisions) but never **underestimate** it.

---

### Q20

Regarding the **interaction between cost estimation, statistics, and query optimization**, which of the following are **correct**?

- **(A)** The optimizer relies on statistics rather than scanning the actual data, so cost estimates are approximations — real data often has correlations and skew that violate the simplifying assumptions.
- **(B)** Cascading estimation errors are dangerous: if the optimizer underestimates the size of an intermediate result, it may choose a nested loop join when a hash join would have been far cheaper, leading to orders-of-magnitude slowdowns.
- **(C)** The cost of a query plan depends on multiple dimensions — CPU, I/O, memory, and potentially network — and no single metric is always the best measure. First-response time and total query time can favor different plans.
- **(D)** Since the optimizer already uses statistics from the system catalog, running `ANALYZE` (or relying on auto vacuum) to refresh statistics has no impact on plan quality — the optimizer will choose the same plan regardless of how stale the statistics are.

---

## Submission Instructions

1. Copy `submission_template.json` and rename it to `submission.json`.
2. For each question, fill in your selected option(s) as a list. Examples:
   - Single answer: `["A"]`
   - Multiple answers: `["A", "C"]`
3. Use **uppercase** letters only.
4. Upload `submission.json` to Gradescope.
