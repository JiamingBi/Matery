%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "option.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
void createAST(struct node *,char *);
int TypeCheck(char* type);
%}

%union {
	int    type_int;
	float  type_float;
        char type_char;
	char   type_id[32];
	struct node *ptr;
};//实现多元支持


//%type 定义【非终结符】的语义值类型
%type  <ptr> program ExtDefList  ExtDef        Specifier ExtDecList     FuncDec             CompSt   VarList  VarDec   ClassDef
//           开始符  外部定义列表 外部函数与变量 类型标识符 外部变量名列表 函数变量名和参数表列  复合语句 参数列表  变量名或函数名
//

%type <ptr>  ParamDec  Stmt     StmList       DefList       Def          DecList              Dec                Exp         Args            List_INT 
//           形参       单条语句  语句列表      定义语句列表  定义语句      单条语句定义多个变量  定义变量名和初始化   运算表达式  调用时参数列表   辅助数组支持
//

/*---------------------1.Tokens---------------------*/
//% token 定义【终结符】的语义值类型
%token <type_int> INT              //指定INT的语义值是type_int，有词法分析得到的数值  
//                整数
%token <type_id> ID RELOP TYPE  //指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串 
//           变量名  >,<,==...  int,float,char   
%token <type_float> FLOAT         //指定ID的语义值是type_id，有词法分析得到的标识符字符串 
//                  浮点数
%token <type_char> CHAR   
//                 字符串
%token LP RP LC RC LB RB SEMI COMMA  COLON VAR DEF ZX//用bison对该文件编译时，带参数-d，生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
//     (  )  {  }  [  ]    ;    ,     
%token PLUS PLUSASS MINUS MINUSASS STAR DIV ASSIGNOP AND OR NOT IF ELSE WHILE RETURN SELFPLUS SLEFMINUS FOR BREAK CONTINUE  CLASS
//      +     +=      -     -=      *    /    =       &&  || !  if  else  while  return  ++      --     for  break continue

%left ASSIGNOP
%left OR
%left AND
%left RELOP
%left SELFPLUS SLEFMINUS
%left MINUSASS PLUSASS
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT 

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%
/*---------------------2.High-level Definitions---------------------*/
/*初始语法单元，表示整个程序*/
program: ExtDefList{startSemanticAnalysis($1);}; 

/*零个或多个ExtDef*/
ExtDefList: {$$=NULL;} 
        | ExtDef ExtDefList {$$=makeNode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}
        ; 

/*表示全局变量，结构体，函数声明，函数定义*/
ExtDef: Specifier ExtDecList SEMI {$$=makeNode(EXT_VAR_DEF,$1,$2,NULL,yylineno);}   //全局变量
        //|Specifier SEMI {} //结构体当前未实现
        |DEF FuncDec ZX Specifier CompSt {$$=makeNode(FUNC_DEF,$2,$4,$5,yylineno);}         //函数定义
        |DEF FuncDec CompSt {$$=makeNode(FUNC_DEF,$2,NULL,$3,yylineno);} 
        | error SEMI   {$$=NULL; }
        |Specifier FuncDec SEMI {$$=makeNode(FUNC_DEF,$1,$2,NULL,yylineno);}//函数声明
        | CLASS ClassDef CompSt {$$=makeNode(CLASS_DEF,$2,$3,NULL,yylineno);} 
        ;
 
/*零个或多个变量的定义VarDec*/
ExtDecList: VarDec {$$=$1;} 
        | VarDec COMMA ExtDecList {$$=makeNode(EXT_DEC_LIST,$1,$3,NULL,yylineno);}
        ; 


/*---------------------3.Specifiers---------------------*/
/*类型描述符,如int float char*/
Specifier: TYPE {$$=makeNode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);$$->type=TypeCheck($1);} 
        //|结构体
        ;    


/*---------------------4.Declarators---------------------*/
/*对一个变量的定义*/ 
VarDec:  ID {$$=makeNode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}   //标识符，如a
        | VarDec LB List_INT RB        {$$=makeNode(Array,$1,$3,NULL,yylineno);} //数组，如a[10]
        ;

/*用于辅助数组文法*/
List_INT: INT {$$=makeNode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}
        ;

