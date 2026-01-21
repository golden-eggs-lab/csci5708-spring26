//
// Created by Chang Ge on 1/19/26.
//

#include "../include/db.h"
#include "../include/csv.h"
#include "../include/operators.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
  int col_course;
  int col_score;
  const char *course;
  int min_score;
} EnrollPredCtx;

static bool pred_course_and_score(const Value *row, const Schema *schema, void *ctx_) {
  (void)schema;
  EnrollPredCtx *ctx = (EnrollPredCtx*)ctx_;
  const Value *v_course = &row[ctx->col_course];
  const Value *v_score  = &row[ctx->col_score];
  if (v_course->type != T_STRING || v_score->type != T_INT) return false;
  return (strcmp(v_course->v.s, ctx->course) == 0) && (v_score->v.i >= ctx->min_score);
}

int main(void) {
  // Schemas must match CSV headers in data/*.csv
  Schema s_students = schema_create(3);
  schema_set_col(&s_students, 0, "sid", T_INT);
  schema_set_col(&s_students, 1, "name", T_STRING);
  schema_set_col(&s_students, 2, "major_id", T_INT);

  Schema s_enroll = schema_create(3);
  schema_set_col(&s_enroll, 0, "sid", T_INT);
  schema_set_col(&s_enroll, 1, "course", T_STRING);
  schema_set_col(&s_enroll, 2, "score", T_INT);

  Schema s_majors = schema_create(2);
  schema_set_col(&s_majors, 0, "major_id", T_INT);
  schema_set_col(&s_majors, 1, "major", T_STRING);

  Table *students = load_csv("data/students.csv", &s_students);
  Table *enroll   = load_csv("data/enroll.csv", &s_enroll);
  Table *majors   = load_csv("data/majors.csv", &s_majors);

  /*
   * TODO: implement the pipeline step by step, read it frmo the left-deep query plan tree
   * SLEECT S.name, M.major
   * FROM students S, major M, enroll E
   * WHERE S.sid=E.sid AND S.major_id=M.major_id AND E.course=‘CSCI4707’ AND E.score>=90;
   */

  // 1) select enroll where course == "CSCI4707" and score >= 90
  // your code here

  // 2) join students ⋈ filtered_enroll on sid
  // your code here

  // 3) join result ⋈ majors on major_id
  // your code here

  // 4) project (name, major)
  // your code here

  // 5) print CSV header "name,major", first 10 rows, and final line "[rows]=N"
  // NOTE: required output format:
  // name,major
  // Alice,Computer Science
  // ...
  // [rows]=<N>

  // your code here

  // 6) IMPORTANT: free anything that are dynamically allocated during the process
  // your code here







  // cleanup
  table_free(students);
  table_free(enroll);
  table_free(majors);
  schema_free(&s_students);
  schema_free(&s_enroll);
  schema_free(&s_majors);
  return 0;
}
