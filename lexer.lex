%{

/* Declarations section */
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <stdbool.h>
#include "tokens.h"
/*<COMM>(\/\*)(.|\s)*(\/\*)(.|\s)*(\*\/)*/
#define STRING_MAX_SIZE (1025)
int commentCounter;
bool CRFlag = false;
char stringBuffer[STRING_MAX_SIZE]={0};
int stringIndex;
void initCommentCounter();
void incCommentCounter();
void showComment();
void showToken(const char * name);
void errorChar();
void error(const char* errorType);
void initStringBuffer();
void addSubStringToBuffer();
void showString();
void errorAscii();
int indexof(const char* str,char c);
void convertToAscii();
void convertEscapings();
void addCharToBuffer(char c);



%}

%option yylineno
%option noyywrap
digit   		([0-9])
hex				([0-9A-Fa-f])
letter  		([a-zA-Z])
lineFeed		([\n\r]|(\r\n))
whitespace		([\x20\x09\x0A\x0D])
not_whitespace	([^\x20\x09\x0A\x0D])
startComment	(\/\*)
endComment		(\*\/)
id				((\-)?{letter}+(_|{letter}|\-|{digit})*)
number			([\-\+])?((0x){hex}+|{digit}+)
integer			([\-\+])?{digit}+
spaceInt		{whitespace}*{integer}{whitespace}*
comment			(\/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+\/)
printable		([\x20-\x7E]|{whitespace})
backSlash		(\x5C)
doubleQuote		(\x22)
singleQuote		(\x27)

%x C_COMMENT
%x C_STRING_TYPE1
%x C_STRING_TYPE2

%%

{startComment}												{BEGIN(C_COMMENT);initCommentCounter();}
<C_COMMENT>{startComment}									error("Warning nested comment");
<C_COMMENT>{endComment}										BEGIN(INITIAL);//showComment();
<C_COMMENT><<EOF>>											error("Error unclosed comment");
<C_COMMENT>{lineFeed}										incCommentCounter();
<C_COMMENT>{printable}										;
<C_COMMENT>.												; //{yytext="/";errorChar();}

%{ /* here no convrtsion of ascii is needed (ERROR HANDLING IS MUST) */ %}
{doubleQuote}												{initStringBuffer();BEGIN(C_STRING_TYPE1);}
<C_STRING_TYPE1>{doubleQuote}								{BEGIN(INITIAL); return STRING;}//showString();
<C_STRING_TYPE1>[(\n)|(\r)|(\r\n)]							error("Error unclosed string");//to check
<C_STRING_TYPE1><<EOF>>										error("Error unclosed string");
<C_STRING_TYPE1>\\[n|r|t|(\x5C)]							addSubStringToBuffer();
<C_STRING_TYPE1>\\(0x|x)?([0-9A-Fa-f]){1,6}					addSubStringToBuffer();
<C_STRING_TYPE1>\\(0x|x)?([^\x20\x09\x0A\x0D])				errorAscii();
<C_STRING_TYPE1>([\x20-\x7E|\x09]{-}[(\x22)])				addSubStringToBuffer();
<C_STRING_TYPE1>.											; //{yytext="\"";errorChar();}

%{ /* here convrtsion of ascii is needed */ %}
{singleQuote}												{initStringBuffer();BEGIN(C_STRING_TYPE2);}
<C_STRING_TYPE2>{singleQuote}								{BEGIN(INITIAL); return STRING;}//showString();
<C_STRING_TYPE2>[(\n)|(\r)|(\r\n)]							error("Error unclosed string");
<C_STRING_TYPE2><<EOF>>										error("Error unclosed string");
<C_STRING_TYPE2>\\[n|r|t|(\x5C)]							convertEscapings();
<C_STRING_TYPE2>\\(0x|x)?([0-9A-Fa-f]){1,6}					convertToAscii(); 
<C_STRING_TYPE2>\\(0x|x)?([^\x20\x09\x0A\x0D])				errorAscii();
<C_STRING_TYPE2>([\x20-\x7E|\x09]{-}[(\x27)])				addSubStringToBuffer();
<C_STRING_TYPE2>.											; //{yytext="'";errorChar();}


