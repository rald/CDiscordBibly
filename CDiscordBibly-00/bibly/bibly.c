#include <string.h>
#include <math.h>

#include "discord.h"

#define BIBLY_IMPLEMENTATION
#include "bibly.h"

#define UTIL_IMPLEMENTATION
#include "util.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define LEXER_IMPLEMENTATION
#include "lexer.h"


BiblyInfo **binfos=NULL;
size_t nbinfos=0;

char biblePath[STRING_MAX]="kjv.csv";
char infoPath[STRING_MAX]="kjv.inf";


void on_ready(struct discord *client) {
	const struct discord_user *bot = discord_get_self(client);
	log_info("Logged in as %s!", bot->username);
}



void on_message(struct discord *client, const struct discord_message *msg) {

	char **verses=NULL;
	size_t nverses=0;
	char *book="John";
	int cnum=3;
	int svnum=16;
	int evnum=0;

	char cite[512];
	char searchText[512];

	size_t page=1;

	if(sscanf(msg->content,".kjv find %511[^\n]",searchText)==1 || sscanf(msg->content,".kjv page %zu %511[^\n]",&page,searchText)==2 ) {

	  char **lines=NULL;
	  size_t nlines=0;

	  char **vtokens=NULL;
	  size_t nvtokens=0;

	  char *xbook=NULL;
	  char *xnum=NULL;
	  char *xvers=NULL;
	  unsigned int xcnum=0;
	  unsigned int xvnum=0;


		char message[STRING_MAX];
		char verse[STRING_MAX];

		size_t i;

	  Bibly_Search(biblePath,&lines,&nlines,searchText);

		size_t pages=ceil((double)nlines/4);

		sprintf(message,"Found %d occurences. %d pages.\n",(int)nlines,(int)pages);

		struct discord_create_message params = { .content = message };
		discord_create_message(client, msg->channel_id, &params, NULL);

		size_t start=4*(page-1);
		size_t end=start+4;

		if(end>nlines) end=nlines;

		if(page<pages) {
			sprintf(message,"Listing page %zu.\n",page);

			struct discord_create_message params = { .content = message };
			discord_create_message(client, msg->channel_id, &params, NULL);
		}

	  for(size_t i=start; i<end; i++) {

	      tokenize(&vtokens,&nvtokens,lines[i],"|",2);

	      xbook=strdup(vtokens[0]);
	      xnum=strdup(vtokens[1]);
	      xvers=strdup(vtokens[2]);

	      freetok(&vtokens,&nvtokens);

	      tokenize(&vtokens,&nvtokens,xnum,":",1);

	      xcnum=atoi(vtokens[0]);
	      xvnum=atoi(vtokens[1]);

	      freetok(&vtokens,&nvtokens);

	      sprintf(verse,"(%zu) %s %d:%d -> %s\n",i+1,xbook,(int)xcnum,(int)xvnum,xvers);

	struct discord_create_message params = { .content = verse };
	discord_create_message(client, msg->channel_id, &params, NULL);


	      xvnum=0;
	      xcnum=0;
	      free(xvers);
	      xvers=NULL;
	      free(xnum);
	      xnum=NULL;
	      free(xbook);
	      xbook=NULL;

	  }

	  for(i=0; i<nlines; i++) {
	      free(lines[i]);
	      lines[i]=NULL;
	  }
	  nlines=0;


	} else if(sscanf(msg->content,".kjv %[^\n]",cite)==1) {

		Token **ltokens=NULL;
	  size_t nltokens=0;

	  char **verses=NULL;
	  size_t nverses=0;

	  char book[512]="";
	  unsigned int cnum=0;
	  unsigned int svnum=0;
	  unsigned int evnum=0;

	  char **vtokens=NULL;
	  size_t nvtokens=0;

	  char *xbook=NULL;
	  char *xnum=NULL;
	  char *xvers=NULL;
	  unsigned int xcnum=0;
	  unsigned int xvnum=0;

		char verse[STRING_MAX];

	  size_t i,j,k;

	  lex(&ltokens,&nltokens,cite);

		if(
	    nltokens==5 &&
	    ltokens[0]->type==TOKEN_TYPE_STRING &&
	    ltokens[1]->type==TOKEN_TYPE_INTEGER &&
	    ltokens[2]->type==TOKEN_TYPE_COLON &&
	    ltokens[3]->type==TOKEN_TYPE_INTEGER
		) {

			strcpy(book,ltokens[0]->text);
			cnum=atoi(ltokens[1]->text);
			svnum=atoi(ltokens[3]->text);
			evnum=svnum;

		} else if(
			nltokens==6 &&
			ltokens[0]->type==TOKEN_TYPE_INTEGER &&
			ltokens[1]->type==TOKEN_TYPE_STRING &&
			ltokens[2]->type==TOKEN_TYPE_INTEGER &&
			ltokens[3]->type==TOKEN_TYPE_COLON &&
			ltokens[4]->type==TOKEN_TYPE_INTEGER
		) {

			sprintf(book,"%s %s",ltokens[0]->text,ltokens[1]->text);
			cnum=atoi(ltokens[2]->text);
			svnum=atoi(ltokens[4]->text);
			evnum=svnum;

		} else if(
			nltokens==7 &&
			ltokens[0]->type==TOKEN_TYPE_STRING &&
			ltokens[1]->type==TOKEN_TYPE_INTEGER &&
			ltokens[2]->type==TOKEN_TYPE_COLON &&
			ltokens[3]->type==TOKEN_TYPE_INTEGER &&
			ltokens[4]->type==TOKEN_TYPE_DASH &&
			ltokens[5]->type==TOKEN_TYPE_INTEGER
		) {

			strcpy(book,ltokens[0]->text);
			cnum=atoi(ltokens[1]->text);
			svnum=atoi(ltokens[3]->text);
			evnum=atoi(ltokens[5]->text);

		} else if(
			nltokens==8 &&
			ltokens[0]->type==TOKEN_TYPE_INTEGER &&
			ltokens[1]->type==TOKEN_TYPE_STRING &&
			ltokens[2]->type==TOKEN_TYPE_INTEGER &&
			ltokens[3]->type==TOKEN_TYPE_COLON &&
			ltokens[4]->type==TOKEN_TYPE_INTEGER &&
			ltokens[5]->type==TOKEN_TYPE_DASH &&
			ltokens[6]->type==TOKEN_TYPE_INTEGER
		) {

			sprintf(book,"%s %s",ltokens[0]->text,ltokens[1]->text);
			cnum=atoi(ltokens[2]->text);
			svnum=atoi(ltokens[4]->text);
			evnum=atoi(ltokens[6]->text);
		}

		Bibly_GetVerses(biblePath,&verses,&nverses,book,cnum,svnum,evnum);

		for(size_t i=0;i<nverses;i++) {

	    tokenize(&vtokens,&nvtokens,verses[i],"|",2);

	    xbook=strdup(vtokens[0]);
	    xnum=strdup(vtokens[1]);
	    xvers=strdup(vtokens[2]);

	    freetok(&vtokens,&nvtokens);

	    tokenize(&vtokens,&nvtokens,xnum,":",1);

	    xcnum=atoi(vtokens[0]);
	    xvnum=atoi(vtokens[1]);

	    freetok(&vtokens,&nvtokens);

	    sprintf(verse,"%s %d:%d -> %s\n",xbook,(int)xcnum,(int)xvnum,xvers);

			struct discord_create_message params = { .content = verse };

			discord_create_message(client, msg->channel_id, &params, NULL);

		}

	}

}



int main(void) {

	srand(time(NULL));

	Bibly_GetInfo(infoPath,&binfos,&nbinfos);

	struct discord *client = discord_init("OTkyNzUyNzYwNzgzMTEwMTg1.G7bS6k.FnKP-T4gqFCk_5gYp7JAoZiK9onq1x2si_HOqc");
	discord_set_on_ready(client, &on_ready);
	discord_set_on_message_create(client, &on_message);
	discord_run(client);
}
