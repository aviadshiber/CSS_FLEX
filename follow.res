Follow:
S = { EF }
LImports = { NAME , HASHID , COLON , LBRACKET , ASTRISK , DOT , EF }
Import = { NAME , HASHID , IMPORT , COLON , LBRACKET , ASTRISK , DOT , EF }
LRules = { EF }
Rule = { NAME , HASHID , COLON , LBRACKET , ASTRISK , DOT , EF }
LDeclaretions = { RBRACE }
MoreDeclerations = { RBRACE }
Selectors = { LBRACE }
MoreSelectors = { LBRACE }
SimpleSelector = { NAME , HASHID , COLON , LBRACE , LBRACKET , ASTRISK , DOT }
Astrisk = { NAME , HASHID , COLON , LBRACKET , DOT }
SelectorModifier = { NAME , HASHID , COLON , LBRACE , LBRACKET , ASTRISK , DOT }
Declaration = { SEMICOLON }
Important = { SEMICOLON }
LTerms = { IMPORTANT , SEMICOLON }
MoreTerms = { IMPORTANT , SEMICOLON }
Term = { IMPORTANT , NAME , HASHID , STRING , SEMICOLON , RBRACKET , NUMBER , UNIT }

