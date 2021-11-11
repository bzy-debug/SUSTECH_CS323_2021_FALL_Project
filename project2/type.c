#include "type.h"
#include <string.h>
#include <stdlib.h>

struct Type *createType(char *str) {
  if (strcmp(str, "int") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = PRIMITIVE;
    type->primitive = INTEGER;
    return type;
  }
  if (strcmp(str, "float") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = PRIMITIVE;
    type->primitive = FLOATING;
    return type;
  }
  if (strcmp(str, "double") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = PRIMITIVE;
    type->primitive = DOUBLE;
    return type;
  }

  if (strcmp(str, "array") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = ARRAY;
    type->array = malloc(sizeof(struct ArrayType));
    return type;
  }
  if (strcmp(str, "struct") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = STRUCTURE;
    return type;
  }
  if (strcmp(str, "func") == 0) {
    struct Type *type = malloc(sizeof(struct Type));
    type->category = FUNCTION;
    type->function = malloc(sizeof(struct FunctionType));
    return type;
  }
  return NULL;
}

struct FieldType *createFileType(struct Type *type, char *name) {
  struct FieldType *field = malloc(sizeof(struct FieldType));
  field->name = name;
  field->type = type;
  field->next = NULL;
  return field;
}

struct ParameterType *createParameterType(struct Type *type, char *name) {
  struct ParameterType *param = malloc(sizeof(struct ParameterType));
  param->name = name;
  param->type = type;
  param->next = NULL;
  return param;
}

void setArraySize(struct Type *array, int size) {
  array->array->size = size;
}

void setArrayType(struct Type *array, struct Type *type) {
  array->array->type = type;
}

void addStructField(struct Type *structure, struct Type *type, char *name) {
  struct FieldType *field = createFileType(type, name);
  if (structure->structure == NULL) {
    structure->structure = field;
    return;
  }
  struct FieldType *fields = structure->structure; 
  while (fields->next != NULL) {
    fields = fields->next;
  }
  fields->next = field;
}

void addFuncParameter(struct Type *function, struct Type *type, char *name) {
  struct ParameterType *param = createParameterType(type, name);
  if (function->function->parameters == NULL) {
    function->function->parameters = param;
    return;
  }
  struct ParameterType *params = function->function->parameters;
  while (params->next != NULL) {
    params = params->next;
  }
  params->next = param;
}

void setFuncReturnType(struct Type *func, struct Type *returnType) {
  func->function->returnType = returnType;
}

int typeEqual(struct Type* t1, struct Type* t2) {
  if (t1->category != t2->category) {
    return -1;
  }

  switch (t1->category) {
    case PRIMITIVE:
      return primitiveTypeEqual(t1->primitive, t2->primitive);
    case ARRAY:
      return arrayTypeEqual(t1->array, t2->array);
    case STRUCTURE:
      return structTypeEqual(t1->structure, t2->structure);
    case FUNCTION:
      return funcTypeEqual(t1->function, t2->function);
  }
}

int primitiveTypeEqual(enum Primitive t1, enum Primitive t2) {
  if (t1 == t2) {
    return 0;
  } else {
    return -1;
  }
}

int arrayTypeEqual(struct ArrayType* t1, struct ArrayType* t2) {
  if (t1->size != t2->size || t1->type->category != t2->type->category) {
    return -1;
  }

  switch (t1->type->category) {
    case PRIMITIVE:
      return primitiveTypeEqual(t1->type->primitive, t2->type->primitive);
    case ARRAY:
      return arrayTypeEqual(t1->type->array, t2->type->array);
    case STRUCTURE:
      return structTypeEqual(t1->type->structure, t2->type->structure);
  }
}

int structTypeEqual(struct FieldType *t1, struct FieldType *t2) {
  return fieldTypeEqual(t1, t2);
}

int fieldTypeEqual(struct FieldType* t1, struct FieldType* t2) {
  if (t1->type->category != t2->type->category) {
    return -1;
  }

  switch (t1->type->category) {
    case PRIMITIVE:
      if (-1 == primitiveTypeEqual(t1->type->primitive, t2->type->primitive)) {
        return -1;
      }
      break;
    case ARRAY:
      if (-1 == arrayTypeEqual(t1->type->array, t2->type->array)) {
        return -1;
      }
      break;
    case STRUCTURE:
      if (-1 == structTypeEqual(t1->type->structure, t2->type->structure)) {
        return -1;
      }
      break;
  }
  
  if (t1->next == NULL && t2->next == NULL) {
    return 0;
  } else if (t1->next != NULL && t2->next != NULL) {
    return fieldTypeEqual(t1->next, t2->next);
  } else {
    return -1;
  }
}

int funcTypeEqual(struct FunctionType* t1, struct FunctionType* t2) {
  // if (-1 == typeEqual(t1->returnType, t2->returnType)) {
  //   return -1;
  // }

  return parameterTypeEqual(t1->parameters, t2->parameters);
}

int parameterTypeEqual(struct ParameterType* t1, struct ParameterType* t2) {
  if (-1 == typeEqual(t1->type, t2->type)) {
    return -1;
  }

  if (t1->next == NULL && t2->next == NULL) {
    return 0;
  } else if (t1->next != NULL && t2->next != NULL) {
    return parameterTypeEqual(t1->next, t2->next);
  } else {
    return -1;
  }
}
