#ifndef TYPE_H
#define TYPE_H

enum Category { PRIMITIVE, ARRAY, STRUCTURE, FUNCTION };
enum Primitive { INTEGER, FLOATING, DOUBLE };
typedef struct Type MyType;
typedef struct ArrayType MyArrayType;
typedef struct FieldType MyFieldType;
typedef struct FunctionType MyFunctionType;
typedef struct ParameterType MyParameterType;

struct Type {
  enum Category category;
  union {
    enum Primitive primitive;
    struct ArrayType *array;
    struct FieldType *structure;
    struct FunctionType *function;
  };
};

struct ArrayType {
  int size;
  struct Type *type;
};

struct FieldType {
  char *name;
  struct Type *type;
  struct FieldType *next;
};

struct FunctionType {
  struct Type *returnType;
  struct ParameterType *parameters;
};

struct ParameterType {
  char *name;
  struct Type *type;
  struct ParameterType *next;
};

struct Type *createType(char *name);

struct FieldType *createFieldType(struct Type *type, char *name);

struct ParameterType *createParameterType(struct Type *type, char *name);

void setArraySize(struct Type *array, int size);

void setArrayType(struct Type *array, struct Type *type);

void addStructField(struct Type *structure, struct Type *field, char *name);

void addFuncParameter(struct Type *function, struct Type *param, char *name);

void setFuncReturnType(struct Type *func, struct Type *returnType);

struct Type *findArrayType(struct Type *array);

struct Type *findFieldType(struct Type *structure, char *name);

struct Type *findFuncParamType(struct Type *func, int idx);

int typeEqual(struct Type *t1, struct Type *t2);

int primitiveTypeEqual(enum Primitive t1, enum Primitive t2);

int arrayTypeEqual(struct ArrayType* t1, struct ArrayType* t2); 

int structTypeEqual(struct FieldType* t1, struct FieldType* t2);

int fieldTypeEqual(struct FieldType* t1, struct FieldType* t2);

int funcTypeEqual(struct FunctionType* t1, struct FunctionType* t2);

int parameterTypeEqual(struct ParameterType* t1, struct ParameterType* t2);


#endif /* TYPE_H */