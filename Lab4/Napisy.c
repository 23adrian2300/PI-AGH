//Opeacje na stringach liczenie słów znaków i diagramów
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)
#define NEWLINE '\n'
#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define MAX_LINE 128


int count[MAX_DIGRAMS] = { 0 };

int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

void wc(int *nl, int *nw, int *nc ) {

    *nl = 0;
    *nw = 0;
    *nc = 0;
    int lineCounter = 0, charCounter = 0, wordCounter = 0;
    int state = 0;
    int c;
	while ((c = getchar()) != EOF) {
		charCounter++;
		if (c == '\n')
        {
            lineCounter++;
        } 
		if (isspace(c)) {
			if(state & 1){
                state &= ~1; 
            } 
		} else if (!(state & 1)) {
			state = 1;
			wordCounter++;
		}
	}
    *nl = lineCounter;
    *nw = wordCounter;
    *nc = charCounter;
}

void char_count(int char_no, int *n_char, int *cnt) {
	int characters[MAX_CHARS];
	for (int i = 0; i < MAX_CHARS; ++i) {
		characters[i] = i;
	}

	int c;
	while ((c = getchar()) != EOF) {
		if (c >= FIRST_CHAR && c < LAST_CHAR){
            count[c - FIRST_CHAR]++;
        }
	}

	qsort(characters, MAX_CHARS, sizeof(int), cmp);

	*n_char = characters[char_no-1] + FIRST_CHAR;
	*cnt = count[characters[char_no-1]];


}

void digram_count(int digram_no, int digram[] ) {
	int digrams[MAX_DIGRAMS];
	for (int i = 0; i < MAX_DIGRAMS; ++i) {
		digrams[i] = i;
	}

	int c, c_prev = getchar();
	while ((c = getchar()) != EOF) {
		if (c < FIRST_CHAR || c >= LAST_CHAR || c_prev < FIRST_CHAR || c_prev >= LAST_CHAR) {
			c_prev = c;
			continue;
		}
		int index = (c_prev - FIRST_CHAR) * MAX_CHARS + c - FIRST_CHAR;
		count[index]++;
		c_prev = c;
	}

	qsort(digrams, MAX_DIGRAMS, sizeof(int), cmp_di);
	digram[0] = digrams[digram_no-1] / MAX_CHARS + FIRST_CHAR;
	digram[1] = digrams[digram_no-1] % MAX_CHARS + FIRST_CHAR;
	digram[2] = count[digrams[digram_no-1]];


}


void find_comments(int *line_comment_counter, int *block_comment_counter ) {
	int c, c_next;
	int line_comment = 0, block_comment = 0;
	int state = 0;

	while ((c = getchar()) != EOF) {
		switch (state) {

			case IN_LINE_COMMENT:
				if (c == '\n') {
					state = 0; // end of line comment
				}
				break;

			case IN_BLOCK_COMMENT:
				if (c != '*') break;
				c_next = getchar();
				if (c_next == '/') {
					state = 0; // end of block comment
				} 
				break;

			default:
				if (c != '/') break;
				c_next = getchar();
				if (c_next == '/') {
					state = IN_LINE_COMMENT; // line comment begins
					line_comment ++;
					break;
				}
				if (c_next == '*') {
					state = IN_BLOCK_COMMENT; // block comment begins
					block_comment ++;
					break;
				}
				break;
		}
	}
    *line_comment_counter = line_comment;
    *block_comment_counter = block_comment;
}

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_line();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
			char_no = read_line();
			digram_count(char_no, digram);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
