#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>

#ifndef _AST_H
#define _AST_H
using namespace std;

namespace ast{
string temp();

string label();

class node{
};

class stmt:public node{
    public:
    virtual void gen();
};

class expr: public node{
    public:
        virtual expr* lvalue();
        virtual expr* rvalue();       
        virtual string str();
};

class if_stmt: public stmt{
    public:
        expr *left;
        stmt *right;
        string l;

        if_stmt(expr* e, stmt* s);
        void gen();
};

class while_stmt: public stmt{
    public:
        expr *left;
        stmt *right;
        string l;
		string begin;

        while_stmt(expr* e, stmt* s);
        void gen();
};
class dowhile_stmt: public stmt{
    public:
        expr *left;
        stmt *right;
        string l;
		string begin;

        dowhile_stmt(expr* e, stmt* s);
        void gen();
};

class eval: public stmt{
    public:
        expr* left;

        eval(expr* e);
        void gen();
};

class seq: public stmt{
    public:
        stmt* left;
        stmt* right;

        seq(stmt* l,  stmt* r);

        void gen();
};


class op: public expr{
    public:

    string type;
    expr *left, *right;

    op(string, expr*, expr*);
    expr* rvalue();
};


class assign:public expr{
    public:
        expr *left, *right;

        assign(expr* l,  expr* r);
        expr* rvalue();
};

class id:public expr{
    public:
        string lexeme;

        id(string name);

        expr* lvalue();
        string str();
};


class num:public expr{
    public:
        int value;

        num(int n);
        string str();
};
}
#endif
