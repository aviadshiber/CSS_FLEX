#include <stdlib.h>
#include "hw2.h"

bool isToken(int token){
    return token>=IMPORTANT;
}
bool isRhsNullable(grammar_rule rule,std::vector<bool> &nullableVector){
    for(int i=0;i<rule.rhs.size();i++){
        int var=rule.rhs[i];
        if(isToken(var))
            return false;
        if(!nullableVector[var])
            return false;
    }
    return true;
}

void compute_nullable(){
    std::vector<bool> nullables(NONTERMINAL_ENUM_SIZE,false);
    for(int i=0;i<=grammar.size();i++){
        for(int j=0;j<grammar.size();j++){
            grammar_rule rule=grammar[j];
            if(isRhsNullable(rule,nullables)){
                nullables[rule.lhs]=true;
            }

        }
    }
    print_nullable(nullables);
}


/**
 * computes first for all nonterminal (see nonterminal enum in grammar.h)
 * calls print_first when finished
 */
void compute_first(){
    exit(1);
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

