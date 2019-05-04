#! /usr/bin/env python3
from nltk import CFG, ChartParser
from random import choice

rules = []

def produce(grammar, symbol):
    words = []
    productions = grammar.productions(lhs = symbol)
    production = choice(productions)
    rules.append(grammar.productions().index(production) + 1)
    for sym in production.rhs():
        if isinstance(sym, str):
            words.append(sym)
        else:
            words.extend(produce(grammar, sym))
    return words

	
grammar = CFG.fromstring("""
 S -> LImports LRules
 LImports -> Import LImports | 
 Import -> '@import' '"string"' ';'
 LRules -> Rule LRules | 
 Rule -> Selectors '{' LDeclaretions '}'
 LDeclaretions -> Declaration ';' MoreDeclerations
 MoreDeclerations -> LDeclaretions | 
 Selectors -> SimpleSelector MoreSelectors
 MoreSelectors -> Selectors | 
 SimpleSelector -> Astrisk SelectorModifier
 Astrisk -> '*' | 
 SelectorModifier -> '.' 'name' | ':' 'name' | '[' 'name' '=' Term ']' | '#hashid' | 'name'
 Declaration -> 'name' ':' LTerms Important
 Important -> '!ImPoRtAnT' | 
 LTerms -> Term MoreTerms
 MoreTerms -> LTerms | 
 Term -> '1337' | '15%' | '"string"' | 'name' | '#hashid'
 """)

parser = ChartParser(grammar)
gr = parser.grammar()

test_name = "generated"

with open(test_name + '.in', 'w+') as writer:
    writer.write(' '.join(produce(gr, gr.start())))

with open(test_name + '.out', 'w+') as writer:
    writer.write("\n".join(map(str, rules)))
    writer.write("\nSuccess\n")
