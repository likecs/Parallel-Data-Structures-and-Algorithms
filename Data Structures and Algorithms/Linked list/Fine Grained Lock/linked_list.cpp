#include <iostream>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <cassert>
#include <climits>
#include <cmath>
#include <omp.h>
 
using namespace std;

struct node {
	int data;
	omp_lock_t lock;
	struct node *next;
};

void print_list(node *a) {
	if (a == NULL) {
		printf("List is empty\n");
		return;
	}
	printf("Contents : ");
	while (a != NULL) {
		printf("%d ", a->data);
		a = a->next;
	}
	printf("\n");
}

node *create_node(int val) {
	node *new_node = (node*)malloc(sizeof(node));
	new_node->data = val;
	new_node->next = NULL;
	omp_init_lock(&(new_node->lock));
	assert(new_node != NULL);
	return new_node;
}

node* add(node *head, int val) {
	omp_set_lock(&(head->lock));
	node *prev = head;
	node *curr = prev->next;
	omp_set_lock(&(curr->lock));
	while(curr->data < val) {
		omp_unset_lock(&(prev->lock));
		prev = curr;
		curr = curr->next;
		omp_set_lock(&(curr->lock));
	}
	node *ref = create_node(val);
	ref->next = curr;
	prev->next = ref;
	omp_unset_lock(&(curr->lock));
	omp_unset_lock(&(prev->lock));
	return head;
}

node* remove(node *head, int val) { 
	omp_set_lock(&(head->lock));
	node *prev = head;
	node *curr = prev->next;
	omp_set_lock(&(curr->lock));
	while(curr->data < val) {
		omp_unset_lock(&(prev->lock));
		prev = curr;
		curr = curr->next;
		omp_set_lock(&(curr->lock));
	}
	if (curr->data == val) {
		prev->next = curr->next;
		omp_unset_lock(&(curr->lock));	
		free(curr);
	}
	else {
		omp_unset_lock(&(curr->lock));
	}
	omp_unset_lock(&(prev->lock));
	return head;
}

void free_locks(node *head) {
	node *curr = head;
	while(curr != NULL) {
		omp_destroy_lock(&(curr->lock));
		curr = curr->next;
	}
}

int data_set[100001][2];

int main() {
	node *root = create_node(INT_MIN);
	node *temp = create_node(INT_MAX);
	root->next = temp;
	int ctr = 0;
	while(1) {
		scanf("%d", &data_set[ctr][0]);
		if (data_set[ctr][0] == 1 || data_set[ctr][0] == 2) {
			scanf("%d", &data_set[ctr][1]);
			ctr += 1;
		}
		else {
			break;
		}
	}
	double start = omp_get_wtime();
	#pragma omp parallel 
	{
		#pragma omp for schedule(dynamic, 1)
		for(int i = 0; i < ctr; ++i) {
			if (data_set[i][0] == 1) {
				root = add(root, data_set[i][1]);
			}
			else {
				root = remove(root, data_set[i][1]);
			}
		}
	}
	free_locks(root);
	double finish = omp_get_wtime();
	print_list(root);
	cerr << "Time taken : " << finish - start << "\n";
	return 0;
}