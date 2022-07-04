#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char *trim(char *str);
char *strupr(const char *str);
char *strsub(const char *str,size_t start,size_t length);
void tokenize(char ***tokens,size_t *ntokens,const char *str,const char *del,size_t n);
void freetok(char ***tokens,size_t *ntkens);

#ifdef UTIL_IMPLEMENTATION

char *trim(char *a) {
    char *p = a, *q = a;
    while (isspace(*q))            ++q;
    while (*q)                     *p++ = *q++;
    *p = '\0';
    while (p > a && isspace(*--p)) *p = '\0';
	return a;
}

char *strupr(const char *str) {
  char *result=strdup(str);
  size_t i=0;
  while(result[i]) {
    result[i]=toupper(result[i]);
    i++;
  }
  return result;
}



char *strsub(const char *str,size_t start,size_t length) {
  char *newstr=malloc(sizeof(*newstr)*(length+1));
  memcpy(newstr,str+start,length);
  newstr[length]='\0';
  return newstr;
}



void tokenize(char ***tokens,size_t *ntokens,const char *str,const char *del,size_t n) {
  char *pos=NULL;
  size_t start=0,length=0;
  size_t cnt=0;

  while((n==0 || cnt<n) && (pos=strstr(str,del))) {
    length=pos-str;
    if(length) {
      (*tokens)=realloc(*tokens,sizeof(*tokens)*(*ntokens+1));
      (*tokens)[(*ntokens)++]=strsub(str,start,length);
    }
    str+=length+strlen(del);
    cnt++;
  }

  if(*str!='\0') {
    (*tokens)=realloc(*tokens,sizeof(*tokens)*(*ntokens+1));
    (*tokens)[(*ntokens)++] = strdup(str);
  }
}

void freetok(char ***tokens,size_t *ntokens) {
  size_t i;
  for(i=0;i<(*ntokens);i++) {
    free((*tokens)[i]);
    (*tokens)[i]=NULL;
  }
  free(*tokens);
  (*tokens)=NULL;
  (*ntokens)=0;
}

#endif /* UTIL_IMPLEMENTATION */

#endif /* UTIL_H */
