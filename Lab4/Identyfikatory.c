//Operacje na stringach identyfikatory
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_DEFAULT 0
#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_CHAR 8
#define IN_ID 16

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define MAX_LINE 2048
#define K_WORDS_LEN 32


int last_index = 0;

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int is_valid ( char c, int first ) {
	if ( first ) {
		if ( '0' <= c && c <= '9' )
			return 0;
	}

	if ( '0' <= c && c <= '9' )
			return 1;
	if ( c == '_' )
		return 1;

	if ( 'a' <= c && c <= 'z' )
		return 1;
	
	if ( 'A' <= c && c <= 'Z' )
		return 1;

	return 0;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}




int is_used ( ) {

	for ( int i = 0; i < K_WORDS_LEN; i++ )
		if ( strcmp( tab[ last_index ], keywords[i] ) == 0)
			return 0;

	for ( int i = 0; i < last_index ; i++ )
		if ( strcmp( tab[last_index], tab[i] ) == 0)
			return 0;

	return 1;

}


int check_identity (
	char line[],
	int start,
	int end
) {
	int is_ok = start <= end;

	if ( !is_valid(line[start], 1) ) {
		is_ok = 0;
	}

	for ( int i = start + 1; is_ok && i <= end; i++ ) {
		if ( !is_valid( line[i], 0) ) {
			is_ok = 0;
			break;
		}
	}

	if ( is_ok == 0 ) 
		return 0;

	return is_used(line, start, end);

}

int find_idents() {

	char line[MAX_LINE];
	int state = 0;
	int index = 0;

	int indexes[MAX_IDS];

	for ( int i = 0; i < MAX_IDS; i++ ){
		indexes[i] = i;
	}

	int c_prev, c_next;
	int result = ( c_prev = getchar() ) != EOF;

	int counter = 0;

	if ( result == 0 ) {
		return 1;
	}

	if ( is_valid(c_prev, 1) ) {
		state = IN_ID;
		tab[last_index][index++] = (char)c_prev;
	}


	while ( ( c_next = getchar() ) != EOF ) {
		
		switch (state) {

			case IN_LINE_COMMENT:
				
				if ( c_next == '\n' )
					state = IN_DEFAULT;

				break;

			case IN_BLOCK_COMMENT:
				
				if ( c_prev != '*' )
					break;
				
				if ( c_next == '/' )
					state = IN_DEFAULT;

				break;

			case IN_STRING:

				if ( c_next == '\"' ) {
					state = IN_DEFAULT;
				}
				
				if ( c_next == '\\' ) {
					getchar();
				}

				break;
			
			case IN_CHAR:
				if ( c_next == '\'' ) {
					state = IN_DEFAULT;
				}
				
				if ( c_next == '\\' ) {
					getchar();
				}

				break;

			case IN_ID:
				if ( is_valid( c_next, 0 ) ) {
					tab[last_index][index++] = (char)c_next;
				} else {
					tab[last_index][index] = '\0';
					
					int is_free = is_used();

					state = IN_DEFAULT;
					index = 0;

					if ( is_free == 0 )
						tab[last_index][0] = '\0';
					else {
						last_index++;
					}
					counter += is_free;
				}

				break;
			
			default:
				if ( c_next == '\"') {
					state = IN_STRING;
					break;
				}

				if ( c_next == '\'' ) {
					state = IN_CHAR;
					break;
				}


				if ( is_valid( c_next, 1 ) ) {
					state = IN_ID;
					tab[last_index][index++] = (char)c_next;
					break;
				}

				if ( c_prev != '/' )
					break;

				if ( c_next == '/' ) {
					state = IN_LINE_COMMENT;
					break;
				}

				if ( c_next == '*' ) {
					state = IN_BLOCK_COMMENT;
					break;
				}

				break;
		}

		c_prev = c_next;
	}
	
	return counter;
	
}
