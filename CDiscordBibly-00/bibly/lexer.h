#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define UTIL_IMPLEMENTATION
#include "util.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define TEXT_MAX 1024

typedef enum LexerState LexerState;

enum LexerState {
	LEXER_STATE_DEFAULT=0,
	LEXER_STATE_STRING,
	LEXER_STATE_INTEGER,
	LEXER_STATE_SYMBOL,
	LEXER_STATE_MAX
};

void lex(Token ***tokens,size_t *ntokens,char *text);



#ifdef LEXER_IMPLEMENTATION

void lex(Token ***tokens,size_t *ntokens,char *str) {
	bool quit=false;
	int i=0;
	LexerState state=LEXER_STATE_DEFAULT;
	char text[TEXT_MAX];

	char ch;

	text[0]='\0';

	while(!quit) {

	ch=str[i];

	switch(state) {

		case LEXER_STATE_DEFAULT:

		if(isalpha(ch) || ch==' ') {
			i--;
			state=LEXER_STATE_STRING;
		} else if(isdigit(ch)) {
			i--;
			state=LEXER_STATE_INTEGER;
		} else if(ch==':') {
			AddToken(tokens,ntokens,CreateToken(TOKEN_TYPE_COLON,":"));
		} else if(ch=='-') {
			AddToken(tokens,ntokens,CreateToken(TOKEN_TYPE_DASH,"-"));
		} else {
			quit=true;
			continue;
		}

		break;

		case LEXER_STATE_STRING:

		if(isalpha(ch) || ch==' ') {
			strcat(text,(char[]){ch,'\0'});
		} else {
			AddToken(tokens,ntokens,CreateToken(TOKEN_TYPE_STRING,trim(text)));
			text[0]='\0';
			i--;
			state=LEXER_STATE_DEFAULT;
		}

		break;
		case LEXER_STATE_INTEGER:

		if(isdigit(ch)) {
			strcat(text,(char[]){ch,'\0'});
		} else {
			AddToken(tokens,ntokens,CreateToken(TOKEN_TYPE_INTEGER,text));
			text[0]='\0';
			i--;
			state=LEXER_STATE_DEFAULT;
		}

		break;
		default: break;
	}

	if(ch=='\0') { quit=true; continue; }

	i++;

	}

	AddToken(tokens,ntokens,CreateToken(TOKEN_TYPE_EOF,NULL));

}

#endif /* LEXER_IMPLEMENTATION */

#endif /* LEXER_H */
