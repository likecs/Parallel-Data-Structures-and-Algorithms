#include <iostream>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <cassert>
#include <climits>
#include <cmath>
#include <omp.h>
 
using namespace std;

//denote maximum levels in skip-list
//should be about O(log n)
const int MAX_LEVEL = 20;

class LazySkipList {
private:
	struct node {
		int data;
		node **next;
		int top_level;
	};
	node *create_node(int val, int height = MAX_LEVEL) {
		node *new_node = (node *)malloc(sizeof(node));
		new_node->data = val;
		new_node->next = (node **)malloc((height + 1) * sizeof(node *));
		new_node->top_level = height;
		for(int i = 0; i <= new_node->top_level; ++i) {
			new_node->next[i] = NULL;
		}
		assert(new_node != NULL);
		return new_node;
	}
	void print_node(node *temp) {
		cout << temp->data << " " << temp->top_level << "\n";
	}
public:
	int get_random_height() {
		static int bits = 0;
		static int reset = 0;
		int height, found = 0;
		for(height = 0; !found; ++height) {
			if (reset == 0) {
				bits = rand();
				reset = sizeof(int) * CHAR_BIT;
			}
			found = bits & 1;
			bits = bits >> 1;
			--reset;
		}
		if (height >= MAX_LEVEL) {
			height = MAX_LEVEL - 1;
		}
		return height;
	}
	node *head, *tail;
	LazySkipList() {
		head = create_node(INT_MIN);
		tail = create_node(INT_MAX);
		for(int i = 0; i <= head->top_level; ++i) {
			head->next[i] = tail;
		}
	}
	~LazySkipList() {
		node *curr = head, *prev;
		while(curr != NULL) {
			prev = curr;
			curr = curr->next[0];
			free(prev->next);
			free(prev);
		}
	}
	void print() {
		printf("Contents: ");
		node *curr = head;
		while(curr != NULL) {
			printf("%d ", curr->data);
			curr = curr->next[0];
		}	
		printf("\n");
	}
	int find(int val, node **preds, node **succs) {
		int found = -1;
		node *prev = head;
		for(int level = MAX_LEVEL; level >= 0; --level) {
			node *curr = prev->next[level];
			while(curr->data < val) {
				prev = curr;
				curr = prev->next[level];
			}
			if (found == -1 && curr->data == val) {
				found = level;
			}
			preds[level] = prev;
			succs[level] = curr;
		}
		return found;
	}
	bool add(int val) {
		int top_level = get_random_height();
		node **preds = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		node **succs = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		for(int i = 0; i <= MAX_LEVEL; ++i) {
			preds[i] = (node *)malloc(sizeof(node));
			succs[i] = (node *)malloc(sizeof(node));
		}
		int found = find(val, preds, succs);
		if (found != -1) {
			return false;
		}
		node *ref = create_node(val, top_level);
		for(int level = 0; level <= top_level; ++level) {
			ref->next[level] = succs[level];
		}
		for(int level = 0; level <= top_level; ++level) {
			preds[level]->next[level] = ref;
		}
		return true;
	}
	bool remove(int val) {
		node *victim = NULL;
		int top_level = -1;
		node **preds = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		node **succs = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		for(int i = 0; i <= MAX_LEVEL; ++i) {
			preds[i] = (node *)malloc(sizeof(node));
			succs[i] = (node *)malloc(sizeof(node));
		}
		int found = find(val, preds, succs);
		if (found != -1) {
			victim = succs[found];
			top_level= victim->top_level;
			for(int level = top_level; level >= 0; --level) {
				preds[level]->next[level] = victim->next[level];
			}
			free(victim->next);
			free(victim);
			return true;
		}
		else {
			return false;
		}
	}
};

int data_set[100001][2];

int main() {
	LazySkipList skip_list;
	int ctr;
	scanf("%d", &ctr);
	for(int i = 0; i < ctr; ++i) {
		scanf("%d", &data_set[i][0]);
		scanf("%d", &data_set[i][1]);
	}
	double start = omp_get_wtime();
	bool ans;
	for(int i = 0; i < ctr; ++i) {
		if (data_set[i][0] == 1) {
			ans = skip_list.add(data_set[i][1]);
		}
		else {
			ans = skip_list.remove(data_set[i][1]);
		}
	}
	double finish = omp_get_wtime();
	skip_list.print();
	cerr << "Sequential time taken : " << finish - start << "\n";
	return 0;
}