#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>

#ifndef _AST_H
#define _AST_H
using namespace std;

namespace ast{
string temp();


class label{
    public: 

    string l;

    //string new_label();
    label();
    void gen();
    string str();

};

class node{
};

class stmt:public node{
    public:
        //label l;
        stmt();
        label l_next;
        virtual void gen();
};

class expr: public node{
    public:
        string type;
        virtual expr* lvalue();
        virtual expr* rvalue();       
        virtual string str();
};

class boolean_expr: public node{
    public:
        label l_true;
        label l_false;

        virtual void gen() = 0;
};

class logical_expr: public boolean_expr{
    public:
        boolean_expr* left;
        boolean_expr* right;

        string op;

        logical_expr(boolean_expr* l, boolean_expr* r, string o);
        void gen();

};
class single_bool_expr: public boolean_expr{
    public:
        expr* right;

        single_bool_expr(expr* r);
        void gen();

};


class not_expr: public boolean_expr{
    public:
        boolean_expr* right;

        string op;

        not_expr(boolean_expr* l, string o);
        void gen();

};

class relational_expr: public boolean_expr{
    public:
        expr* left;
        expr* right;
        string relop;

        relational_expr(expr* l, expr* r, string ro);
        void gen();

};



class if_stmt: public stmt{
    public:
        boolean_expr *left;
        stmt *right;
        label l_true;

        if_stmt(boolean_expr* e, stmt* s);
        void gen();
};

class while_stmt: public stmt{
    public:
        boolean_expr *left;
        stmt *right;
        label l_true;
        label l_temp;

        while_stmt(boolean_expr* e, stmt* s);
        void gen();
};
class dowhile_stmt: public stmt{
    public:
        boolean_expr *left;
        stmt *right;
        label l_true;

        dowhile_stmt(boolean_expr* e, stmt* s);
        void gen();
};

class for_stmt: public stmt{
    public:
        stmt * a;
        boolean_expr *left;
        stmt * a2;
        stmt *right;
        label l_true;
        label l_temp;


        for_stmt(stmt * a,boolean_expr* e, stmt * a2, stmt* s);
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

    string kind;
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

        id(string name, string);

        expr* lvalue();
        string str();
};


class num:public expr{
    public:
        int value;

        num(int n, string type);
        string str();
};


class boolnum:public expr{
    public:
        string value;

        boolnum(string n, string type);
        string str();
};
}
#endif
