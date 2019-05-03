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

bool isNullable( vector<bool> &nullables,int var){
    if(var >= nullables.size()) return false;
    return nullables[var];
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

set<tokens> first(int var,const vector< set<tokens> > &firstCollection){
    if(isToken(var)){
        set<tokens> firstSet;
        firstSet.insert((tokens)var);
        return firstSet;
    }
    return firstCollection[var];
}

set<tokens> first(int index,const vector<int> &sentenial){
    set<tokens> firstSentenial;
    if(index>=sentenial.size()) return firstSentenial;
    vector< set<tokens> > firstCollection= first();
    vector<bool> nullables = getNullable();
    firstSentenial=first(sentenial[index],firstCollection);
    for(int i=index;i<sentenial.size()-1 && isNullable(nullables,sentenial[i]);i++){
        firstSentenial= unionSet(firstSentenial,first(sentenial[i+1],firstCollection));
    }
    return firstSentenial;
}

bool isSentenialNullable(int index,const vector<int> &sentenial){
    vector<bool> nullables = getNullable();
    for(int i=index;i<sentenial.size();i++){
        if(isToken(sentenial[i]) || !nullables[sentenial[i]]){
            return false;
        }
    }
    return true;
}
/**
 * The function return the index of the var in a rule.
 * if the var does not exist on that side the funciton will return -1;
**/
int getIndexOfRightHandSide(int var,grammar_rule rule){
    for(int i=0;i<rule.rhs.size();i++){
        if(rule.rhs[i]==var)
            return i;
    }
    return -1;
}

vector< set<tokens> > follow(){
    vector< set<tokens> > followCollection(NONTERMINAL_ENUM_SIZE);
    followCollection[S].insert(EF);
    bool isChanged;
    do{
        isChanged=false;
        for(int var=S;var<NONTERMINAL_ENUM_SIZE;var++){
            int varSetSize=followCollection[var].size();
            for(int rule=0;rule<grammar.size();rule++){
                int varIndex=getIndexOfRightHandSide(var,grammar[rule]);
                if(0<=varIndex){
                    int sentenialIndex=varIndex+1;
                    vector<int> &rhs=grammar[rule].rhs;
                    followCollection[var]=unionSet(followCollection[var],first(sentenialIndex,rhs));
                    if(isSentenialNullable(sentenialIndex,rhs)){
                        followCollection[var]=unionSet(followCollection[var],followCollection[grammar[rule].lhs]);
                    }
                }
            }
            if(followCollection[var].size()>varSetSize)
                isChanged=true;
        }
    }while(isChanged);
    return followCollection;
}

vector< set<tokens> > select(){
    vector< set<tokens> > followCollection = follow();
    vector< set<tokens> > selectCollection(grammar.size());
    for(int rule=0; rule<selectCollection.size() ; rule++){
        if(isSentenialNullable(0,grammar[rule].rhs)){
            selectCollection[rule] = unionSet(selectCollection[rule],followCollection[grammar[rule].lhs]);
        }
        selectCollection[rule] = unionSet(selectCollection[rule], first(0,grammar[rule].rhs));
    }

    return selectCollection;
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
    print_follow(follow());
}

/**
 * computes select for all grammar rules (see grammar global variable in grammar.h)
 * calls print_select when finished
 */
void compute_select(){
    print_select(select());
}

/**
 * implements an LL(1) parser for the grammar using yylex()
 */
void parser(){
    exit(1);
}

