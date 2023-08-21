#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
	struct tagListElement *next;
	void                  *data;
} ListElement;

typedef struct tagList {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp      free_data;
	CompareDataFp compare_data;
	InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void to_lower( char *str ){
	for ( int i = 0; i < strlen(str); i++ )
		str[i] = tolower(str[i]);
}

void init_list(List *list, ConstDataFp dump_data, DataFp free_data,
			   CompareDataFp compare_data, InsertInOrder insert_sorted) {
	list->head=0;
	list->tail=0;
	list->dump_data=dump_data;
	list->free_data=free_data;
	list->compare_data=compare_data;
	list -> insert_sorted = insert_sorted;

}

// Print all elements of the list
void dump_list(const List* list) {
	ListElement*i;
	for(i=list->head;i!=0;i=i->next){
		if(list->dump_data){
		    list->dump_data(i->data);
		}
		else printf("%p ",i->data);
	}

}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement*i;
	for(i=p_list->head;i!=0;i=i->next){
		if(p_list->dump_data && p_list -> compare_data(data, i->data) == 0){
		    p_list->dump_data(i->data);
		}
	}
}

// Free all elements of the list
void free_list(List* list) {
	ListElement*current = 0;
	current = list->head;
	while(current!=0){
		ListElement*todelete = current;
		current=current->next;
		if(list->free_data){
			list->free_data(todelete->data);
		}
		free(todelete);
	}
	list->head=0;
	list->tail=0;
}

// Push element at the beginning of the list
void push_front(List *list, void *data){
	ListElement*element = (ListElement*)malloc(sizeof(ListElement));
	element->data=data;
	if(list -> head == 0){
		element -> next = 0;
		list -> head = element;
		list ->tail = element;
		return;
	}
	element -> next=list->head;
	list->head=element;
	if(list -> tail == 0){
	    list -> tail = element;
	}
}


// Push element at the end of the list
void push_back(List *list, void *data) {
	ListElement*element = (ListElement*)malloc(sizeof(ListElement));
	element->data=data;
	element->next=0;
	if(list->tail != 0){
		list->tail->next=element;
	}
	list->tail=element;
	if(list->head == 0){
		list->head=list->tail;
	}
}

// Remove the first element
void pop_front(List *list) {
	if(!list->head)return;
	void*data=data=list->head->data;
	ListElement*toDel = list->head;
	list->head=list->head->next;
	free(toDel);
	if(list->head==0)list->tail=0;

}

// Reverse the list
void reverse(List *list) {
	ListElement *cur = list ->head, *prev = 0, *tmp = 0;
	while(cur != 0) {
		tmp = cur -> next;
		cur -> next = prev;
		prev = cur;
		cur = tmp;
	}
	ListElement *tempEl = list -> head;
	list ->head = list->tail;
	list-> tail = tempEl;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
	p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *list, ListElement *element) {
	ListElement*insertionPoint = 0;
	ListElement*current = 0;
	for(current = list->head;current!=0;current=current->next){
		if( list->compare_data(current->data,element->data)<=0)
		insertionPoint=current;
	} // else break;
	return insertionPoint;
}

// Insert element after 'previous'
void push_after(List *list, void *data, ListElement *previous) {
	ListElement*element = (ListElement*)malloc(sizeof(ListElement));
	element -> data = data;
	if(list -> tail == previous){
		element -> next = 0;
		previous -> next = element;
		list -> tail = element;
	}else{
		element -> next = previous -> next;
		previous -> next = element;
	}
}

// Insert element preserving order (no counter)
void insert_elem(List *list, void *p_data) {
    if ( list -> head == 0 ) {
		push_back( list, p_data);
		return;
	}

	ListElement *prev = list->head;
	ListElement *next = list->head->next;
	ListElement *p = malloc( sizeof( ListElement ));

	p->data = p_data;


	if ( list->compare_data( prev->data, p_data ) == 0 )
		return;
	if ( list->compare_data( prev->data, p_data ) > 0 ) {
		push_front( list, p_data );
		return;
	}

	while ( next != 0 ) {
		if ( list->compare_data( p_data, next->data ) == 0 )
			return;

		if ( list->compare_data( p_data, next->data ) < 0 ) {
			prev->next = p;
			p->next = next;
			return;
		}

		prev = prev->next;
		next = next->next;
	}

	push_back( list, p_data);

}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
	int id;
} DataInt;

void dump_int(const void *d) {
    int val = ((struct DataInt *)d)->id;
	printf("%d ",  val);
}

void free_int(void *d) {
    DataInt *str = (DataInt *)d;
	free(str);
}

int cmp_int(const void *a, const void *b) {
    int val1 = ((struct DataInt *)a)->id;
    int val2 = ((struct DataInt *)b)->id;

	return val1 - val2;
}

DataInt *create_data_int(int v) {
    DataInt*d = (DataInt*)malloc(sizeof(DataInt));
	d -> id = v;
	return d;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
	printf("%s\n",((struct DataWord *)d)->word);
}

void dump_word_lowercase (const void *d) {
	printf( "%s\n",((struct DataWord *)d)->word);
}

void free_word(void *d) {
    DataWord *p = ((struct DataWord *)d);
	free( p->word);
	free( p );
}

// // conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    return strcmp(((struct DataWord *)a)->word,((struct DataWord *)b)->word);
}

int cmp_word_counter(const void *a, const void *b) {
    return ((struct DataWord *)a)->counter - ((struct DataWord *)b)->counter;
}

void increment ( void *data ){
	DataWord *p = data;
	p->counter++;
}

// // insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data) {
	if ( p_list -> head == 0 ) {
		push_back( p_list, data);
		return;
	}
    ListElement *prev = p_list->head;
	ListElement *next = p_list->head->next;
	ListElement *p = malloc( sizeof( ListElement ));

	p->data = data;


	if ( p_list->compare_data( prev->data, data ) == 0 ) {
		increment( prev->data );
		return;
	}
	if ( p_list->compare_data( prev->data, data ) > 0 ) {
		push_front( p_list, data );
		return;
	}

	while ( next != NULL ) {
		if ( p_list->compare_data( data, next->data ) == 0 ) {
			increment( next->data );
			return;
		}

		if ( p_list->compare_data( data, next->data ) < 0 ) {
			prev->next = p;
			p->next = next;
			return;
		}

		prev = prev->next;
		next = next->next;
	}

	push_back( p_list, data);

}

// // read text, parse it to words, and insert those words to the list
// // in order given by the last parameter (0 - read order,
// // 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {

    char buffer[BUFFER_SIZE];
	int i = 0;
	while(fgets(buffer, BUFFER_SIZE, stream)){
        if(strlen(buffer) <= 2 && i != 0){
            break;
        }
		i++;
		char * pch;
		pch = strtok (buffer,".,?!:;-.\n\t\r ");
		while (pch != NULL)
		{
			DataWord *w = malloc( sizeof( DataWord ));
			w->word = strdup( pch );
			w->counter = 1;
			if ( order ) {
				to_lower( w->word );
				insert_elem_counter( p_list, w );
			}
			else{
				push_back( p_list, w );
			}
			pch = strtok (NULL,".,?!:;-.\n\t\r ");

		}
	}
}

// test integer list
void list_test(List *p_list, int n) {
	char op[2];
	int v;
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int,
					  cmp_int, insert_elem);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word,
					  cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 0);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word,
			          cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 1);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
//			list.dump_data = dump_word_lowercase;
			dump_list_if(&list, &data);
			printf("\n");
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

