#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define TAB_SIZE  1001
#define BUF_SIZE  1001

int ptr_sizes[TAB_SIZE];

double get(int cols, int row, int col, const double *A){
    int number =cols*row+col;
    return A[number];
}

void set(int cols, int row, int col, double *A, double value){
    int number = cols*row+col;
    A[number] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB){
    int counter = 0;
    for(int i=0; i<rowsA ;i++){
        for(int j=0; j<colsB ;j++){
            int s =0;
            for(int k = 0; k<colsA ;k++){
                double d1 = get(colsA, i, k,A);
                double d2 = get(colsB, k, j, B);;
                s+= d1*d2;
            }
            set(rowsA, i,j,AB,s);
        }
    }
}

void read_mat(int rows, int cols, double *t){

    for(int i = 0; i < rows;i++){
        for(int j = 0; j < cols;j++){
            double temp = 0;
            scanf("%lf", &temp);
            set(cols, i,j,t,temp);
        }
    }

}

void print_mat(int rows, int cols, double *t){
    for(int i= 0 ; i<rows ;i++){
        for(int j = 0; j <cols ;j++){
            printf("%.2f ", get(rows, i,j,t));
        }
        printf("\n");
    }
}



int read_char_lines(char *tab[]){
    char* *strings;
    strings = malloc(TAB_SIZE* sizeof(char *));
    int len = 0;
    char buffer[BUF_SIZE];

    while(getchar()!='\n');
    int counter = 0;
    for(int i=0; i < TAB_SIZE; i++){
        if(fgets(buffer, BUF_SIZE, stdin) == NULL){
            break;
        }
        if(strlen(buffer)==1){
            break;
        }
        len = strlen(buffer);
        buffer[len-1]= '\0';
        strings[i] = malloc(len*sizeof (char ));
        strcpy(strings[i], buffer);
        counter++;
    }

    for(int i =0 ; i<counter; i++){
        tab[i] = strings[i];
    }
}

void write_char_line(char *tab[], int n){
    printf("%s", tab[n-1]);
}

int read_dbl_lines_v1(double *ptr_tab[]) {

    int index = 0;
    int index1 = 0;
    int i = 0;


    double dbuffer[BUF_SIZE];
    char buffer[BUF_SIZE];
    char c;

    while ( ( c = getchar()) != EOF ) {
        if ( isspace( c ) ) {
            double d = 0;
            sscanf( buffer, "%lf", &d );

            dbuffer[index1++] = d;
            for (int j = 0 ; j < i; j++ ) {
                buffer[j] = (char) 0;
            }
            i = 0;
        } else {
            buffer[i] = c;
            i++;
        }

        if ( c == '\n' ) {
            ptr_tab[index] = (double *) malloc( index1 * sizeof ( double ) );
            for ( int j = 0; j < index1; j++ ) {
                ptr_tab[index][j] = dbuffer[j];
            }
            for ( int j = 0; j < index1; j++ ) {
                dbuffer[j] = 0;
            }

            ptr_sizes[index] = index1 ;
            index++;
            index1 = 0;
            i = 0;
        }

    }
    ptr_tab[index] = (double *) malloc( index1 * sizeof ( double ) );

    for (int j = 0; j < index1 ; j++ ) {
        ptr_tab[index][j] = dbuffer[j];
    }

    ptr_sizes[index] = index1 ;
    return index ;
}

void write_dbl_line_v1(double *ptr_tab[], int n) {
    for ( int i = 0; i < ptr_sizes[n]; i++ ) {
        printf("%.2f ", ptr_tab[n][i]);
    }
}

int main(void) {
    int to_do;

    scanf ("%d", &to_do);

    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA,colsA,rowsB,colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d",&rowsA,&colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d",&rowsB,&colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA,colsA,colsB,A,B,C);
            print_mat(rowsA,colsB,C);
            break;
        case 2:
            scanf("%d",&n);
            ptr_table[0] = series_table;
            lines_counter = read_dbl_lines_v1(ptr_table);
            write_dbl_line_v1(ptr_table,n);
            break;
        case 3:
            scanf("%d", &n);
            lines_counter = read_char_lines(char_lines_table);
            write_char_line(char_lines_table,n);
// 			delete_lines(char_lines_table,lines_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
