#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
    int int_data;
    char char_data;
    void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
    struct ht_element *next;
    data_union data;
} ht_element;

typedef struct hash_table {
    int size;
    int no_elements;
    ht_element **ht_element;
    DataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;
typedef struct DataWord {
    char *word;
    int counter;
} DataWord;
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {

    p_table -> size = size;
    p_table -> no_elements = 0;
    p_table -> ht_element = (ht_element**)malloc(sizeof(ht_element) * size);
    for(int i =0 ; i<size;i++){
        p_table->ht_element[i] = NULL;
    }
    p_table -> dump_data = dump_data;;
    p_table -> free_data = free_data;
    p_table -> compare_data = compare_data;
    p_table -> hash_function = hash_function;
    p_table -> modify_data = modify_data;

}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
    for(int i = 0; i<p_table->size; i++) {
        if(p_table->ht_element[i] != NULL && i ==n){
            ht_element *elem = p_table->ht_element[i];
            while (elem != NULL){
                ht_element *temp = elem;
                elem= elem->next;
                p_table->dump_data(temp->data);
            }
        }
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if(free_data != NULL){

        free(to_delete->data.ptr_data);
    }
    free(to_delete);
}



// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for(int i = 0; i<p_table->size; i++) {
        if(p_table->ht_element[i] != NULL){
            ht_element *elem = p_table->ht_element[i];
            while (elem != NULL){
                ht_element *temp = elem;
                elem= elem->next;
                free_element(p_table->free_data, temp);
            }
        }
    }
}

// calculate hash function for integer k
int hash_base(int k, int size) {
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (int)floor(size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    ht_element ** p_table_temp = malloc(sizeof(ht_element) * p_table->size);
    for(int i = 0; i<p_table->size; i++) {
        p_table_temp[i] = p_table->ht_element[i];
    }

    p_table->ht_element = realloc(p_table->ht_element, (p_table->size * 2) * sizeof(ht_element));
    for(int i =0 ; i< p_table->size*2;i++){
        p_table->ht_element[i] = NULL;
    }
    for(int i = 0; i< p_table->size; i++){
        if(p_table_temp[i] != NULL){
            ht_element *elem = p_table_temp[i];
            while (elem != NULL){
                ht_element *temp = elem;
                ht_element *e = malloc(sizeof(ht_element));
                ht_element  *nl = NULL;
                e->next = nl;
                e->data.int_data = elem->data.int_data;
                e->data.char_data = elem->data.char_data;
                if(p_table->modify_data != NULL){
                    DataWord newdata1 = *((struct DataWord *)elem->data.ptr_data);
                    DataWord *newone = malloc(sizeof(DataWord));
                    newone->word =  strdup(newdata1.word);
                    newone->counter = newdata1.counter;
                    e->data.ptr_data = newone;
                }else{
                    e->data.ptr_data = elem->data.ptr_data;
                }
                
                int newkey = p_table->hash_function(e->data, p_table->size*2);
                if(p_table->ht_element[newkey] == NULL) {
                    p_table->ht_element[newkey] = e;
                    p_table->ht_element[newkey]->next = NULL;
                }
                else{
                    ht_element *next2 = p_table->ht_element[newkey];
                    e->next  = next2;
                    p_table->ht_element[newkey] = e;
                }
                elem = elem->next;
            }
        }
    }
    for(int i = 0; i<p_table->size; i++) {
        if(p_table_temp[i] != NULL){
            ht_element *elem = p_table_temp[i];
            while (elem != NULL){
                ht_element *temp = elem;
                elem= elem->next;
                free_element(p_table->free_data, temp);
            }
        }
    }
    p_table -> size = p_table->size*2;
}

// // return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    ht_element *e = malloc(sizeof(ht_element));
    ht_element  *nl = NULL;
    e->data.int_data = data->int_data;
    e->data.char_data = data->char_data;
    e->data.ptr_data = data->ptr_data;
    e->next = nl;
    int key = p_table ->hash_function(e->data, p_table -> size);

    if(p_table->ht_element[key] == NULL ) {
        return NULL;
    }
    ht_element *next = p_table->ht_element[key];
    while(next != NULL){
        if(p_table->compare_data(next->data, e->data) == 0){
            return next;
        }else{
            next = next -> next;
        }
    }
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    ht_element *e = malloc(sizeof(ht_element));
    e->data.int_data = data->int_data;
    e->data.char_data = data->char_data;
    e->data.ptr_data = data->ptr_data;
    e->next = NULL;
    int key = p_table ->hash_function(e->data, p_table -> size);
    if(p_table->ht_element[key] == NULL ) {
        p_table->ht_element[key] = e;
        p_table->ht_element[key] -> next = NULL;
    }
    else{

        ht_element *next = p_table->ht_element[key];
        while(next != NULL){
            if(p_table->compare_data(next->data, e->data) == 0){
                if(p_table->modify_data != NULL){
                    p_table->modify_data(&next->data);
                }
                return;
            }else{
                next = next -> next;
            }

            }
            next = p_table->ht_element[key];
            e->next = next;
            p_table->ht_element[key] = e;
    }
    p_table -> no_elements ++;

    if(p_table->size*MAX_RATE == p_table->no_elements){
        rehash(p_table);
    }
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    int key = p_table ->hash_function(data, p_table -> size);
    ht_element *e = malloc(sizeof(ht_element));
    e->data.int_data = data.int_data;
    e->data.char_data = data.char_data;
    e->data.ptr_data = data.ptr_data;
    e->next = NULL;

    if(p_table ->ht_element[key] != NULL){
        ht_element *next = p_table->ht_element[key];
        ht_element *prev = NULL;
        while(next != NULL){
              if(p_table->compare_data(e->data, next->data) == 0){
                  if(next == p_table->ht_element[key]){
                      ht_element *temp = next;
                      next = next->next;
                      p_table->ht_element[key] = next;
                      free_element(p_table->free_data, temp);
                      p_table -> no_elements--;
                  }else{
                      ht_element *temp = next;
                      if(prev != NULL){
                          prev->next = next->next;
                      }
                      next= next->next;
                      free_element(p_table->free_data, temp);
                      p_table -> no_elements--;
                  }
              }
              else{
                  prev = next;
                  next = next->next;
              }
        }
    }


}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
    return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
    data_union data;
    int liczba;
    scanf("%d", &liczba);
    data.int_data = liczba;
    return data;
}

