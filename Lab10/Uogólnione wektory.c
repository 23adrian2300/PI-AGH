#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector
{
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person
{
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
    if(vector != NULL){
        vector->element_size = element_size;
    	vector->size = 0;
    	vector->capacity = block_size;
    	vector->data = malloc(block_size * element_size);
    }
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity)
{
	if (vector->capacity >= new_capacity)
		return;

	vector->data = realloc(vector->data, new_capacity * vector->element_size);
	vector->capacity = new_capacity;
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size)
{
	reserve(vector, new_size);

	if (vector->size >= new_size)
	{
		vector->size = new_size;
		return;
	}

	for (size_t i = vector->size; i < new_size; i++)
		for (size_t j = 0; j < vector->element_size; j++)
		{
			char *el = (char *)(vector->data + (i * vector->element_size) + j);
			*el = 0;
		}
	vector->size = new_size;
}

void checkCapacity(Vector *vector){
	if(vector->size +1 > vector->capacity){
		reserve(vector, vector->capacity*2);
	}
}
// Add element to the end of the vector
void push_back(Vector *vector, void *value){
	checkCapacity(vector);
	void *l= vector->data + (vector->size*vector->element_size);
	memcpy(l, value, vector->element_size);
	vector->size  = vector -> size + 1;
}

// Remove all elements from the vector
void clear(Vector *vector){
	vector->size = 0;
}

// Remove the last element from the vector
void pop_back(Vector *vector){
	vector->size = vector ->size -1;
}



// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value){
	if(index < 0 ){
		return;
	}
	checkCapacity(vector);
	for(int i = vector->size-1; i>= index; i--){
		void *el  = vector->data + (i*vector->element_size);
		memcpy(el + vector->element_size, el, vector->element_size);
	}
	void *el  = vector->data + (index*vector->element_size);
	memcpy(el, value, vector->element_size);
	vector->size = vector -> size +1;
}

// Erase element at position index
void erase(Vector *vector, int index){

	for (size_t i = index; i < vector->size - 1; i++){
		void *el = vector->data + (i * vector->element_size);
		memcpy(el, el + vector->element_size, vector->element_size);
	}
	vector->size =vector->size -1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int(*cmp)(const void*, const void*)){
	int i =0;
	while(i<vector->size){
		void *el  = vector->data + (i*vector->element_size);
		if(cmp(el,value)==0){
			erase(vector, i);
			i--;
		}
		i++;
	}
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)){
	int i =0;
	while(i<vector->size){
		void *el  = vector->data + (i*vector->element_size);
		if(predicate(el)==1){
			erase(vector, i);
			i--;
		}
		i++;
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector){
	vector->data = realloc(vector->data, vector->size*vector->element_size);
	vector->capacity = vector->size;
}

// Print integer vector
void print_vector_int(Vector *vector){
    printf("%zu\n", vector->capacity);
	for(int i = 0; i<vector->size; i++){
		int *el = (int *)(vector->data + (i * vector->element_size));
		printf("%d ", *el);
	}
}

// Print char vector
void print_vector_char(Vector *vector){
    printf("%zu\n", vector->capacity);
	for(int i = 0; i<vector->size; i++){
		char *el = (char *)(vector->data + (i * vector->element_size));
		printf("%c ", *el);
	}
}

// Print vector of Person
void print_vector_person(Vector *vector){
    printf("%zu\n", vector->capacity);
	for(int i = 0; i<vector->size; i++){
		Person *el = (Person *)(vector->data + (i * vector->element_size));
		printf("%d %s %s\n", el->age, el->first_name, el->last_name);
	}
}

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2){
	int *l1 = (int *)v1;
	int *l2 = (int *)v2;
	return *l1 - *l2;
}

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2){
	char *l1 = (char *)v1;
	char l2 = char v2;
	return *l1 - l2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2){
	Person *pers1 = (Person *)p1;
	Person *pers2 = (Person *)p2;

	if (pers1->age != pers2->age)
		return pers2->age - pers1->age;

	if (strcmp(pers1->first_name, pers2->first_name))
		return strcmp(pers1->first_name, pers2->first_name);

	return strcmp(pers1->last_name, pers2->last_name);
}

// predicate: check if number is even
int is_even(void *value){
	int *el = (int *)value;
	return *el % 2 ? 0 : 1;
}

// predicate: check if char is a vowel
int is_vowel(void *value){
	char *el = (char *)value;
	switch (*el)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
	case 'y':
	case 'A':
	case 'E':
	case 'I':
	case 'O':
	case 'U':
	case 'Y':
		return 1;
	default:
		return 0;
	}
}

// predicate: check if person is older than 25
int is_older_than_25(void *person){
	Person *pers = (Person *)person;
	if(pers->age >25){
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------

void read_int(void *value)
{
	scanf("%d", (int *)value);
}

void read_char(void *value)
{
	char c[2];
	scanf("%s", c);
	*(char *)value = c[0];
}

void read_person(void *value)
{
	Person *person = (Person *)value;
	scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void (*read)(void *),
				 int (*cmp)(const void *, const void *), int (*predicate)(void *))
{
	char op[2];
	int index;
	size_t size;
	void *v = malloc(vector->element_size);
	for (int i = 0; i < n; ++i)
	{
		scanf("%s", op);
		switch (op[0])
		{
		case 'p': // push_back
			read(v);
			push_back(vector, v);
			break;
		case 'i': // insert
			scanf("%d", &index);
			read(v);
			insert(vector, index, v);
			break;
		case 'e': // erase
			scanf("%d", &index);
			read(v);
			erase(vector, index);
			erase_value(vector, v, cmp);
			break;
		case 'd': // erase (predicate)
			erase_if(vector, predicate);
			break;
		case 'r': // resize
			scanf("%zu", &size);
			resize(vector, size);
			break;
		case 'c': // clear
			clear(vector);
			break;
		case 'f': // shrink
			shrink_to_fit(vector);
			break;
		case 's': // sort
			qsort(vector->data, vector->size,
				  vector->element_size, cmp);
			break;
		default:
			printf("No such operation: %s\n", op);
			break;
		}
	}
	free(v);
}

int main(void)
{
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do)
	{
	case 1:
		init_vector(&vector_int, 4, sizeof(int));
		vector_test(&vector_int, n, read_int, int_cmp, is_even);
		print_vector_int(&vector_int);
		free(vector_int.data);
		break;
	case 2:
		init_vector(&vector_char, 2, sizeof(char));
		vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
		print_vector_char(&vector_char);
		free(vector_char.data);
		break;
	case 3:
		init_vector(&vector_person, 2, sizeof(Person));
		vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
		print_vector_person(&vector_person);
		free(vector_person.data);
		break;
	default:
		printf("Nothing to do for %d\n", to_do);
		break;
	}

	return 0;
}

