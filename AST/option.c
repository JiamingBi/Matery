#include "option.h"
#include "string.h"
//生成抽象语法树节点
struct node *makeNode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}
//用于绘制抽象语法树新节点名生成
char * newNode()
{
    static int no=1;
    char s[10];
    intToChar(no++,s,10);
    return stringConcatenate("node",s);
}

//生成.gv文件
void createAST(struct node *T,char * fkind)
{
    int i = 1;
    int j=0;
    struct node *T0;
    char  a[100];
    char  b[100];
    char  c[100];
    for(j=0;j<100;j++)
    {
        a[j]=b[j]=c[j]='\0';
    }
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            strcpy(a,newNode());
            if(strcmp(fkind," "))
            {
                fprintf(fp,"%s->%s\n",fkind,a);
            }
            fprintf(fp,"%s[label=\"EXT_DEF_LIST\"]\n",a);
            createAST(T->ptr[0],a);
            createAST(T->ptr[1],a);
            break;
        case EXT_VAR_DEF:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXT_VAR_DEF\"]\n",a);
            createAST(T->ptr[0],a); //全局变量类型
            createAST(T->ptr[1],a); //全局变量列表
            break;
        case TYPE:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"TYPE %s\"]\n",a,T->type_id);
            break;
        case EXT_DEC_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXT_DEC_LIST\"]\n",a);
            createAST(T->ptr[0],a);  //全局变量名   
            createAST(T->ptr[1],a); 
            break;
        case FUNC_DEF:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_DEF\"]\n",a);
            createAST(T->ptr[0],a); //函数返回类型
            createAST(T->ptr[1],a); //函数名和参数
            createAST(T->ptr[2],a); //函数体
            break;
         case CLASS_DEF:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"CLASS_DEF\"]\n",a);
            createAST(T->ptr[0],a); //函数名和参数
            createAST(T->ptr[1],a); //函数体
            break;
         case CLASS_DEC:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_DEC %s\"]\n",a,T->type_id);
        case FUNC_DEC:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_DEC %s\"]\n",a,T->type_id);
            if (T->ptr[0])
            {
                //含参
                createAST(T->ptr[0],a); //函数参数列表
            }
            break;
        case PARAM_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"PARAM_LIST\"]\n",a);
            createAST(T->ptr[0],a); //全部参数类型和名称，
            createAST(T->ptr[1],a);
            break;
        case PARAM_DEC:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"PARAM_DEC\"]\n",a);
            strcpy(b,newNode());
            fprintf(fp,"%s->%s\n",a,b);
            fprintf(fp,"%s[label=\"%s\"]\n",b,T->ptr[0]->type == INT ? "int" : T->ptr[0]->type == FLOAT ? "float"
                                                                                                    : "char");                                                                
            strcpy(c,newNode());
            fprintf(fp,"%s->%s\n",a,c);
            fprintf(fp,"%s[label=\"%s\"]\n",c,T->ptr[1]->type_id);
            break;
        case EXP_STMT:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"EXP_STMT\"]\n",a);
            createAST(T->ptr[0],a);
            break;
        case RETURN:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"RETURN\"]\n",a);
            createAST(T->ptr[0],a);
            break;
        case COMP_STM:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"COMP_STM\"]\n",a);
            createAST(T->ptr[0],a); //定义部分
            createAST(T->ptr[1],a); //语句部分
            break;
        case STM_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"STM_LIST\"]\n",a);
            createAST(T->ptr[0],a); //显示第一条语句
            createAST(T->ptr[1],a); //显示剩下语句
            break;
        case WHILE:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"WHILE\"]\n",a);
            createAST(T->ptr[0],a); //循环条件
            createAST(T->ptr[1],a); //循环体
            break;
        case IF_THEN:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"IF_THEN\"]\n",a);
            createAST(T->ptr[0],a); //条件
            createAST(T->ptr[1],a); //if子句
            break;
        case IF_THEN_ELSE:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"IF_THEN_ELSE\"]\n",a);
            createAST(T->ptr[0],a); //条件
            createAST(T->ptr[1],a); //if子句
            createAST(T->ptr[2], a); //else子句
            break;
        case FOR:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"For\"]\n",a);
            createAST(T->ptr[0],a); //条件
            createAST(T->ptr[1],a); //if子句
            createAST(T->ptr[2], a); //else子句
            break;
        case DEF_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"DEF_LIST\"]\n",a);
            createAST(T->ptr[0],a); //该局部变量定义列表中的第一个
            createAST(T->ptr[1],a); //其它局部变量定义
            break;
        case VAR_DEF:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"VAR_DEF\"]\n",a);
            createAST(T->ptr[0],a); //变量类型
            createAST(T->ptr[1],a); //该定义的全部变量名
            break;
        case DEC_LIST:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"DEC_LIST\"]\n",a);
            T0 = T;
            while (T0)
            {
                if (T0->ptr[0]->kind == ID)
                {  
                    strcpy(b,newNode());
                    fprintf(fp,"%s->%s\n",a,b);
                    fprintf(fp,"%s[label=\"ID %s\"]\n",b,T0->ptr[0]->type_id);
                }
                else if (T0->ptr[0]->kind == ASSIGNOP)
                {
                    //显示初始化表达式
                    createAST(T0->ptr[0],a);
                }
                else if (T0->ptr[0]->kind == Array)
                {
                    createAST(T0->ptr[0],a);
                }
                T0 = T0->ptr[1];
            }
            break;
        case ID:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"ID %s\"]\n",a,T->type_id);
            break;
        case Array:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"Array %s[%d]\"]\n",a,T->ptr[0]->type_id,T->ptr[1]->type_int);
            break;
        case INT:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"INT %d\"]\n",a,T->type_int);
            break;
        case FLOAT:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FLOAT %f\"]\n",a,T->type_float);
            break;
        case CHAR:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"CHAR %c\"]\n",a,T->type_char);
            break;
        case BREAK:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"BREAK\"]\n",a);
            break;
        case CONTINUE:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"CONTINUE\"]\n",a);
            break;
        case ASSIGNOP:
        case AND:
        case OR:
        case RELOP:
        case PLUS:
        case PLUSASS:
        case USELFPLUS:
        case MSELFPLUS:
        case USLEFMINUS:
        case MSLEFMINUS:
        case MINUS:
        case MINUSASS:
        case STAR:
        case DIV:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"%s\"]\n",a,T->type_id);
            createAST(T->ptr[0],a);
            createAST(T->ptr[1],a);
            break;
        case NOT:
        case UMINUS:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"%s\"]\n",a,T->type_id);
            createAST(T->ptr[0],a);
            break;

        case Array_Call:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"Array_Call %s[%d]\"]\n",a,T->type_id,T->ptr[0]->type_int);
            break;
        case FUNC_CALL:
            strcpy(a,newNode());
            fprintf(fp,"%s->%s\n",fkind,a);
            fprintf(fp,"%s[label=\"FUNC_CALL %s\"]\n",a,T->type_id);
            createAST(T->ptr[0],a);
            break;
        case ARGS:
            i = 1;
            while (T)
            {
                struct node *T0 = T->ptr[0];
                strcpy(a,newNode());
                fprintf(fp,"%s->%s\n",fkind,a);
                fprintf(fp,"%s[label=\"ARGS\"]\n",a);
                createAST(T->ptr[0],a);
                T = T->ptr[1];
            }
            break;
        }
    }
}

