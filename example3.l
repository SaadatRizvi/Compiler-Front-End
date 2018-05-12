%option noyywrap

%{ 
#include<iostream>
#include<cstdlib>
#include<string>
#include<sstream>
#include "ast.h"
#include "example3.tab.h"

using namespace std;
int line=0;
%}

alpha [a-zA-Z_]
alphanum [0-9a-zA-Z_]
digit [0-9]

%%
int {yylval.type = new string(yytext); return T_INT;}
float {yylval.type = new string(yytext); return T_FLOAT;}
bool {yylval.type = new string(yytext); return T_BOOL;}
char {yylval.type = new string(yytext); return T_CHAR;}
string {yylval.type = new string(yytext); return T_STRING;}


true {yylval.relop = new string(yytext); return BOOLNUM; }
false {yylval.relop = new string(yytext); return BOOLNUM; }

[ \t]+ ;
\n  { line++;}

\&\& { return T_AND;}
\|\| { return T_OR;}
\!   {return T_NOT;}

\<|\>|\<=|\>=|==|\!= { yylval.relop = new string(yytext); return T_RELOP; } 

if  { return T_IF;}
while  { return T_WHILE;}
do  { return T_DOWHILE;}
for  { return T_FOR;}


{alpha}{alphanum}*  { yylval.id = new string(yytext); return T_ID;}
{digit}+    {
            yylval.value = atoi(yytext); 
            return NUM; }
.   { return *yytext;}
%%



//FOR FLOAT VALUES
// [0-9]+(\.[0-9]+)?  { 
//             /* yylval is a union */
//             yylval.value = atof(yytext);  
//              return NUM;
//             }