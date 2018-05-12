#include<iostream>
#include<string>
#include<map>

using namespace std;

//symbol table entry
struct sym_ent{
    string type;
};

class sym_tab{
    public:
    //The symbol table is a C++ map (hash table)
    map<string, sym_ent*> table;
    //Tables are chained in a linked list
    sym_tab* prev;

    sym_tab(sym_tab* p){
        prev = p;
    }
    
    sym_tab* get_prev(){
        return prev;
    }

    bool put(string s, sym_ent* sym){
        if(table.count(s) == 0){
            table[s] = sym;
            return true;
        }
        return false;
    }


    sym_ent* get(string s){
        for(sym_tab* i = this; i!=NULL; i=i->prev){
            if(i->table.count(s) > 0)
                return i->table[s];
        }
        return NULL;
    }

    //delete table entries in destructor
    ~sym_tab(){
        for(map<string, sym_ent*>::iterator x=table.begin();
            x!=table.end(); x++){
            delete (*x).second;
        }
    }
};


