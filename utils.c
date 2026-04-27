#include <string.h>
char *crlf_to_lf(char *line_with_crlf)
{	
	size_t l= strlen(line_with_crlf);
	if (l<2){
		return NULL;
	}
	if (line_with_crlf[l-2]=='\r' && line_with_crlf[l-1]=='\n'){
		line_with_crlf[l-2]='\n';
		line_with_crlf[l-1]='\0';
		return line_with_crlf;
	}
	return NULL;
}

char *lf_to_crlf(char *line_with_lf)
{	size_t l =strlen(line_with_lf);
	if (l<2){
		return NULL;
	}
	if (line_with_lf[l-1]=='\n'){
		line_with_lf[l-1]='\r';
		line_with_lf[l]='\n';
		line_with_lf[l+1]='\0';
		return line_with_lf ;
	}
	return NULL;
}