/*函数头的定义*/
FuncDec: ID LP VarList RP  {$$=makeNode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//含参数函数
        |ID LP  RP   {$$=makeNode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//无参数函数
        ;  

ClassDef:ID    {$$=makeNode(CLASS_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//无参数函数
        ;

/*参数列表*/
VarList: ParamDec  {$$=makeNode(PARAM_LIST,$1,NULL,NULL,yylineno);}//一个形参的定义
        | ParamDec COMMA  VarList  {$$=makeNode(PARAM_LIST,$1,$3,NULL,yylineno);}
        ;

/*一个形参的定义*/
ParamDec: Specifier VarDec         {$$=makeNode(PARAM_DEC,$1,$2,NULL,yylineno);}
        ;



/*---------------------5.Statements---------------------*/
/*花括号括起来的语句块*/
CompSt: LC DefList StmList RC    {$$=makeNode(COMP_STM,$2,$3,NULL,yylineno);}
        ;

/*一系列语句列表*/
StmList: {$$=NULL; }  
        | Stmt StmList  {$$=makeNode(STM_LIST,$1,$2,NULL,yylineno);}
        ;

/*单条语句*/
Stmt: Exp SEMI    {$$=makeNode(EXP_STMT,$1,NULL,NULL,yylineno);}//一条表达式
      | CompSt      {$$=$1;}      //另一个语句块
      | RETURN Exp SEMI   {$$=makeNode(RETURN,$2,NULL,NULL,yylineno);}//返回语句
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=makeNode(IF_THEN,$3,$5,NULL,yylineno);} //if语句
      | IF LP Exp RP Stmt ELSE Stmt   {$$=makeNode(IF_THEN_ELSE,$3,$5,$7,yylineno);}//if-else 语句
      | WHILE LP Exp RP Stmt {$$=makeNode(WHILE,$3,$5,NULL,yylineno);}//while 语句
      | FOR LP Exp SEMI Exp SEMI Exp RP Stmt {$$=makeNode(FOR,$3,$5,$9,yylineno);}//while 语句     
      ;

/*---------------------6.Local Definitions---------------------*/
/*一系列变量定义，如int a;float b;int d[10]*/
DefList: {$$=NULL; }
        | Def DefList {$$=makeNode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;

/*一个变量定义*/
Def: VAR  DecList COLON Specifier SEMI {$$=makeNode(VAR_DEF,$2,$4,NULL,yylineno);}
     ;
     
/*如int a,b,c*/
DecList: Dec  {$$=makeNode(DEC_LIST,$1,NULL,NULL,yylineno);}
        | Dec COMMA DecList  {$$=makeNode(DEC_LIST,$1,$3,NULL,yylineno);}
	;

/*变量名或者带初始化*/
Dec:   VarDec {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=makeNode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
       ;



/*---------------------7.Expressions---------------------*/
/*运算表达式*/
Exp:  //二元运算
        Exp ASSIGNOP Exp {$$=makeNode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//二元运算
      | Exp AND Exp   {$$=makeNode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}//逻辑与
      | Exp OR Exp    {$$=makeNode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}//逻辑或
      | Exp RELOP Exp {$$=makeNode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}//关系表达式
      | Exp PLUS Exp  {$$=makeNode(PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUS");}//四则运算
      | Exp MINUS Exp {$$=makeNode(MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS");}
      | Exp STAR Exp  {$$=makeNode(STAR,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=makeNode(DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV");}
      //-------额外实现-------
      | Exp PLUSASS Exp  {$$=makeNode(PLUSASS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUSASS");} //复合加
      | Exp SELFPLUS      {$$=makeNode(USELFPLUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SELFPLUS");}  //自增
      | SELFPLUS Exp     {$$=makeNode(MSELFPLUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"SELFPLUS");}  //自增
      | Exp MINUSASS Exp {$$=makeNode(MINUSASS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUSASS");} //复合减
      | Exp SLEFMINUS  {$$=makeNode(USLEFMINUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"SLEFMINUS");} //自减
      | SLEFMINUS Exp {$$=makeNode(MSLEFMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"SLEFMINUS");} //自减
      //-------以上-------

      //一元运算
      | LP Exp RP     {$$=$2;}//括号表达式
      | MINUS Exp %prec UMINUS   {$$=makeNode(UMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"UMINUS");}//取负
      | NOT Exp       {$$=makeNode(NOT,$2,NULL,NULL,yylineno);strcpy($$->type_id,"NOT");}//逻辑或
      
      //不包含运算符，较特殊的表达式
      | ID LP Args RP {$$=makeNode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}  //函数调用(含参)
      | ID LP RP      {$$=makeNode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数调用(无参)
      | ID LB List_INT RB  {$$=makeNode(Array_Call,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//数组索引调用
      
      //最基本表达式
      | ID            {$$=makeNode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//普通变量
      | INT           {$$=makeNode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}      //整常数常量
      | FLOAT         {$$=makeNode(FLOAT,NULL,NULL,NULL,yylineno);$$->type_float=$1;$$->type=FLOAT;} //浮点数常量
      | CHAR          {$$=makeNode(CHAR,NULL,NULL,NULL,yylineno);$$->type_char=$1;$$->type=CHAR;} //字符常量
      | BREAK         {$$=makeNode(BREAK,NULL,NULL,NULL,yylineno);}
      | CONTINUE      {$$=makeNode(CONTINUE,NULL,NULL,NULL,yylineno);}
      ;

/*实参列表*/
Args:  Exp COMMA Args    {$$=makeNode(ARGS,$1,$3,NULL,yylineno);}
       | Exp               {$$=makeNode(ARGS,$1,NULL,NULL,yylineno);}
       ;
       
%%

int TypeCheck(char* type)
{
    if (!strcmp(type,"int"))
	{
		return INT;
	}
	if (!strcmp(type,"float"))
	{
		return FLOAT;
	}
	if (!strcmp(type,"char"))
	{
		return CHAR;
	}
}



#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}