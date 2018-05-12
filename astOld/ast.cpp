#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include "ast.h"

using namespace std;

namespace ast{
int t_idx = 0;
int l_idx = 0;

string temp(){
    stringstream ss;
    ss<<".t"<<t_idx++;
    return ss.str();

}

string label(){
    stringstream ss;
    ss<<".L"<<l_idx++;
    return ss.str();

}

void stmt::gen(){}

expr* expr::lvalue()
{
        cerr<<"Error: invalid lvalue\n";
        exit(1);
}
expr* expr::rvalue()
{
        return this;
} 
        
string expr::str(){
        return "Not implemented";
}


if_stmt::if_stmt(expr* e, stmt* s):left(e),right(s){
    l = label(); 
}

while_stmt::while_stmt(expr* e, stmt* s):left(e),right(s){
	begin = label();
    l = label(); 
}
dowhile_stmt::dowhile_stmt(expr* e, stmt* s):left(e),right(s){
    	begin = label();
	l = label(); 
}

void if_stmt::gen(){
    expr* e=left->rvalue();
    cout<<"ifFalse "<<e->str()<<" goto "<<l<<"\n";
    right->gen();
    cout<<l<<":\n";
}

void while_stmt::gen(){
	cout<<begin<<endl;
	expr* e = left->rvalue();
	cout<<"ifFalse "<<e->str()<<" goto "<<l<<endl;
	right->gen();
	cout<<"goto "<<begin<<endl;
	cout<<l<<endl;
}
void dowhile_stmt::gen(){
	cout<<begin<<endl;
	right->gen();
	expr* e = left->rvalue();
	cout<<"ifFalse "<<e->str()<<" goto "<<l<<endl;
	cout<<"goto "<<begin<<endl;
	cout<<l<<endl;
}

eval::eval(expr* e):left(e){}

void eval::gen(){
    left->rvalue();
}

seq::seq(stmt* l,  stmt* r):left(l),right(r) {}

void seq::gen(){
    if(left)
        left->gen();
    if(right)
        right->gen();
}

assign::assign(expr* l,  expr* r):left(l),right(r) {}

expr* assign::rvalue(){
    expr* l = left->lvalue();
    expr* r = right->rvalue();

    cout<<l->str()<<"="<<r->str()<<"\n";
    return r;

}

op::op(string t, expr* l, expr* r):type(t), left(l),right(r){}

expr* op::rvalue(){
    string t = temp();

    string e1 = left->rvalue()->str();
    string e2 = right->rvalue()->str();

    cout<<t<<"="<<e1<<type<<e2<<"\n";

    return new ast::id(t);
}


id::id(string name):lexeme(name) {}

expr* id::lvalue(){
    return this;
}

string id::str(){
    return lexeme;
}


num::num(int n):value(n) {}

string num::str(){
    stringstream t;
    t<<value;
    return t.str();
}
}
