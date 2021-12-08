#include"inter_code.h"
#include<assert.h>
#include<string.h>

int main() {
    inter_code* c = cnt_ic(DEF_LAB, 1, cnt_op_int(LABEL, 1));
    print_code(c);
    c = cnt_ic(DEF_FUNC, 1, cnt_op_str(FUNCTION, "myfunction"));
    print_code(c);
    print_code(cnt_ic(ASSIGN, 2, cnt_op_str(VARIABLE, "x"), cnt_op_int(CONSTANT, 12)));
    print_code(cnt_ic(ADD, 3, cnt_op_str(VARIABLE, "y"), cnt_op_str(VARIABLE, "z"), cnt_op_str(VARIABLE,"x")));
    print_code(cnt_ic(SUB, 3, cnt_op_str(VARIABLE, "y"), cnt_op_str(VARIABLE, "z"), cnt_op_str(VARIABLE,"x")));
    print_code(cnt_ic(DIV, 3, cnt_op_str(VARIABLE, "y"), cnt_op_str(VARIABLE, "z"), cnt_op_str(VARIABLE,"x")));
    print_code(cnt_ic(MUL, 3, cnt_op_str(VARIABLE, "y"), cnt_op_str(VARIABLE, "z"), cnt_op_str(VARIABLE,"x")));
}