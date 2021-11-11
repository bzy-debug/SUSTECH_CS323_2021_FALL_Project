#include "type.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
  struct Type *Integer = createType("int");
  assert(Integer->category == PRIMITIVE);
  assert(Integer->primitive == INTEGER);

  struct Type *Float = createType("float");
  assert(Float->category == PRIMITIVE);
  assert(Float->primitive == FLOATING);

  struct Type *Double = createType("double");
  assert(Double->category == PRIMITIVE);
  assert(Double->primitive == DOUBLE);

  struct Type *Array = createType("array");
  assert(Array->category == ARRAY);

  struct Type *Struct = createType("struct");
  assert(Struct->category == STRUCTURE);

  struct Type *Func = createType("func");
  assert(Func->category == FUNCTION);

  assert(-1 == typeEqual(Integer, Array));
  assert(-1 == typeEqual(Float, Struct));
  assert(-1 == typeEqual(Double, Func));
  assert(-1 == typeEqual(Array, Struct));
  assert(-1 == typeEqual(Array, Func));
  assert(-1 == typeEqual(Struct, Func));

  setArraySize(Array, 10);
  setArrayType(Array, Integer);
  assert(Array->array->size == 10);
  assert(Array->array->type == Integer);

  struct Type *arraytype = createType("array");
  setArraySize(arraytype, 10);
  setArrayType(arraytype, Float);
  assert(-1 == typeEqual(Array, arraytype));

  addStructField(Struct, Float, "flo");
  assert(Struct->structure->type == Float);
  assert(strcmp(Struct->structure->name, "flo") == 0);

  addStructField(Struct, Double, "dou");
  assert(Struct->structure->next->type == Double);
  assert(strcmp(Struct->structure->next->name, "dou") == 0);

  struct Type *structtype = createType("struct");
  struct Type *floattype = createType("float");
  struct Type *doubletype = createType("double");
  addStructField(structtype, floattype, "foo");
  addStructField(structtype, doubletype, "bar");
  assert(typeEqual(Struct, structtype) == 0);

  addStructField(structtype, Integer, "var");
  assert(-1 == typeEqual(Struct, structtype));

  addFuncParameter(Func, Integer, "x");
  addFuncParameter(Func, Integer, "y");
  setFuncReturnType(Func, Integer);
  assert(Func->function->parameters->type == Integer);
  assert(strcmp(Func->function->parameters->name, "x") == 0);
  assert(Func->function->returnType == Integer);

  struct Type *functype = createType("func");
  struct Type *inttype = createType("int");
  addFuncParameter(functype, inttype, "a");
  addFuncParameter(functype, inttype, "b");
  assert(typeEqual(functype, Func) == 0);

  printf("Success!\n");
}