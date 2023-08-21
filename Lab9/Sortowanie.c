#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2){
    Person *pers1 = (Person *)p1;
    Person *pers2 = (Person *)p2;
    int a = pers1 -> age;
    int b = pers2 -> age;
    if(a > b){
        return -1;
    }
    else if(a < b){
        return 1;
    }


    if(strcmp(pers1 -> first_name, pers2 -> first_name) == 1 || strcmp(pers1 -> first_name, pers2 -> first_name) == -1){
        return strcmp(pers1 -> first_name,pers2 -> first_name);
    }

    return strcmp(pers1 -> last_name,pers2 -> last_name);
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons){
	int index = 0;
    int index1 = 0;
    int i = 0;
	int counter = 0;

    double dbuffer[MAX_PERSONS];
    char buffer[MAX_PERSONS];
    char c;
    char newString[MAX_PERSONS][MAX_STR_LEN];
    int ctr = 0;
    int j = 0;
    while ( ( c = getchar()) != EOF ) {
        if ( isspace( c ) ) {
			if(index1 == 0){
				int d = 0;
            	sscanf( buffer, "%d", &d );
				persons[index].age = d;
				index1++;
			    i=0;
			}else{
			     buffer[i] = c;
                 i++;
			}

        } else {
            buffer[i] = c;
            i++;
        }


        if ( c == '\n' ) {
            j = 0;
            for(int i = 0; i <= strlen(buffer); i++){
                if(buffer[i] == ' ' || buffer[i] == '\0'){
                    newString[ctr][j] = '\0';
                    ctr++;
                    j=0;
                }else{
                    newString[ctr][j] = buffer[i];
                    j++;
                }
            }
            memset(buffer, 0, sizeof buffer);
            index++;
            index1 = 0;
            i = 0;
        }


    }
    int k = 0;
    int l = 0;
    for(int i = 0; i < ctr;i++){
        if(k==0){
            strcpy(persons[l].first_name, newString[i]);
            k = 1;
        }else{
           strcpy(persons[l].last_name, newString[i]);
           k =0;
           l ++;
        }
    }
    return l ;
}

// Print Person array
void print_person_array(Person *persons, int n){
    for(int i = 0; i<n; i++){
        printf("%d %s %s", persons[i].age, persons[i].first_name, persons[i].last_name);
    }
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2){
    const char *a = l1;
    const char *b = l2;
    int age1, age2;
    sscanf(a,"%d", &age1);
    sscanf(b,"%d", &age2);
    char p1[3][MAX_STR_LEN];
    char p2[3][MAX_STR_LEN];
    int ctr = 0;
    int j = 0;
    for(int i = 0; i <= strlen(a); i++){
                if(a[i] == ' ' || a[i] == '\0'){
                    p1[ctr][j] = '\0';
                    ctr++;
                    j=0;
                }else{
                    p1[ctr][j] = a[i];
                    j++;
                }
            }
    j = 0;
    ctr = 0;
    for(int i = 0; i <= strlen(b); i++){
                if(b[i] == ' ' || b[i] == '\0'){
                    p2[ctr][j] = '\0';
                    ctr++;
                    j=0;
                }else{
                    p2[ctr][j] = b[i];
                    j++;
                }
    }

    if((p1[1][strlen(p1[1])-1] == 'a' && p2[1][strlen(p2[1])-1] != 'a')){
        return -1;
    }else if ((p1[1][strlen(p1[1])-1] != 'a' && p2[1][strlen(p2[1])-1] == 'a')){
        return 1;
    }else if((p1[1][strlen(p1[1])-1] == 'a' && p2[1][strlen(p2[1])-1] == 'a')){
       if (age1 > age2){
            return 1;
        }else{
            return -1;
        }
    }
    else if(p1[1][strlen(p1[1])-1] != 'a' && p2[1][strlen(p2[1])-1] != 'a'){
        return strcmp(p1[2], p2[2]);
    }else {
        return 1;
    }
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]){
    int index = 0;
    int i = 0;

    char buffer[MAX_PERSONS];
    char c;
    int ctr = 0;
    while ( ( c = getchar()) != EOF ) {

            buffer[i] = c;
            i++;


        if ( c == '\n' ) {
            if(i==1){
                break;
            }
            strcpy(lines[ctr], buffer);
            ctr++;
            memset(buffer, 0, sizeof buffer);
            index++;
            i = 0;
        }


    }

    return ctr ;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n){
    for(int i = 0; i < n; i++){
        printf("%s", lines[i]);
    }
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}


