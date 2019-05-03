#include <stdlib.h>
#include "hw2.h"

using namespace std;

bool isToken(int token){
    return token>=IMPORTANT;
}
bool isRhsNullable(grammar_rule rule,vector<bool> &nullableVector){
    for(int i=0;i<rule.rhs.size();i++){
        int var=rule.rhs[i];
        if(isToken(var))
            return false;
        if(!nullableVector[var])
            return false;
    }
    return true;
}

vector<bool> getNullable(){
    vector<bool> nullables(NONTERMINAL_ENUM_SIZE,false);
    for(int i=0;i<=grammar.size();i++){
        for(int j=0;j<grammar.size();j++){
            grammar_rule rule=grammar[j];
            if(isRhsNullable(rule,nullables)){
                nullables[rule.lhs]=true;
            }

        }
    }

    return nullables;
}

void compute_nullable(){
    print_nullable(getNullable());
}

set<tokens> unionSet(const set<tokens> &s1,const set<tokens> &s2){
    set<tokens> uSet;
    set<tokens>::iterator it;
    for (it = s1.begin(); it != s1.end(); ++it) {
          uSet.insert(*it);
     }
    for (it = s2.begin(); it != s2.end(); ++it) {
          uSet.insert(*it);
     }

     return uSet;
}

bool isNullable( vector<bool> &nullables,int index){
    if(index >= nullables.size()) return false;
    return nullables[index];
}


vector< set<tokens> > first(){
    vector< set<tokens> > firstCollection(NONTERMINAL_ENUM_SIZE);
    vector<bool> nullables = getNullable();
    bool isChanged;
    do{
        isChanged = false;
        for(int i=0 ; i < grammar.size() ; i++){
            int left = grammar[i].lhs;
            if(grammar[i].rhs.size() == 0) continue;
            int right = grammar[i].rhs[0];
            int lSize = firstCollection[left].size();
            if(isToken(right)){
                firstCollection[left].insert((tokens)right);
            }else{
                firstCollection[left] = unionSet(firstCollection[left],firstCollection[right]);
                
                for(int j =1 ;j < grammar[i].rhs.size() && isNullable(nullables, right) ; j++){
                    right = grammar[i].rhs[j];
                    if(isToken(right)){
                        firstCollection[left].insert((tokens)right);

                    }else{
                        firstCollection[left] = unionSet(firstCollection[left],firstCollection[right]);
                    }
                    
                }
            }
            if(lSize != firstCollection[left].size())
                isChanged = true;
        }
    }while(isChanged);

    return firstCollection;
}


/**
 * computes first for all nonterminal (see nonterminal enum in grammar.h)
 * calls print_first when finished
 */
void compute_first(){
    print_first(first());
}

/**
 * computes follow for all nonterminal (see nonterminal enum in grammar.h)
 * calls print_follow when finished
 */
void compute_follow(){
    exit(1);
}

/**
 * computes select for all grammar rules (see grammar global variable in grammar.h)
 * calls print_select when finished
 */
void compute_select(){
    exit(1);
}

/**
 * implements an LL(1) parser for the grammar using yylex()
 */
void parser(){
    exit(1);
}

