#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"

typedef enum TokenType TokenType;
typedef struct Token Token;

enum TokenType {
	TOKEN_TYPE_NONE=0,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_INTEGER,
	TOKEN_TYPE_COLON,
	TOKEN_TYPE_DASH,
	TOKEN_TYPE_EOF,
	TOKEN_TYPE_UNKNOWN,
	TOKEN_TYPE_MAX
};

struct Token {
	TokenType type;
	char *text;
};



Token *CreateToken(TokenType type,char *text);
void DestroyToken(Token **token);
void DestroyTokens(Token ***tokens,size_t *ntokens);
void AddToken(Token ***tokens,size_t *ntokens,Token *token);
char *TokenToString(Token *token);



#ifdef TOKEN_IMPLEMENTATION

char *token_type_names[] = {
	"TOKEN_TYPE_NONE",
	"TOKEN_TYPE_STRING",
	"TOKEN_TYPE_INTEGER",
	"TOKEN_TYPE_COLON",
	"TOKEN_TYPE_DASH",
	"TOKEN_TYPE_EOF",
	"TOKEN_TYPE_UNKNOWN"
};

Token *CreateToken(TokenType type,char *text) {
	Token *token=malloc(sizeof(*token));
	if(token) {
	token->type=type;
	token->text=text==NULL?NULL:strdup(text);
	}
	return token;
}



void DestroyToken(Token **token) {
	free((*token)->text);
	(*token)->text=NULL;
	free(*token);
	(*token)=NULL;
}

void DestroyTokens(Token ***tokens,size_t *ntokens) {
	size_t i;
	for(i=0; i<(*ntokens); i++) {
		DestroyToken(&(*tokens)[i]);
	}
	free(*tokens);
	(*tokens)=NULL;
	(*ntokens)=0;
}

void AddToken(Token ***tokens,size_t *ntokens,Token *token) {
	(*tokens)=realloc(*tokens,sizeof(*tokens)*((*ntokens)+1));
	(*tokens)[(*ntokens)++]=token;
}


char *TokenToString(Token *token) {
	char *result=malloc(sizeof(*result)*STRING_MAX);
	if(result) {
	sprintf(result,"{ \"type\": \"%s\", \"text\": \"%s\" }",token_type_names[token->type],token->text);
	}
	return result;
}


#endif /* TOKEN_IMPLEMENTATION */

#endif /* TOKEN_H */
