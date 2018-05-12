%{
#include<cstdio>
#include<iostream>
#include<math.h>
#include<string>
#include<sstream>
#include "ast.h"
#include "symtab.h"


using namespace std;

int yylex(void);
extern FILE* yyin;
void yyerror(const char*);
string Temp();

int var = 0;
sym_tab* top = new sym_tab(NULL);
sym_tab* saved;


%}

/* type of yylval is now union with a single float inside */
/* %union redefines the type of the attributes */
%union{
    int value;
    std::string* type;
    std::string* id;
    std::string* relop;
    ast::expr* pe;
    ast::boolean_expr* be;
    ast::stmt* ps;
    std::string* boolValue;
};


%token T_IF
%token T_AND
%token T_OR
%token T_NOT
%token T_WHILE
%token T_DOWHILE
%token T_FOR



%token <relop> T_RELOP
%token <id> T_ID
%token <value> NUM
%token <type> T_FLOAT
%token <type> T_BOOL
%token <type> T_INT
%token <type> T_CHAR
%token <type> T_STRING

%token <boolValue> BOOLNUM



/* The types of tokens and non-terminals are chosen from the yylval union. */

%type <pe> expr
%type <ps> A
%type <ps> S
%type <be> B
%type <ps> Seq




%left '+' '-'
%left '*' '/'
%right '^'
%left T_AND
%left T_OR
%left T_NOT

%%
P : Seq {
        //walk syntax tree to generate code
        $1->gen();
    }

Seq : Seq S { $$ = new ast::seq($1, $2); }
  | {$$ = NULL; }

OpenBlock : '{' {  
                saved = top; 
                top = new sym_tab(saved); 
        } 

CloseBlock : '}' { 
                delete top; 
                top = saved; 
        } 
         
S : A { $$=$1;}
  | D {$$ = NULL;}
  | T_IF '(' B ')' OpenBlock Seq CloseBlock {
                        $$ = new ast::if_stmt($3, $6);
                    }
  | T_WHILE '(' B ')' OpenBlock Seq CloseBlock {
				$$ = new ast::while_stmt($3, $6);
			}
  | T_DOWHILE OpenBlock Seq CloseBlock T_WHILE '(' B ')' {
			
					$$ = new ast::dowhile_stmt($7,$3);
					}
  | T_FOR '(' A  B';' A ')' OpenBlock Seq CloseBlock {
			
					$$ = new ast::for_stmt($3, $4, $6, $9);
					}

;
D : T_INT T_ID ';' { 
                        sym_ent* s = new sym_ent;
                        s->type = *$1; 
                        if(!top->put(*$2, s)){
                            cerr<< "Error: Multiple declarations of "<<*$2<<"\n"; //" 
                       }
                    }
  | T_FLOAT T_ID ';' {
                        sym_ent* s = new sym_ent;
                        s->type = *$1; 
                        if(!top->put(*$2, s)){
                            cerr<<"Error: Multiple declarations of "<<*$2<<"\n";
                        }
                    }
  | T_CHAR T_ID ';' {
                        sym_ent* s = new sym_ent;
                        s->type = *$1; 
                        if(!top->put(*$2, s)){
                            cerr<<"Error: Multiple declarations of "<<*$2<<"\n";
                        }
                    }
  | T_STRING T_ID ';' {
                        sym_ent* s = new sym_ent;
                        s->type = *$1; 
                        if(!top->put(*$2, s)){
                            cerr<<"Error: Multiple declarations of "<<*$2<<"\n";
                        }
                    }
  | T_BOOL T_ID ';' {
                        sym_ent* s = new sym_ent;
                        s->type = *$1; 
                        if(!top->put(*$2, s)){
                            cerr<<"Error: Multiple declarations of "<<*$2<<"\n";
                        }
                    }

;
A : T_ID '=' expr ';' {
                    sym_ent* s = top->get(*$1);
                    if(!s){
                        cerr<<"Undeclared variable "<<*$1<<"\n";
                        exit(1);
                    }

                    ast::id *t = new ast::id(*$1, s->type);
                    ast::expr* p = new ast::assign(t, $3);
                    $$ = new ast::eval(p);
                    }
;

expr: expr '+' expr {
                    
                    $$ = new ast::op("+", $1, $3);
                    }  
    | expr '-' expr {
                    $$ = new ast::op("-", $1, $3);
                    
                    }
    | expr '*' expr {
                    $$ = new ast::op("*", $1, $3);
                    
                    }
    | expr '/' expr {
                    $$ = new ast::op("/", $1, $3);                 
                    }
    | expr '^' expr {
                    $$ = new ast::op("^", $1, $3);                 
                    }
    | NUM           { 
                    $$ = new ast::num($1, "int");               
                    }
    | BOOLNUM       { 
                    $$ = new ast::boolnum(*$1, "bool");               
                    }

    |  T_ID          { 
                    sym_ent* s = top->get(*$1);
                    if(!s){
                        cerr<<"Undeclared variable "<<*$1<<"\n";
                        exit(1);
                    }
                    
                    $$ = new ast::id(*$1, s->type);               
                    }


    | '(' expr ')' {
                    $$ = $2;
                    }

B: B T_AND B { $$ = new ast::logical_expr($1, $3, "&&"); }

    |  B T_OR B { $$ = new ast::logical_expr($1, $3, "||"); }

    | T_NOT '(' B ')' { $$ = new ast::not_expr($3, "!"); }

    | expr T_RELOP expr {$$ = new ast::relational_expr($1, $3, *$2); }
    | expr {$$ = new ast::single_bool_expr($1); }
    ;
%% 

//Creates a new temporary variable such as ".L5"
string Temp(){
    stringstream t;
    t<<".L"<<var;
    var++;
    return t.str();
}

void yyerror (const char *error)
{
  std::cout << error << '\n';
}


int main(int argc, char *argv[]){
    if(argc==2)
        yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}


//    | '(' expr ')'  { 
//                         $$ = new ast::eval($2);
//                       }