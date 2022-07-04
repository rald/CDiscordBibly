#define UTIL_IMPLEMENTATION
#include "util.h"

int main() {

	char **toks=NULL;
	size_t ntoks=0;

	char *s="Gerald|1:1|Hello World";

	tokenize(&toks,&ntoks,s,"|",2);

	for(size_t i=0;i<ntoks;i++) {
		printf("%s\n",toks[i]);
	}

	freetok(&toks,&ntoks);

	return 0;
}