#({digit}|(\-)?{letter})(_|{letter}|\-|{digit})*			return HASHID; /*showToken("HASHID");*/
"@import"													return IMPORT; /*showToken("IMPORT");*/
!{whitespace}*[i|I][m|M][p|P][o|O][r|R][t|T][a|A][n|N][t|T] return IMPORTANT; /*showToken("IMPORTANT");*/
{number}													return NUMBER; /*showToken("NUMBER");*/
{digit}*(\.)?{digit}+((\%)|[a-z]+)							return UNIT; /*showToken("UNIT");*/
rgb\({spaceInt},{spaceInt},{spaceInt}\)						;/*showToken("RGB");*/
rgb\(														error("Error in rgb parameters");
(\>|\+|~) 													return COMB; /*showToken("COMB");*/
(:)															return COLON; /*showToken("COLON");*/
(;)															return SEMICOLON; /*showToken("SEMICOLON");*/
(\{)														return LBRACE; /*showToken("LBRACE");*/
(\})														return RBRACE; /*showToken("RBRACE");*/
(\[)														return LBRACKET; /*showToken("LBRACKET");*/
(\])														return RBRACKET; /*showToken("RBRACKET");*/
(=)															return EQUAL; /*showToken("EQUAL");*/
(\*)														return ASTERISK; /*showToken("ASTERISK");*/
(\.)														return DOT; /*showToken("DOT");*/
(,)															return COMMA;
{id}														return NAME;/*showToken("NAME");*/
<<EOF>> 													return EF;
{whitespace}												;
.															errorChar();


%%

void error(const char* errorType){
	printf("%s\n",errorType);
	exit(0);
}

void errorAscii(){
	printf("Error undefined escape sequence %c\n",yytext[1]);
	exit(0);
}

void errorChar(){
	printf("Error %s\n",yytext);
	exit(0);
}

int indexof(const char* str,char c){
	int i=0;
	while('\0'!=str[i]){
		if(str[i]==c)
			return i;
		i++;
	}
	return -1;
}

void addCharToBuffer(char c){
	stringBuffer[stringIndex]=c;
	stringIndex++;
	stringBuffer[stringIndex]='\0';
	CRFlag = false;
}


void convertEscapings(){
	
	char c = yytext[1];
	if(c == 't'){
		addCharToBuffer('\t');
	}else if(c == 'r'){
		addCharToBuffer('\r');
		CRFlag = true;
	}else if(c == 'n' /*&& !CRFlag*/){
		addCharToBuffer('\n');
	}else{
		if(c=='\\')
			addCharToBuffer('\\');
	}
	
}

void convertToAscii(){
	int i,ascii;
	char c,tempChar;
	i=indexof(yytext,'x');
	if(i<0) i=0;
	ascii=strtol(yytext+i+1, NULL, 16);
	
	
	if( 32<= ascii && ascii<127){
			tempChar=(char)ascii;
			addCharToBuffer(tempChar);
	}
	
}

void initStringBuffer(){
	stringIndex=0;
}
void addSubStringToBuffer(){
	int i,j;
	for(i=stringIndex,j=0;j<yyleng && i<STRING_MAX_SIZE;i++,j++){
		stringBuffer[i]=yytext[j];
	}
	stringIndex+=yyleng;
	stringBuffer[stringIndex]='\0';
	CRFlag = false;
}

void showString(){
	printf("%d STRING %s\n",yylineno,stringBuffer);
}


void initCommentCounter(){
	commentCounter=1;
}

void incCommentCounter(){
	commentCounter++;
}

void showComment(){
	printf("%d COMMENT %d\n",yylineno,commentCounter);
}

void showToken(const char * name){
		printf("%d %s %s\n",yylineno,name,yytext);
        
}


