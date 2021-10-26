# Report

11910501 鲍志远(A)
		11910934 吴泽敏(A)
		11810925 周翊澄(A)

#### Basic:

##### Lexer

The lexer is rather simple, there are three points worth mentioning. First, the regular expression. To support the feature of recognizing A-type error, we must write the regex of possible wrong patterns. When lexer read the wrong pattern, it will report an A-type error. Second is about grammar tree. When lexer encounters one valid lexeme, it will construct a node containing needed information of that lexeme. The design of node will be discussed later. Third, line number support and ```iserror``` variable. Add ```%option yylineno``` and ```YY_USER_ACTION``` macro to support line number in flex and bison respectively. ```iserror``` is declared with ```static``` to recognize whether the spl source file has errors or not. (0 means no error, 1 means error)

##### Bison

Every token and nonterminal in bison is the ```node*``` type to construct the tree while parsing. When several tokens and nonterminal are reduced to a new nonterminal, it will form a tree like this:

![1](C:\Users\Bzy\学习\cs323\project1\1.png)
When it encounters an $\epsilon$ rule, it will construct a node with ```isempty = 0``` . This kind of node will not be printed. There is a new token```INVALID_TOKEN```, it makes parser work properly while lexer finds an A-type error in source file. Bison's error recovery works for recognizing B-type error.

##### Grammar Tree

```node.h``` and ```node.c``` together define and implement the grammar tree. While c do not have ```vector```, we use sibling representation to implement the quadtree (this method comes from *Instruction to Algorithms*). The variable name and comment can express the meaning of each filed.

```c
typedef struct NODE
{
    nodeType node_type;
    int isempty; // 0 not empty, 1 empty. for nterms
    int line; //lineno
    nodeVal val;
    struct NODE* child;
    struct NODE* sibling;
} node;
```

Also, it has two functions ```addchild``` and ```print_tree```. ```addchild``` support varying arguments and is able to add several nodes to their parent. ```print_tree``` will print the tree in dfs from root. Sadly, c does not have stack, so we use recursive way to print.

####  Bonus:

we support one line and multi-line comment, and they are implemented in lexer. one line comment is rather simple, it just eats the rest from "//" until meets a '\n'. multi-line comment is a little tricky, and we refer to the flex manual. 

```c
"/*" {
    int c;
    for( ; ; ){
        c = input();
        while( c != '*' && c != 0)
            c = input();
        if ( c == '*' ){
            while ( (c = input()) == '*' )
                ;
            if ( c == '/')
                break;
        }
        if ( c == 0 ){
            printf("error: EOF in comment\n");
            iserror = 1;
            break;
        }
    }
}
```

When lexer encounters "/*", which means the beginning of comment, it starts eating until it meets '\*' or EOF(0). When it reads '\*', it's possible that next char is '/' and it stops. But there is an occasion like this: "\*\*\*\*\*\*\*\*\*\*\*\*/", so it will eat '\*' as many as possible. When '/' occurs after '\*', comment finishes. Otherwise, it encounters EOF within comment, and reports an error.