//int转换为字符串
char *intToChar(int num, char *str, int radix)
{ /*索引表*/
    char index[] = "0123456789ABCDEF";
    unsigned unum; /*中间变量*/
    int i = 0, j, k;
    /*确定unum的值*/
    if (radix == 10 && num < 0) /*十进制负数*/
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num; /*其他情况*/
    /*转换*/
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    /*逆序*/
    if (str[0] == '-')
        k = 1; /*十进制负数*/
    else
        k = 0;

    for (j = k; j <= (i - 1) / 2; j++)
    {
        char temp;
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}

//连接字符
char *stringConcatenate(char *s1, char *s2)
{
    static char result[10];
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//变量别名生成
char *newValueName()
{
    static int no = 1;
    char s[10];
    intToChar(no++, s, 10);
    return stringConcatenate("v", s);
}

//label名字生成
char *newLabelName()
{
    static int no = 1;
    char s[10];
    intToChar(no++, s, 10);
    return stringConcatenate("label", s);
}

//临时变量名生成
char *newTempName()
{
    static int no = 1;
    char s[10];
    intToChar(no++, s, 10);
    return stringConcatenate("temp", s);
}

//生成一条TAC代码的结点组成的双向循环链表，返回头指针
struct irnode *getIR(int op, struct opn opn1, struct opn opn2, struct opn result)
{
    struct irnode *h = (struct irnode *)malloc(sizeof(struct irnode));
    h->op = op;
    h->opn1 = opn1;
    h->opn2 = opn2;
    h->result = result;
    h->next = h->prior = h;
    return h;
}

//生成一条标号语句，返回头指针
struct irnode *getLabelIR(char *label)
{
    struct irnode *h = (struct irnode *)malloc(sizeof(struct irnode));
    h->op = LABEL;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    return h;
}

//生成GOTO语句，返回头指针
struct irnode *getGotoIR(char *label)
{
    struct irnode *h = (struct irnode *)malloc(sizeof(struct irnode));
    h->op = GOTO;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    return h;
}

//合并多个中间代码的双向循环链表，首尾相连
struct irnode *mergeIR(int num, ...)
{
    struct irnode *h1, *h2, *p, *t1, *t2;
    va_list ap;
    va_start(ap, num);
    h1 = va_arg(ap, struct irnode *);
    while (--num > 0)
    {
        h2 = va_arg(ap, struct irnode *);
        if (h1 == NULL)
            h1 = h2;
        else if (h2)
        {
            t1 = h1->prior;
            t2 = h2->prior;
            t1->next = h2;
            t2->next = h1;
            h1->prior = t2;
            h2->prior = t1;
        }
    }
    va_end(ap);
    return h1;
}

// 输出四元式
void printIR(struct irnode *head)
{
    char opnstr1[32], opnstr2[32], resultstr[32];
    struct irnode *h = head;
    char f[100];
    char c[100];
    fprintf(fp1,"other[color=\"white\"]\n");
    fprintf(fp1,"other[label=\"\"]\n");
    fprintf(fp1,"start[label=\"\n");
    strcpy(f,"start");
    strcpy(c,"start");
    if(h==NULL)
    {
        return;
    }
    do
    {
        if (h->opn1.kind == INT)
            sprintf(opnstr1, "#%d", h->opn1.const_int);
        if (h->opn1.kind == FLOAT)
            sprintf(opnstr1, "#%f", h->opn1.const_float);
        if (h->opn1.kind == CHAR)
            sprintf(opnstr1, "#%c", h->opn1.const_char);
        if (h->opn1.kind == ID)
            sprintf(opnstr1, "%s", h->opn1.id);
        if (h->opn2.kind == INT)
            sprintf(opnstr2, "#%d", h->opn2.const_int);
        if (h->opn2.kind == FLOAT)
            sprintf(opnstr2, "#%f", h->opn2.const_float);
        if (h->opn1.kind == CHAR)
            sprintf(opnstr2, "#%c", h->opn1.const_char);
        if (h->opn2.kind == ID)
            sprintf(opnstr2, "%s", h->opn2.id);
        sprintf(resultstr, "%s", h->result.id);
        char c[10];
        switch (h->op)
        {
        case ASSIGNOP:
            if (h->result.kind == Array_Call)
            {
                printf("([]=\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
                fprintf(fp1,"([]=\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            }
            else
            {
                printf("(:=\t\t,%s\t\t,_\t\t,%-12s)\n", opnstr1, resultstr);
                fprintf(fp1,"(:=\t\t,%s\t\t,_\t\t,%-12s)\n", opnstr1, resultstr);
            }
            break;
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
            printf("(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", h->op == PLUS ? '+' : h->op == MINUS ? '-'
                                                                       : h->op == STAR    ? '*'
                                                                                          : '\\',
                    opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", h->op == PLUS ? '+' : h->op == MINUS ? '-'
                                                                : h->op == STAR    ? '*'
                                                                                   : '\\',
                    opnstr1, opnstr2, resultstr);
            break;
        case AND:
        case OR:
        case RELOP:
            if (h->op == AND)
            {
                strcpy(c, "&&");
            }
            else if (h->op == OR)
            {
                strcpy(c, "||");
            }
            else
            {
                strcpy(c, h->result.rtype);
            }
            printf("(%s\t\t,%s\t\t,%s\t\t,%-12s)\n", c, opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(%s\t\t,%s\t\t,%s\t\t,%-12s)\n", c, opnstr1, opnstr2, resultstr);
            break;
        case SELFPLUS:
        case SLEFMINUS:
            printf("(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", h->op == SELFPLUS ? '+' : '-', opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", h->op == SELFPLUS ? '+' : '-', opnstr1, opnstr2, resultstr);
            break;
        case PLUSASS:
            printf("(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", '+', resultstr, opnstr1, resultstr);
            fprintf(fp1,"(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", '+', resultstr, opnstr1, resultstr);
            break;
        case MINUSASS:
            printf("(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", '-', resultstr, opnstr1, resultstr);
            fprintf(fp1,"(%c\t\t,%s\t\t,%s\t\t,%-12s)\n", '-', resultstr, opnstr1, resultstr);
            break;
        case FUNCTION:
            printf("(FUNCTION\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            fprintf(fp1,"(FUNCTION\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            break;
        case PARAM:
            // printf("(PARAM\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            // fprintf(fp1,"(PARAM\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            break;
        case LABEL:
            printf("(LABEL\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            fprintf(fp1,"\"]\n");
            strcpy(c,h->result.id);
            if(f[0]!=' ')
            {
                fprintf(fp1,"%s->%s\n",f,c);
            }
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            fprintf(fp1,"(LABEL\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            break;
        case GOTO:
            printf("(J\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            fprintf(fp1,"(J\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            fprintf(fp1,"\"]\n");
            // strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f," ");
            // fprintf(fp1,"%s[label=\"\n",c);
            fprintf(fp1,"other[label=\"\n");
            break;
        case JLE: // <=
            printf("(JLE\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JLE\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case JLT: // <
            printf("(JLT\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JLT\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case JGE: // >=
            printf("(JGE\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JGE\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case JGT: // >
            printf("(JGT\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JGT\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case EQ: // ==
            printf("(JEQ\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JEQ\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case NEQ: // !=
            printf("(JNEQ\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"(JNEQ\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            fprintf(fp1,"\"]\n");
            strcpy(c,newNode());
            fprintf(fp1,"%s->%s\n",f,c);
            fprintf(fp1,"%s->%s\n",f,resultstr);
            strcpy(f,c);
            fprintf(fp1,"%s[label=\"\n",c);
            break;
        case ARG:
            // printf("(ARG\t\t,_\t\t,_\t\t,%-12s)\n", h->result.id);
            break;
        case CALL:
            // printf("(CALL\t\t,%s\t\t,%s\t\t,%-12s)\n", opnstr1, opnstr2, resultstr);
            printf("(CALL\t\t,_\t\t,_\t\t,%-12s)\n", opnstr1);
            break;
        case RETURN:
            if (h->result.kind)
            {
                printf("(RETURN\t\t,_\t\t,_\t\t,%-12s)\n", resultstr);
                fprintf(fp1,"(RETURN\t\t,_\t\t,_\t\t,%-12s)\n", resultstr);
            }
            else
            {
                printf("(RETURN\t\t,_\t\t,_\t\t,_\t\t)\n");
                fprintf(fp1,"(RETURN\t\t,_\t\t,_\t\t,_\t\t)\n");
            }
            break;
        }
        h = h->next;
    } while (h != head);
    fprintf(fp1,"\"]\n");
}

//收集错误信息，最后一次性显示
void semanticError(int line, char *msg1, char *msg2)
{
    // printf("在%d行,%s %s\n",line,msg1,msg2);
    errorTable.errs[errorTable.index].line = line;
    errorTable.errs[errorTable.index].msg1 = msg1;
    errorTable.errs[errorTable.index].msg2 = msg2;
    errorTable.index++;
}

//输出错误
void prnErr()
{
    int i;
    for (i = 0; i < errorTable.index; i++)
    {
        printf("在%d行,%s %s\n", errorTable.errs[i].line,
               errorTable.errs[i].msg1,
               errorTable.errs[i].msg2);
    }
}

//显示符号表
void printSymbol()
{
    int i = 0;
    printf("%s\t\t%s\t\t%s\t\t%s\t%s\n", "变量名", "别  名", "层  号", "数据类型", "标记类型");
    char ptype[10];
    char pflag[20];
    for (i = 0; i < symbolTable.index; i++)
    {
        if (symbolTable.symbols[i].type == INT)
        {
            strcpy(ptype, "int");
        }
        else if (symbolTable.symbols[i].type == FLOAT)
        {
            strcpy(ptype, "float");
        }
        else if (symbolTable.symbols[i].type == CHAR)
        {
            strcpy(ptype, "char");
        }

        if (symbolTable.symbols[i].flag == 'F')
        {
            strcpy(pflag, "函数定义");
        }
        else if (symbolTable.symbols[i].flag == 'V')
        {
            strcpy(pflag, "变量名");
        }
        else if (symbolTable.symbols[i].flag == 'P')
        {
            strcpy(pflag, "参数");
        }
        else if (symbolTable.symbols[i].flag == 'T')
        {
            strcpy(pflag, "临时变量");
        }
        else if (symbolTable.symbols[i].flag =='D')
        {
            strcpy(pflag,"函数声明");
        }

        printf("%s\t\t%s\t\t%d\t\t%s\t\t%s\n", symbolTable.symbols[i].name,
               symbolTable.symbols[i].alias, symbolTable.symbols[i].level,
               ptype,
               pflag);
        strcpy(ptype, "");
    }
}

//搜索符号表，返回位置，若无返回-1
int searchSymbolTable(char *name)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
        if (!strcmp(symbolTable.symbols[i].name, name))
            return i;
    return -1;
}

//填充符号表
int fillSymbolTable(char *name, char *alias, int level, int type, char flag)
{
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for (i = symbolTable.index - 1; symbolTable.symbols[i].level == level || (level == 0 && i >= 0); i--)
    {
        if (level == 0 && symbolTable.symbols[i].level == 1)
            continue; //外部变量和形参不必比较重名
        if(flag=='F'&&symbolTable.symbols[i].flag=='D')
            continue;//函数定义和函数声明不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))
            return -1;
    }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name, name);
    strcpy(symbolTable.symbols[symbolTable.index].alias, alias);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}

//填写临时变量到符号表，返回临时变量在符号表中的位置
int fillTemp(char *name, int level, int type, char flag)
{
    strcpy(symbolTable.symbols[symbolTable.index].name, "");
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}

int LEV = 0;   //层号

//处理变量列表
void dealExtvarlist(struct node *T)
{
    int rtn, num = 1;
    switch (T->kind)
    {
    case EXT_DEC_LIST:
        T->ptr[0]->type = T->type; //将类型属性向下传递变量结点
        T->ptr[1]->type = T->type; //将类型属性向下传递变量结点
        dealExtvarlist(T->ptr[0]);
        dealExtvarlist(T->ptr[1]);
        T->num = T->ptr[1]->num + 1;
        break;
    case ID:
        rtn = fillSymbolTable(T->type_id, newValueName(), LEV, T->type, 'V'); //最后一个变量名
        if (rtn == -1)
        {
            semanticError(T->pos, T->type_id, "变量重复定义");
        }
        else
        {
            T->place = rtn;
        }
        T->num = 1;
        break;
    case Array:
        rtn = fillSymbolTable(T->ptr[0]->type_id, newValueName(), LEV, T->type, 'V');
        if (rtn == -1)
            semanticError(T->pos, T->type_id, "变量重复定义");
        else
            T->place = rtn;
        T->num = 1;
        break;
    }
}

//函数错误检查
int matchParam(int i, struct node *T)
{
    int j, num = symbolTable.symbols[i].paramnum;
    int type1, type2;
    if (num == 0 && T == NULL)
        return 1;
    for (j = 1; j < num; j++)
    {
        if (!T)
        {
            semanticError(T->pos, "", "函数调用参数太少");
            return 0;
        }
        type1 = symbolTable.symbols[i + j].type; //形参类型
        type2 = T->ptr[0]->type;
        if (type1 != type2)
        {
            semanticError(T->pos, "", "参数类型不匹配");
            return 0;
        }
        T = T->ptr[1];
    }
    if (T->ptr[1])
    { //num个参数已经匹配完，还有实参表达式
        semanticError(T->pos, "", "函数调用参数太多");
        return 0;
    }
    return 1;
}

//布尔表达式
void boolExp(struct node *T)
{ 
    struct opn opn1, opn2, result;
    int op;
    int rtn;
    if (T)
    {
        switch (T->kind)
        {
        case INT:
            if (T->type_int != 0)
            {
                T->ir = getGotoIR(T->Etrue);
            }
            else
            {
                T->ir = getGotoIR(T->Efalse);
            }
            break;
        case FLOAT:
            if (T->type_float != 0.0)
            {
                T->ir = getGotoIR(T->Etrue);
            }
            else
            {
                T->ir = getGotoIR(T->Efalse);
            }
            break;
        case ID: //查符号表，获得符号表中的位置，类型送type
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
                semanticError(T->pos, T->type_id, "变量未定义");
            if (symbolTable.symbols[rtn].flag == 'F')
            {
                semanticError(T->pos, T->type_id, "是函数名，类型不匹配");
            }
            else
            {
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[rtn].alias);
                opn2.kind = INT;
                opn2.const_int = 0;
                result.kind = ID;
                strcpy(result.id, T->Etrue);
                T->ir = getIR(NEQ, opn1, opn2, result);
                T->ir = mergeIR(2, T->ir, getGotoIR(T->Efalse));
            }
            break;
        case RELOP: //处理关系运算表达式,2个操作数都按基本表达式处理
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            result.kind = ID;
            strcpy(result.id, T->Etrue);
            if (strcmp(T->type_id, "<") == 0)
                op = JLT;
            else if (strcmp(T->type_id, "<=") == 0)
                op = JLE;
            else if (strcmp(T->type_id, ">") == 0)
                op = JGT;
            else if (strcmp(T->type_id, ">=") == 0)
                op = JGE;
            else if (strcmp(T->type_id, "==") == 0)
                op = EQ;
            else if (strcmp(T->type_id, "!=") == 0)
                op = NEQ;
            T->ir = getIR(op, opn1, opn2, result);
            T->ir = mergeIR(4, T->ptr[0]->ir, T->ptr[1]->ir, T->ir, getGotoIR(T->Efalse));
            break;
        case AND:
        case OR:
            if (T->kind == AND)
            {
                strcpy(T->ptr[0]->Etrue, newLabelName());
                strcpy(T->ptr[0]->Efalse, T->Efalse);
            }
            else
            {
                strcpy(T->ptr[0]->Etrue, T->Etrue);
                strcpy(T->ptr[0]->Efalse, newLabelName());
            }
            strcpy(T->ptr[1]->Etrue, T->Etrue);
            strcpy(T->ptr[1]->Efalse, T->Efalse);
            boolExp(T->ptr[0]);
            boolExp(T->ptr[1]);
            if (T->kind == AND)
            {
                T->ir = mergeIR(3, T->ptr[0]->ir, getLabelIR(T->ptr[0]->Etrue), T->ptr[1]->ir);
            }
            else
            {
                T->ir = mergeIR(3, T->ptr[0]->ir, getLabelIR(T->ptr[0]->Efalse), T->ptr[1]->ir);
            }
            break;
        case NOT:
            strcpy(T->ptr[0]->Etrue, T->Efalse);
            strcpy(T->ptr[0]->Efalse, T->Etrue);
            boolExp(T->ptr[0]);
            T->ir = T->ptr[0]->ir;
            break;
        }
    }
}

 //处理基本表达式
void Exp(struct node *T)
{
    int rtn, num;
    struct node *T0;
    struct opn opn1, opn2, result;
    if (T)
    {
        switch (T->kind)
        {
        case ID: //查符号表，获得符号表中的位置，类型送type
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
                semanticError(T->pos, T->type_id, "变量未定义");
            if (symbolTable.symbols[rtn].isArray == 1)
                semanticError(T->pos, T->type_id, "不能对数组名赋值");
            if (symbolTable.symbols[rtn].flag == 'F')
                semanticError(T->pos, T->type_id, "是函数名，类型不匹配");
            else
            {
                T->place = rtn; //结点保存变量在符号表中的位置
                T->ir = NULL; //标识符不需要生成TAC
                T->type = symbolTable.symbols[rtn].type;
            }
            break;
        case INT:
            T->place = fillTemp(newTempName(), LEV, T->type, 'T'); //为整常量生成一个临时变量
            T->type = INT;
            opn1.kind = INT;
            opn1.const_int = T->type_int;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            T->ir = getIR(ASSIGNOP, opn1, opn2, result);
            break;
        case FLOAT:
            T->place = fillTemp(newTempName(), LEV, T->type, 'T'); //为浮点常量生成一个临时变量
            T->type = FLOAT;
            opn1.kind = FLOAT;
            opn1.const_float = T->type_float;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            T->ir = getIR(ASSIGNOP, opn1, opn2, result);
            break;
        case CHAR:
            T->place = fillTemp(newTempName(), LEV, T->type, 'T'); //为浮点常量生成一个临时变量
            T->type = CHAR;
            opn1.kind = CHAR;
            opn1.const_char = T->type_char;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            T->ir = getIR(ASSIGNOP, opn1, opn2, result);
            break;
        case BREAK:
            if(strcmp(T->Bnext,""))
            {
                T->ir=getGotoIR(T->Bnext);
            }
            else
            {
                semanticError(T->pos,"","break语句使用位置有误");
            }
            break;
        case CONTINUE:
            if(strcmp(T->Cnext,""))
            {
                T->ir=getGotoIR(T->Cnext);
            }
            else
            {
                semanticError(T->pos,"","continue语句使用位置有误");
            }
            break;
        case ASSIGNOP:
            if (T->ptr[0]->kind != ID && T->ptr[0]->kind != Array_Call)
            {
                semanticError(T->pos, "", "赋值语句需要左值");
            }
            else
            {
                Exp(T->ptr[0]); //处理左值，例中仅为变量
                Exp(T->ptr[1]);
                if (T->ptr[0]->kind == Array_Call)
                {
                    rtn = searchSymbolTable(T->ptr[0]->type_id);
                    if (symbolTable.symbols[rtn].type != T->ptr[1]->type)
                    {
                        semanticError(T->pos, "", "赋值语句数组类型不一致");
                    }
                }
                else if (T->ptr[1]->kind == FUNC_CALL)
                {
                    int rtn_func = searchSymbolTable(T->ptr[1]->type_id);
                    rtn = searchSymbolTable(T->ptr[0]->type_id);
                    if (symbolTable.symbols[rtn].type != symbolTable.symbols[rtn_func].type)
                    {
                        semanticError(T->pos, "", "赋值语句左右类型不一致");
                    }
                }
                else if (T->ptr[0]->type != T->ptr[1]->type)
                {
                    semanticError(T->pos, "", "赋值语句左右类型不一致");
                }
                T->type = T->ptr[0]->type;
                T->ir = mergeIR(2, T->ptr[0]->ir, T->ptr[1]->ir);
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias); //右值一定是个变量或临时变量
                if (T->ptr[0]->kind == Array_Call)
                {
                    result.kind = Array_Call;
                    Exp(T->ptr[0]->ptr[0]);
                    opn2.kind = ID;
                    strcpy(opn2.id, symbolTable.symbols[T->ptr[0]->ptr[0]->place].alias);
                }
                else
                {
                    result.kind = ID;
                }
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                if (T->ptr[1]->kind == USELFPLUS || T->ptr[1]->kind == USLEFMINUS)
                {
                    T->ir = mergeIR(2, getIR(ASSIGNOP, opn1, opn2, result), T->ir);
                }
                else
                {
                    T->ir = mergeIR(2, T->ir, getIR(ASSIGNOP, opn1, opn2, result));
                }
            }
            break;
        case AND:
        case OR:
        case RELOP:
            T->type = INT;
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            if (T->ptr[0]->type != T->ptr[1]->type)
            {
                semanticError(T->pos, "", "双目运算两边类型不一致");
            }
            if (T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT)
            {
                T->type = FLOAT;
            }
            else
            {
                T->type = INT;
            }
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.type = T->ptr[0]->type;
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            opn2.type = T->ptr[1]->type;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.type = T->type;
            result.rtype = T->type_id;
            T->ir = mergeIR(3, T->ptr[0]->ir, T->ptr[1]->ir, getIR(T->kind, opn1, opn2, result));
            break;
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
            //下面的类型属性计算，没有考虑错误处理情况
            if (T->ptr[0]->type != T->ptr[1]->type)
            {
                semanticError(T->pos, "", "双目运算两边类型不一致");
            }
            if (T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT)
            {
                T->type = FLOAT;
            }
            else
            {
                T->type = INT;
            }
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.type = T->ptr[0]->type;
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            opn2.type = T->ptr[1]->type;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            result.type = T->type;
            T->ir = mergeIR(3, T->ptr[0]->ir, T->ptr[1]->ir, getIR(T->kind, opn1, opn2, result));
            break;
        case USELFPLUS:
        case MSELFPLUS:
            Exp(T->ptr[0]);
            if (T->ptr[0]->kind != ID && T->ptr[0]->kind != Array_Call)
                semanticError(T->pos, "", "自增操作对象不是变量");
            if (T->ptr[0]->type == CHAR)
                semanticError(T->pos, "", "自增操作对象不能是字符");
            T->type = T->ptr[0]->type;
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn2.kind = INT;
            opn2.const_int = 1;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            T->ir = getIR(SELFPLUS, opn1, opn2, result);
            break;
        case USLEFMINUS:
        case MSLEFMINUS:
            Exp(T->ptr[0]);
            if (T->ptr[0]->kind != ID && T->ptr[0]->kind != Array_Call)
                semanticError(T->pos, "", "自减操作对象不是变量");
            if (T->ptr[0]->type == CHAR)
                semanticError(T->pos, "", "自减操作对象不能是字符");
            T->type = T->ptr[0]->type;
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn2.kind = INT;
            opn2.const_int = 1;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            T->ir = getIR(SLEFMINUS, opn1, opn2, result);
            break;
        case PLUSASS:
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            if (T->ptr[0]->kind != ID && T->ptr[0]->kind != Array_Call)
                semanticError(T->pos, "", "复合加操作对象不是变量");
            if (T->ptr[0]->type != T->ptr[1]->type)
            {
                if (T->ptr[0]->kind == Array_Call)
                {
                    if (symbolTable.symbols[T->ptr[0]->place].type != T->ptr[1]->type)
                        semanticError(T->pos, "", "复合运算两边变量类型不一致");
                }
                else
                    semanticError(T->pos, "", "复合运算两边变量类型不一致");
            }
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            T->ir = mergeIR(2, T->ir, getIR(PLUSASS, opn1, opn2, result));
            break;
        case MINUSASS:
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            if (T->ptr[0]->kind != ID && T->ptr[0]->kind != Array_Call)
                semanticError(T->pos, "", "复合减操作对象不是变量");
            if (T->ptr[0]->type != T->ptr[1]->type)
            {
                if (T->ptr[0]->kind == Array_Call)
                {
                    if (symbolTable.symbols[T->ptr[0]->place].type != T->ptr[1]->type)
                        semanticError(T->pos, "", "复合运算两边变量类型不一致");
                }
                else
                    semanticError(T->pos, "", "复合运算两边变量类型不一致");
            }
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            T->ir = mergeIR(2, T->ir, getIR(MINUSASS, opn1, opn2, result));
            break;
        case NOT:
            Exp(T->ptr[0]);
            T->type = T->ptr[0]->type;
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            T->ir = mergeIR(2, T->ir, getIR(NOT, opn1, opn2, result));
            break;
        case UMINUS:
            Exp(T->ptr[0]);
            T->type = T->ptr[0]->type;
            break;
        case Array_Call:
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
            {
                semanticError(T->pos, "", "数组变量未定义");
            }
            //只能检查一维
            if (T->ptr[0]->type_int >= symbolTable.symbols[rtn].listLength[0])
            {
                semanticError(T->pos, "", "数组访问越界");
            }
            T->place = rtn;
            T->ir = NULL; //标识符不需要生成TAC
            result.kind = Array_Call;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            T->type = symbolTable.symbols[rtn].type;
            break;
        case FUNC_CALL: //根据T->type_id查出函数的定义，如果语言中增加了实验教材的read，write需要单独处理一下
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
            {
                semanticError(T->pos, T->type_id, "函数未定义");
                break;
            }
            if (symbolTable.symbols[rtn].flag != 'F'&&symbolTable.symbols[rtn].flag != 'D')
            {
                semanticError(T->pos, T->type_id, "不是一个函数");
                break;
            }
            T->type = symbolTable.symbols[rtn].type;
            if (T->ptr[0])
            {
                Exp(T->ptr[0]); //处理所有实参表达式求值，及类型
                T->ir = T->ptr[0]->ir;
            }
            else
            {
                T->ir = NULL;
            }
            if (T->ptr[0] == NULL && symbolTable.symbols[rtn].paramnum > 0)
            {
                semanticError(T->pos, "", "函数调用参数太少");
            }
            else
            {
                matchParam(rtn, T->ptr[0]); //处理所以参数的匹配
            }
            //处理参数列表的中间代码
            T0 = T->ptr[0];
            while (T0)
            {
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                T->ir = mergeIR(2, T->ir, getIR(ARG, opn1, opn2, result));
                T0 = T0->ptr[1];
            }
            T->place = fillTemp(newTempName(), LEV, T->type, 'T');
            opn1.kind = ID;
            strcpy(opn1.id, T->type_id); //保存函数名
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);
            T->ir = mergeIR(2, T->ir, getIR(CALL, opn1, opn2, result)); //生成函数调用中间代码
            break;
        case ARGS: //此处仅处理各实参表达式的求值的代码序列，不生成ARG的实参系列
            Exp(T->ptr[0]);
            T->ir = T->ptr[0]->ir;
            if (T->ptr[1])
            {
                Exp(T->ptr[1]);
                T->ir = mergeIR(2, T->ir, T->ptr[1]->ir);
            }
            break;
        }
    }
}

//语义分析，生成符号表
//对抽象语法树的先根遍历,按createAST的控制结构修改完成符号表管理和语义检查
void semanticAnalysis(struct node *T)
{
    int rtn, num;
    struct node *T0;
    struct opn opn1, opn2, result;
    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            if (!T->ptr[0])
                break;
            semanticAnalysis(T->ptr[0]); //访问外部定义列表中的第一个
            //ir是四元式部分
            T->ir = T->ptr[0]->ir;
            if (T->ptr[1])
            {
                semanticAnalysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
                T->ir = mergeIR(2, T->ir, T->ptr[1]->ir);
            }
            break;
        case EXT_VAR_DEF: //处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
            T->type = T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : !strcmp(T->ptr[0]->type_id, "float") ? FLOAT
                                                                                                                        : CHAR;
            dealExtvarlist(T->ptr[1]); //处理外部变量说明中的标识符序列
            T->ir = NULL;          //目前不支持全局变量初始化
            break;
        case FUNC_DEF: //填写函数定义信息到符号表
            T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : !strcmp(T->ptr[0]->type_id, "float") ? FLOAT
                                                                                                              : CHAR; //获取函数返回类型送到含函数名、参数的结点
                                                                                 
            if(T->ptr[2])//如果有函数体，即为函数定义，若没有函数体，只是函数声明
            {
                semanticAnalysis(T->ptr[1]); //处理函数名和参数结点部分，这里不考虑用寄存器传递参数
                strcpy(T->ptr[2]->Snext, newLabelName()); //函数体语句执行结束后的位置属性
                semanticAnalysis(T->ptr[2]);  //处理函数体结点
                T->ir = mergeIR(3, T->ptr[1]->ir, T->ptr[2]->ir, getLabelIR(T->ptr[2]->Snext));//函数体的代码作为函数的代码
            }
            else
            {
                T0=T->ptr[1];
                rtn=fillSymbolTable(T0->type_id, newValueName(), LEV, T0->type, 'D');//函数声明
                if(rtn==-1)
                {
                    semanticError(T->pos, T->type_id, "函数重复声明");
                }
                T->ir=NULL;
            }
            break;
        case FUNC_DEC:                                                        //根据返回类型，函数名填写符号表
            rtn = fillSymbolTable(T->type_id, newValueName(), LEV, T->type, 'F'); //函数不在数据区中分配单元，偏移量为0
            //semanticError报错部分，支持简单错误报错
            if (rtn == -1)
            {
                semanticError(T->pos, T->type_id, "函数重复定义");
                break;
            }
            else
            {
                T->place = rtn;
            }
            result.kind = ID;
            strcpy(result.id, T->type_id);
            T->ir = getIR(FUNCTION, opn1, opn2, result); //生成中间代码：FUNCTION 函数名
            if (T->ptr[0])
            {                                 //判断是否有参数
                semanticAnalysis(T->ptr[0]); //处理函数参数列表
                symbolTable.symbols[rtn].paramnum = T->ptr[0]->num;
                // T->ir = mergeIR(3, T->ptr[1]->ir, T->ptr[2]->ir, getLabelIR(T->ptr[2]->Snext)); //连接函数名和参数代码序列
                T->ir = mergeIR(2, T->ptr[0]->ir, getLabelIR(T->ptr[0]->Snext));
            }
            else
            {
                symbolTable.symbols[rtn].paramnum = 0;
            }
            break;
        case PARAM_LIST: //处理函数形式参数列表
            semanticAnalysis(T->ptr[0]);
            if (T->ptr[1])
            {
                semanticAnalysis(T->ptr[1]);
                T->num = T->ptr[0]->num + T->ptr[1]->num;             //统计参数个数
                T->ir = mergeIR(2, T->ptr[0]->ir, T->ptr[1]->ir); //连接参数代码
            }
            else
            {
                T->num = T->ptr[0]->num;
                T->ir = T->ptr[0]->ir;
            }
            break;
        case PARAM_DEC:
            rtn = fillSymbolTable(T->ptr[1]->type_id, newValueName(), 1, T->ptr[0]->type, 'P');//P 参数
            if (rtn == -1)
            {
                semanticError(T->ptr[1]->pos, T->ptr[1]->type_id, "参数名重复定义");
            }
            else
            {
                T->ptr[1]->place = rtn;
            }
            T->num = 1; //参数个数计算的初始值
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[rtn].alias);
            T->ir = getIR(PARAM, opn1, opn2, result); //生成：FUNCTION 函数名
            break;
        case COMP_STM:
            LEV++;
            //设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
            symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
            T->ir = NULL;
            if (T->ptr[0])
            {
                semanticAnalysis(T->ptr[0]); //处理该层的局部变量DEF_LIST
                T->ir = T->ptr[0]->ir;
            }
            if (T->ptr[1])
            {
                strcpy(T->ptr[1]->Snext, T->Snext); //S.next属性向下传递
                strcpy(T->ptr[1]->Bnext,T->Bnext);//如果有break的出口点
                strcpy(T->ptr[1]->Cnext,T->Cnext);//如果有continue语句
                semanticAnalysis(T->ptr[1]);       //处理复合语句的语句序列
                T->ir = mergeIR(2, T->ir, T->ptr[1]->ir);
            }
            if (OPTION == 's')
                printSymbol();                                              //c在退出一个符合语句前显示的符号表
                                                                           //  printSymbol();
            LEV--;                                                         //出复合语句，层号减1
            symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
            break;
        case DEF_LIST:
            T->ir = NULL;
            if (T->ptr[0])
            {
                semanticAnalysis(T->ptr[0]); //处理一个局部变量定义
                T->ir = T->ptr[0]->ir;
            }
            if (T->ptr[1])
            {
                semanticAnalysis(T->ptr[1]); //处理剩下的局部变量定义
                T->ir = mergeIR(2, T->ir, T->ptr[1]->ir);
            }
            break;
        case VAR_DEF: //处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                      //类似于上面的外部变量EXT_VAR_DEF，换了一种处理方法
            T->ir = NULL;
            T->ptr[1]->type = !strcmp(T->ptr[0]->type_id, "int") ? INT : !strcmp(T->ptr[0]->type_id, "float") ? FLOAT
                                                                                                              : CHAR; //确定变量序列各变量类型
            T0 = T->ptr[1];                                                                                           //T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
            num = 0;
            while (T0)
            { //处理所以DEC_LIST结点
                num++;
                T0->ptr[0]->type = T0->type; //类型属性向下传递
                if (T0->ptr[1])
                    T0->ptr[1]->type = T0->type;
                if (T0->ptr[0]->kind == ID)
                {
                    rtn = fillSymbolTable(T0->ptr[0]->type_id, newValueName(), LEV, T0->ptr[0]->type, 'V'); //此处偏移量未计算，暂时为0
                    if (rtn == -1)
                    {
                        semanticError(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
                    }
                    else
                    {
                        T0->ptr[0]->place = rtn;
                    }
                    symbolTable.symbols[rtn].isArray = 0;
                }
                else if (T0->ptr[0]->kind == Array)
                {
                    if (T0->ptr[0]->ptr[1]->type_int <= 0)
                    {
                        semanticError(T0->ptr[0]->pos, "", "数组长度不能等于0");
                    }
                    rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newValueName(), LEV, T0->ptr[0]->type, 'V');
                    //printf("%s\n", symbolTable.symbols[rtn].alias);
                    if (rtn == -1)
                    {
                        semanticError(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
                    }
                    else
                    {
                        T0->ptr[0]->place = rtn;
                    }
                    struct node *tmp = T0->ptr[0];
                    int plusWidth = 1;
                    int count = 0;
                    //多维数组的处理
                    while (tmp->ptr[0] != NULL)
                    {
                        plusWidth *= tmp->ptr[1]->type_int;
                        symbolTable.symbols[rtn].listLength[count] = tmp->ptr[1]->type_int;
                        tmp = tmp->ptr[0];
                    }
                    strcpy(symbolTable.symbols[rtn].name, tmp->type_id);
                    symbolTable.symbols[rtn].isArray = 1;
                }
                else if (T0->ptr[0]->kind == ASSIGNOP)
                {
                    rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newValueName(), LEV, T0->ptr[0]->type, 'V'); //此处偏移量未计算，暂时为0
                    if (rtn == -1)
                    {
                        semanticError(T0->ptr[0]->ptr[0]->pos, T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                    }
                    else
                    {
                        T0->ptr[0]->place = rtn;
                        Exp(T0->ptr[0]->ptr[1]);
                        opn1.kind = ID;
                        strcpy(opn1.id, symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                        result.kind = ID;
                        strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                        T->ir = mergeIR(3, T->ir, T0->ptr[0]->ptr[1]->ir, getIR(ASSIGNOP, opn1, opn2, result));
                    }
                }
                T0 = T0->ptr[1];
            }
            break;
        case STM_LIST:
            if (!T->ptr[0])
            {
                T->ir = NULL;
                break;
            }              //空语句序列
            strcpy(T->ptr[0]->Bnext,T->Bnext);
            strcpy(T->ptr[0]->Cnext,T->Cnext);//如果有continue语句
            if (T->ptr[1]) //2条以上语句连接，生成新标号作为第一条语句结束后到达的位置
            {
                strcpy(T->ptr[0]->Snext, newLabelName());
            }
            else //语句序列仅有一条语句，S.next属性向下传递
            {
                strcpy(T->ptr[0]->Snext, T->Snext);
            }
            semanticAnalysis(T->ptr[0]);
            T->ir = T->ptr[0]->ir;
            if (T->ptr[1])
            { //2条以上语句连接,S.next属性向下传递
                strcpy(T->ptr[1]->Snext, T->Snext);
                strcpy(T->ptr[1]->Bnext,T->Bnext);//如果有break的出口点
                strcpy(T->ptr[1]->Cnext,T->Cnext);//如果有continue语句
                semanticAnalysis(T->ptr[1]);
                //序列中第1条为表达式语句，返回语句，复合语句时，第2条前不需要标号
                if (T->ptr[0]->kind == RETURN || T->ptr[0]->kind == EXP_STMT || T->ptr[0]->kind == COMP_STM)
                {
                    T->ir = mergeIR(2, T->ir, T->ptr[1]->ir);
                }
                else
                {
                    T->ir = mergeIR(3, T->ir, getLabelIR(T->ptr[0]->Snext), T->ptr[1]->ir);
                }
            }
            break;
        case IF_THEN:
            strcpy(T->ptr[0]->Etrue, newLabelName()); //设置条件语句真假转移位置
            strcpy(T->ptr[0]->Efalse, T->Snext);
            boolExp(T->ptr[0]);
            strcpy(T->ptr[1]->Snext, T->Snext);
            strcpy(T->ptr[1]->Bnext,T->Bnext);//如果有break的出口点
            strcpy(T->ptr[1]->Cnext,T->Cnext);//如果有continue语句
            semanticAnalysis(T->ptr[1]); //if子句
            T->ir = mergeIR(3, T->ptr[0]->ir, getLabelIR(T->ptr[0]->Etrue), T->ptr[1]->ir);
            break;
        case IF_THEN_ELSE:
            strcpy(T->ptr[0]->Etrue, newLabelName()); //设置条件语句真假转移位置
            strcpy(T->ptr[0]->Efalse, newLabelName());
            boolExp(T->ptr[0]); //条件，要单独按短路代码处理
            strcpy(T->ptr[1]->Snext, T->Snext);
            strcpy(T->ptr[1]->Bnext,T->Bnext);//如果有break的出口点
            strcpy(T->ptr[1]->Cnext,T->Cnext);//如果有continue语句
            semanticAnalysis(T->ptr[1]); //if子句
            strcpy(T->ptr[2]->Snext, T->Snext);
            strcpy(T->ptr[2]->Bnext,T->Bnext);//如果有break的出口点
            strcpy(T->ptr[2]->Cnext,T->Cnext);//如果有continue语句
            semanticAnalysis(T->ptr[2]); //else子句
            T->ir = mergeIR(6, T->ptr[0]->ir, getLabelIR(T->ptr[0]->Etrue), T->ptr[1]->ir,
                            getGotoIR(T->Snext), getLabelIR(T->ptr[0]->Efalse), T->ptr[2]->ir);
            break;
        case WHILE:
            strcpy(T->ptr[0]->Etrue, newLabelName()); //真入口
            strcpy(T->ptr[0]->Efalse, T->Snext);//假出口
            boolExp(T->ptr[0]); //循环条件，要单独按短路代码处理
            strcpy(T->ptr[1]->Snext, newLabelName());//循环开始点
            strcpy(T->ptr[1]->Bnext,T->Snext);//如果有break的出口点
            strcpy(T->ptr[1]->Cnext,T->ptr[1]->Snext);//如果有continue语句
            semanticAnalysis(T->ptr[1]); //循环体
            T->ir = mergeIR(5, getLabelIR(T->ptr[1]->Snext), T->ptr[0]->ir,
                            getLabelIR(T->ptr[0]->Etrue), T->ptr[1]->ir, getGotoIR(T->ptr[1]->Snext));
            break;
        case EXP_STMT:
            strcpy(T->ptr[0]->Bnext,T->Bnext);//如果有break的出口点
            strcpy(T->ptr[0]->Cnext,T->Cnext);//如果有continue语句
            semanticAnalysis(T->ptr[0]);
            T->ir = T->ptr[0]->ir;
            break;
        case RETURN:
            if (T->ptr[0])
            {
                Exp(T->ptr[0]);
                //获取函数返回值类型
                num = symbolTable.index;
                do
                    num--;
                while (symbolTable.symbols[num].flag != 'F');
                if (T->ptr[0]->type != symbolTable.symbols[num].type)
                {
                    semanticError(T->pos, "返回值类型错误", "");
                    T->ir = NULL;
                    break;
                }
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                T->ir = mergeIR(2, T->ptr[0]->ir, getIR(RETURN, opn1, opn2, result));
            }
            else
            {
                result.kind = 0;
                T->ir = getIR(RETURN, opn1, opn2, result);
            }
            break;
        case ID:
        case INT:
        case FLOAT:
        case CHAR:
        case BREAK:
        case CONTINUE:
        case ASSIGNOP:
        case AND:
        case OR:
        case RELOP:
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
        case NOT:
        case UMINUS:
        case USELFPLUS:
        case MSELFPLUS:
        case USLEFMINUS:
        case MSLEFMINUS:
        case PLUSASS:
        case MINUSASS:
        case Array_Call:
        case FUNC_CALL:
            Exp(T); //处理基本表达式
            break;
        }
    }
}

void startSemanticAnalysis(struct node *T)
{
    //符号表，错误表初始化
    symbolTable.index = 0;
    errorTable.index = 0;
    symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
    symbol_scope_TX.top = 1;

    //生成抽象语法树.gv文件
    // fp=fopen("./tree.gv","w+");
    // if(fp==NULL)
    // {
    //     printf("文件打开失败\n");
    //     return;
    // }
    // fprintf(fp,"digraph G {\n");
    // createAST(T," ");
    // fprintf(fp,"}\n");
    // fclose(fp);
    // printf("\n***********************************************************************************\n");
    // //符号表
    // semanticAnalysis(T);
    // //四元式
    // printf("\n***********************************************************************************\n");
    // fp1=fopen("./flowGraph.gv","w+");
    // if(fp1==NULL)
    // {
    //     printf("文件打开失败\n");
    //     return;
    // }
    // fprintf(fp1,"digraph G {\n");
    // printIR(T->ir);
    // fprintf(fp1,"}\n");
    // fclose(fp1);
    // //错误信息
    // printf("\n***********************************************************************************\n");
    // prnErr();
    // return;

    //生成抽象语法树.gv文件ast
    if(OPTION=='a')
    {
        fp=fopen("./tree.gv","w+");
        if(fp==NULL)
        {
            printf("文件打开失败\n");
            return;
        }
        fprintf(fp,"digraph G {\n");
        createAST(T," ");
        fprintf(fp,"}\n");
        fclose(fp);
    }
    // printf("\n***********************************************************************************\n");
    //符号表sym
    if(OPTION=='s')
    {
        semanticAnalysis(T);
    }
    //四元式ir
    // printf("\n***********************************************************************************\n");
    if(OPTION=='i')
    {
        fp1=fopen("./flowGraph.gv","w+");
        if(fp1==NULL)
        {
            printf("文件打开失败\n");
            return;
        }
        fprintf(fp1,"digraph G {\n");
        semanticAnalysis(T);
        printIR(T->ir);
        fprintf(fp1,"}\n");
        fclose(fp1);
    }
    //错误信息
    printf("\n***********************************************************************************\n");
    prnErr();
}