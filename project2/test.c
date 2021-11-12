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

  struct Type *Char = createType("char");
  assert(Char->category == PRIMITIVE);
  assert(Char->primitive == CHAR);

  struct Type *Array = createType("array");
  assert(Array->category == ARRAY);

  struct Type *Struct = createType("struct");
  assert(Struct->category == STRUCTURE);

  struct Type *Func = createType("func");
  assert(Func->category == FUNCTION);

  assert(-1 == typeEqual(Integer, Array));
  assert(-1 == typeEqual(Float, Struct));
  assert(-1 == typeEqual(Char, Func));
  assert(-1 == typeEqual(Array, Struct));
  assert(-1 == typeEqual(Array, Func));
  assert(-1 == typeEqual(Struct, Func));

  setArraySize(Array, 10);
  setArrayType(Array, Integer);
  assert(Array->array->size == 10);
  assert(Array->array->type == Integer);
  assert(typeEqual(Integer, findArrayType(Array)) == 0);

  struct Type *arraytype = createType("array");
  setArraySize(arraytype, 10);
  setArrayType(arraytype, Float);
  assert(-1 == typeEqual(Array, arraytype));
  assert(typeEqual(Float, findArrayType(arraytype)) == 0);

  addStructField(Struct, Float, "flo");
  assert(typeEqual(Float, findFieldType(Struct, "flo")) == 0);

  addStructField(Struct, Char, "cha");
  assert(typeEqual(Char, findFieldType(Struct, "cha")) == 0);

  struct Type *structtype = createType("struct");
  struct Type *floattype = createType("float");
  struct Type *chartype = createType("char");
  addStructField(structtype, floattype, "foo");
  addStructField(structtype, chartype, "bar");
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

  assert(typeEqual(inttype, findFuncParamType(functype, 0)) == 0);
  assert(typeEqual(inttype, findFuncParamType(functype, 1)) == 0);

  printf("Success!\n");
}