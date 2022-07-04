#include "ezxml.h"

#include <stdio.h>

int main() {

 	FILE *fout=fopen("kjv.csv","w");

	ezxml_t bibly = ezxml_parse_file("kjv.xml"),book,chap,vers;
	 
	for (book = ezxml_child(bibly, "BIBLEBOOK"); book; book = book->next) {
	    for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {
		    for (vers = ezxml_child(chap, "VERS"); vers; vers = vers->next) {
	    		fprintf(fout,"%s|%s:%s|%s\n", 
	    			ezxml_attr(book, "bname"), 
	    			ezxml_attr(chap, "cnumber"), 
	    			ezxml_attr(vers, "vnumber"), 
	    			ezxml_txt(vers)
	    		);
	    	}
	    }
	}
	
	ezxml_free(bibly); 

	fclose(fout);

	return 0;
}
