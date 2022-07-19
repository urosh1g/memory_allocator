#include <stdio.h>
#include <assert.h>
#include "./include/mem.h"

typedef struct node {
	int value;
	struct node* next;
} node_t;

typedef struct ll {
	node_t* head;
	node_t* tail;
} linked_list_t;

node_t* node_create();
linked_list_t* list_create();
void list_append(linked_list_t**, int);
int list_remove_head(linked_list_t**);
void list_iterate(linked_list_t**, void (*f)(int));
void list_free(linked_list_t**);

void print_int(int val){
	printf("%d->", val);
}

int main(){
	linked_list_t* list = list_create();
	for(int i = 0; i < 10; i++){
		list_append(&list, i);
	}
	print_blocks();
	list_iterate(&list, &print_int);
	list_free(&list);
	print_blocks();
	return 0;
}

linked_list_t* list_create(){
	linked_list_t* list = (linked_list_t*)mem_alloc(sizeof(linked_list_t));
	list->head = list->tail = NULL;
	return list;
}

node_t* node_create(){
	node_t* node = (node_t*)mem_alloc(sizeof(node_t));
	node->next = NULL;
	node->value = 0;
	return node;
}

void list_append(linked_list_t** l, int val){
	assert(*l != NULL);
	linked_list_t* list = *l;
	if(!list->head){
		list->head = list->tail = mem_alloc(sizeof(node_t));
		list->head->value = val;
		return;
	}
	node_t* node = (node_t*)mem_alloc(sizeof(node_t));
	node->value = val;
	list->tail->next = node;
	list->tail = list->tail->next;
}

int list_remove_head(linked_list_t** l){
	assert(*l != NULL);
	linked_list_t* list = *l;
	node_t* to_remove = list->head;
	list->head = list->head->next;
	int val = to_remove->value;
	mem_free(to_remove);
	return val;
}
void list_iterate(linked_list_t** l, void (*f)(int)){
	assert(*l != NULL);
	linked_list_t* list = *l;
	node_t* iter = list->head;
	while(iter){
		f(iter->value);
		iter = iter->next;
	}
	return;
}

void list_free(linked_list_t** l){
	assert(*l != NULL);
	linked_list_t* list = *l;
	while(list->head){
		list_remove_head(&list);
	}
	mem_free(list);
	return;
}