// char element

int hash_char(data_union data, int size) {
    return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ",data.char_data);
}

int cmp_char(data_union a, data_union b) {
    char c1 = a.char_data;
    char c2 = b.char_data;
    if (c1 == c2){
        return 0;
    }
    return 1;
}

// read char value and insert to the union
data_union create_data_char() {
    data_union data;
    char liczba;
    scanf("%c", &liczba);
    scanf("%c", &liczba);
    data.char_data = liczba;
    return data;
}

// Word element



void dump_word(data_union data) {
    DataWord newdata1 = *((struct DataWord *)data.ptr_data);
    printf("%s %d", newdata1.word, newdata1.counter);
}

void free_word(data_union data) {
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    DataWord newdata1 = *((struct DataWord *)a.ptr_data);
    DataWord newdata2 = *((struct DataWord *)b.ptr_data);
    return strcmp(newdata1.word, newdata2.word);
}

int hash_word(data_union data, int size) {
    int s = 0;
    DataWord *dw = (DataWord*)data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
    DataWord *dw = (DataWord*)data->ptr_data;
    dw->counter ++;
}
void to_lower( char *str ){
    for ( int i = 0; i < strlen(str); i++ )
        str[i] = tolower(str[i]);
}
// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    DataWord *newone = malloc(sizeof(DataWord));
    newone->word =  strdup(value);
    newone->counter = 1;
    to_lower( newone->word );
    data_union newdata;
    newdata.ptr_data = newone;
    return newdata;

}



// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {

    char buffer[BUFFER_SIZE];
    while(fgets(buffer, BUFFER_SIZE, stream)){
        char * pch;
        pch = strtok (buffer,".,?!:;-.\n\t\r ");
        while (pch != NULL)
        {
            data_union data = create_data_word(pch);
            insert_element( p_table, &data );
            pch = strtok (NULL,".,?!:;-.\n\t\r ");
        }
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        data = create_data();
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n, index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;
    ht_element *e;
    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_int,NULL,
                    cmp_int, hash_int, NULL);
            table_test(&table, n, create_data_int);
            printf ("%d\n", table.size);
            dump_list(&table, index);
 			free_table(&table);
            break;
        case 2: // test char table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_char,NULL,
                    cmp_char, hash_char, NULL);
            table_test(&table, n, create_data_char);
            printf ("%d\n", table.size);
            dump_list(&table, index);
 			free_table(&table);
            break; 
 		case 3: // read words from text, insert into table, and print
 			scanf("%s", buffer);

 			init_ht(&table, 8, dump_word, free_word,
 			        cmp_word, hash_word, modify_word);
 			stream_to_ht(&table, stdin);
 			printf ("%d\n", table.size);
 			data = create_data_word(buffer);
 			e = get_element(&table, &data);
 			if (e) table.dump_data(e->data);
 			if (table.free_data) table.free_data(data);
 			free_table(&table);
 			break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}