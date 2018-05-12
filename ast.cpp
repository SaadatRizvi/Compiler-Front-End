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

label::label()
{
    stringstream ss;
    ss<<".L"<<l_idx++;
    l =  ss.str();
}

void label::gen()
{
    cout<<l<<":\n";
}

string label::str()
{
    return l;
}

stmt::stmt():l_next(){
}

void stmt::gen(){}

expr* expr::lvalue() {
        cerr<<"Error: invalid lvalue\n";
        exit(1);
}
expr* expr::rvalue() {
        return this;
} 
        
string expr::str(){
        return "Not implemented";
}


logical_expr::logical_expr(boolean_expr* l, boolean_expr *r, string o):left(l),right(r),op(o)
{ }

not_expr::not_expr(boolean_expr* l, string o):right(l),op(o)
{ }


void logical_expr::gen() {
    if (op == "&&"){
        left->l_true = label();
        left->l_false = l_false;

        right->l_true = l_true;
        right->l_false = l_false;

        left->gen();
        cout<<left->l_true.str()<<":";
        right->gen();
    }

	else if (op == "||"){

		left->l_true = l_true;
		left->l_false = label();
		
		right->l_true = l_true;
		right->l_false = l_false;

		left->gen();
        cout<<left->l_false.str()<<":";
        right->gen();
	}
}


void not_expr::gen() {
    if (op == "!"){
        right->l_true = l_false;
        right->l_false = l_true;

        right->gen();
    }
}


relational_expr::relational_expr(expr* l, expr* r, string ro):left(l),right(r),relop(ro)
{
}

void relational_expr::gen(){
    expr *el = left->rvalue();
    expr *er = right->rvalue();

    cout<<"if "<<el->str()<<" "<<relop<<" "<<er->str()<<" goto "<<l_true.str()<<"\n";
    cout<<"goto "<<l_false.str()<<"\n";
}


single_bool_expr::single_bool_expr(expr* r):right(r)
{
    if (right->type != "bool") {
        cerr<<"If statement can only process boolean values. Aborting\n";
        exit(1);
    }
}

void single_bool_expr::gen(){
    expr *er = right->rvalue();

    cout<<"if "<<er->str()<<" goto "<<l_true.str()<<"\n";
    cout<<"goto "<<l_false.str()<<"\n";
}





if_stmt::if_stmt(boolean_expr* e, stmt* s):left(e),right(s), l_true(){
    e->l_true = l_true;
    e->l_false = l_next;
    s->l_next = l_next;
}
while_stmt::while_stmt(boolean_expr* e, stmt* s):left(e),right(s), l_true(), l_temp(){
    e->l_true = l_true;
    e->l_false = l_next;
    s->l_next = l_temp;
}
dowhile_stmt::dowhile_stmt(boolean_expr* e, stmt* s):left(e),right(s), l_true(){
    e->l_true = l_true;
    e->l_false = l_next;
    s->l_next = l_next;
}
for_stmt::for_stmt(stmt * a,boolean_expr* e, stmt * a2, stmt* s):a(a),left(e),a2(a2),right(s), l_true(), l_temp(){
    e->l_true = l_true;
    e->l_false = l_next;
    s->l_next = l_temp;
}
void for_stmt::gen(){
    a->gen();
    cout<<l_temp.str()<<":";
    left->gen();
    cout<<l_true.str()<<":";
    right->gen();
    a2->gen();
    cout<<"goto "<<l_temp.str()<<"\n";

    cout<<l_next.str()<<":";
}

void if_stmt::gen(){
    left->gen();
    cout<<l_true.str()<<":";
    right->gen();
    cout<<l_next.str()<<":";
}


void while_stmt::gen(){
    cout<<l_temp.str()<<":";
    left->gen();
    cout<<l_true.str()<<":";
    right->gen();
    cout<<"goto "<<l_temp.str()<<"\n";

    cout<<l_next.str()<<":";
}
void dowhile_stmt::gen(){
    cout<<l_true.str()<<":";
    right->gen();
    left->gen();
    cout<<l_next.str()<<":";
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

    if (r->type == l->type) {
        cout<<l->str()<<"="<<r->str()<<"\n";
    }
     else if (r->type == "char" && l->type == "string"){
        string t = temp();
        cout<<t<<"="<<r->str()<<"\n";
        cout<<l->str()<<"= string("<<t<<")\n";
    }
    else if (r->type == "bool" && l->type == "float"){
        string t = temp();
        cout<<t<<"="<<r->str()<<"\n";
        cout<<l->str()<<"= float("<<t<<")\n";
    }
    else if (r->type == "int" && l->type == "float"){
        string t = temp();
        cout<<t<<"="<<r->str()<<"\n";
        cout<<l->str()<<"= float("<<t<<")\n";
    }
    else if (r->type == "bool" && l->type == "int"){
        string t = temp();
        cout<<t<<"="<<r->str()<<"\n";
        cout<<l->str()<<"= int("<<t<<")\n";
    }
    else{
        cerr<<"Invalid conversion from "<<r->type<<" to "<<l->type<<". Aborting\n";
        exit(1);
    }
    return r;

}

op::op(string k, expr* l, expr* r):kind(k), left(l),right(r){
    //compute type for op node given types for left and right.
    if (l->type == "float" || r->type == "float") {
        type = "float";
    }
    else if (l->type == "int" || r->type == "int") {
        type = "int";
    }
    else {
        type = "bool";
    }
}

expr* op::rvalue(){
    string t = temp();

    string e1 = left->rvalue()->str();
    string e2 = right->rvalue()->str();

    //type coercion
    if (left->type != right->type){
        if (left->type != type) {
            t = temp();
            cout <<t<<"= "<<type<<"(" <<e1<<")\n"; 
            e1 = t;
        }
        if (right->type != type) {
            t = temp();
            cout <<t<<"= "<<type<<"(" <<e2<<")\n"; 
            e2 = t;
        }
    }

    cout<<t<<"="<<e1<<kind<<e2<<"\n";

    return new ast::id(t, type);
}

id::id(string name, string _type):lexeme(name) { type = _type;}

expr* id::lvalue(){
    return this;
}

string id::str(){
    return lexeme;
}


num::num(int n, string _type):value(n) { type = _type;}

string num::str(){
    stringstream t;
    t<<value;
    return t.str();
}


boolnum::boolnum(string n, string _type):value(n) { type = _type;}

string boolnum::str(){
    stringstream t;
    t<<value;
    return t.str();
}
}