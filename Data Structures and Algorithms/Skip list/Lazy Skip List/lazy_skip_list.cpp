#include <iostream>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
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
		omp_nest_lock_t lock;
		bool marked, fully_linked;
	};
	node *create_node(int val, int height = MAX_LEVEL) {
		node *new_node = (node *)malloc(sizeof(node));
		new_node->data = val;
		new_node->next = (node **)malloc((height + 1) * sizeof(node *));
		new_node->top_level = height;
		new_node->marked = false;
		new_node->fully_linked = false;
		omp_init_nest_lock(&(new_node->lock));
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
			omp_destroy_nest_lock(&(curr->lock));
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
		while(true) {
			int found = find(val, preds, succs);
			if (found != -1) {
				node *ref = succs[found];
				bool tmp_fully_linked, tmp_marked;
				#pragma omp flush
				#pragma omp atomic read
					tmp_marked = ref->marked;
				if (!tmp_marked) {
					while(1) {
						#pragma omp flush
						#pragma omp atomic read
							tmp_fully_linked = ref->fully_linked;
						if(tmp_fully_linked) {
							break;
						}
					}
					#pragma omp flush
					return false;
				}
				continue;
			}
			int highest_locked = -1;
			node *pred, *succ;
			bool valid = true;
			for(int level = 0; valid && (level <= top_level); ++level) {
				pred = preds[level];
				succ = succs[level];
				omp_set_nest_lock(&(pred->lock));
				highest_locked = level;
				bool tmp_pred_marked, tmp_succ_marked;
				#pragma omp flush
				#pragma omp atomic read
					tmp_pred_marked = pred->marked;
				#pragma omp atomic read
					tmp_succ_marked = succ->marked;
				valid = !(tmp_pred_marked) && !(tmp_succ_marked) && (pred->next[level] == succ);
			}
			if (!valid) {
				for(int level = 0; level <= highest_locked; ++level) {
					omp_unset_nest_lock(&(preds[level]->lock));
				}
				continue;
			}
			node *ref = create_node(val, top_level);
			for(int level = 0; level <= top_level; ++level) {
				ref->next[level] = succs[level];
			}
			for(int level = 0; level <= top_level; ++level) {
				preds[level]->next[level] = ref;
			}
			#pragma omp flush
			#pragma omp atomic write
				ref->fully_linked = true;
			#pragma omp flush
			for(int level = 0; level <= highest_locked; ++level) {
				omp_unset_nest_lock(&(preds[level]->lock));
			}
			return true;
		}
	}
	bool remove(int val) {
		node *victim = NULL;
		bool is_marked = false;
		int top_level = -1;
		node **preds = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		node **succs = (node **)malloc((MAX_LEVEL + 1) * sizeof(node *));
		for(int i = 0; i <= MAX_LEVEL; ++i) {
			preds[i] = (node *)malloc(sizeof(node));
			succs[i] = (node *)malloc(sizeof(node));
		}
		while(true) {
			int found = find(val, preds, succs);
			if (found != -1) {
				victim = succs[found];
			}
			#pragma omp flush
			if (is_marked || (found != -1 && (victim->fully_linked && victim->top_level == found && !(victim->marked)))) {
				if (!is_marked) {
					#pragma omp flush
					#pragma omp atomic read
						top_level = victim->top_level;
					omp_set_nest_lock(&(victim->lock));
					bool tmp_victim_marked;
					#pragma omp flush
					#pragma omp atomic read
						tmp_victim_marked = victim->marked;
					if (tmp_victim_marked) {
						omp_unset_nest_lock(&(victim->lock));
						return false;
					}
					#pragma omp flush
					#pragma omp atomic write
						victim->marked = true;
					#pragma omp flush
					is_marked = true;
				}
				int highest_locked = -1;
				node *pred;
				bool valid = true;
				for(int level = 0; valid && (level <= top_level); ++level) {
					pred = preds[level];
					omp_set_nest_lock(&(pred->lock));
					highest_locked = level;
					bool tmp_pred_marked;
					#pragma omp flush
					#pragma omp atomic read
						tmp_pred_marked = pred->marked;
					valid = !(tmp_pred_marked) && (pred->next[level] == victim);
				}
				if (!valid) {
					for(int i = 0; i <= highest_locked; ++i) {
						omp_unset_nest_lock(&(preds[i]->lock));
					}
					continue;
				}
				for(int level = top_level; level >= 0; --level) {
					preds[level]->next[level] = victim->next[level];
				}
				omp_unset_nest_lock(&(victim->lock));
				// free(victim);
				for(int i = 0; i <= highest_locked; ++i) {
					omp_unset_nest_lock(&(preds[i]->lock));
				}
				return true;
			}
			else {
				return false;
			}
		}
	}
};

int data_set[100001][2];

int main() {
	LazySkipList skip_list;
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
		bool ans;
		#pragma omp for schedule(dynamic) private(ans)
		for(int i = 0; i < ctr; ++i) {
			if (data_set[i][0] == 1) {
				ans = skip_list.add(data_set[i][1]);
			}
			else {
				ans = skip_list.remove(data_set[i][1]);
			}
		}
	}
	double finish = omp_get_wtime();
	skip_list.print();
	cerr << "Time taken : " << finish - start << "\n";
	return 0;
}