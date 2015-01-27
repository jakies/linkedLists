#include <stdio.h>
#include <stdlib.h>

/* TO-DO
	findValue is broken
*/


struct node {
	int index, value;
	struct node *prev, *next;
};

typedef struct node Node;

// List creator
Node *linkedList(int length);
// Node creator
Node *link(int value);

// Node Insert
Node *insertLeft(Node *head, Node *right, Node *insert);
Node *insertRight(Node *head, Node *left, Node *insert);
Node *insertIndex(Node *head, Node *insert, int index);

// Node Delete
Node *deleteNode(Node *head, Node *node);
Node *deleteIndex(Node *head, int index);
Node *deleteValue(Node *head, int value);

// Node Search
Node *findIndex(Node *head, int index);
Node *findValue(Node *head, int value);

// Helpers
void printNodes(char *title, Node *head);
void updateIndexes(Node *head);

int main() {

	int listLength = 3;
	Node *link_a = link(111);
	Node *link_b = link(222);
	Node *link_c = link(200);
	Node *link_d = link(333);

	// Initial list
	Node *list = linkedList(listLength);
	printNodes("Initial list", list);

	// Adding to end of linked list
	list = insertIndex(list, link_a, listLength+1);
	printNodes("Adding link_a(value: 111) to TAIL", list);

	// Adding to left of head (new head)
	list = insertLeft(list, list, link_b);
	printNodes("Adding link_b(value: 111) to HEAD", list);
	
	// Adding to right of list[2]
	list = insertRight(list, findIndex(list, 4), link_c);
	printNodes("Adding link_c(value: 200) to list[4]->next", list);

	// Insert after the first Node of value '111'
	//list = insertRight(list, findValue(list, 111), link_c);
	//printNodes("Adding link_d(value: 333) to (value: 111)->next", list);
 

	// Delete list head
	list = deleteIndex(list, 0);
	printNodes("Deleting list head link_a(value: 111)", list);

	// Delete all nodes with value 200
	list = deleteValue(list, 200);
	printNodes("Deleting all nodes(value: 200)", list);

	return 0;
}

//
Node *insertLeft(Node *head, Node *right, Node *insert) {
	if (right->prev) {
		Node *left = right->prev;
		left->next = insert;
		insert->prev = left;
	} else {
		head = insert;
	}

	right->prev = insert;
	insert->next = right;

	updateIndexes(head);

	return head;
}

//
Node *insertRight(Node *head, Node *left, Node *insert) {
	if (left->next) {
		Node *right = left->next;
		right->prev = insert;
		insert->next = right;
	}

	left->next = insert;
	insert->prev = left;

	updateIndexes(head);

	return head;
}

//
Node *insertIndex(Node *head, Node *insert, int index) {
	updateIndexes(head);
	Node *oldNode = head;
	
	if (index == 0) {
		return insertLeft(head, head, insert);
	}

	while(oldNode->next) {
		if (oldNode->index == index) {
			return insertLeft(head, oldNode, insert);
		}
		oldNode = oldNode->next;
	}

	return insertRight(head, oldNode, insert);
}

//
Node *findValue(Node *head, int value) {
	updateIndexes(head);
	while(head) {
		if (head->value == value) {
			return head;
		}
		head = head->next;
	}
	
	return NULL;
}

//
Node *findIndex(Node *head, int index) {
	updateIndexes(head);
	while(head) {
		if (head->index == index) {
			return head;
		}
		head = head->next;
	}

	return NULL;
}

//
Node *deleteIndex(Node *head, int index) {
	return deleteNode(head, findIndex(head, index));
}

//
Node *deleteValue(Node *head, int value) {
	while( findValue(head, value) ) {
		head = deleteNode(head, findValue(head, value));
	}
	return head;
}

//
Node *deleteNode(Node *head, Node *node) {
	Node *left, *right;

	// Invalid node
	if(!node) {
		return head;
	}

	// Only head
	if(node == head && !head->next) {
		free(head);
		return NULL;
	}

	// Removing head
	if(node == head) {
		right = head->next;
		right->prev = NULL;
		free(head);
		updateIndexes(right);
		return right;
	}

	// Removing tail 
	if(!node->next) {
		left = head->prev;
		left->next = NULL;
		free(node);
		return head;
	}

	// Remove body section
	left = node->prev;
	right = node->next;
	left->next = right;
	right->prev = left;
	free(node);
	updateIndexes(head);
	
	return head;
}

//
void updateIndexes(Node *head) {
	int i = 0;
	while(head) {
		head->index = i++;
		head = head->next;
	}
}

//
void printNodes(char *title, Node *head) {
	Node *current = head;
	printf("\n@@@ %s\n", title);
	while(current) {
		printf( "\n" );
		if (current->prev) {
			printf( "  ^[%p]^\n", current->prev );
		}
		printf(	" ----------------\n"
				"|  index: % 5d  |\n"
				"|  value: % 5d  |\n" 
				" ----------------\n", 
				current->index, current->value );
		if (current->next) {
			printf( "  v[%p]v\n", current->next );
		}

		current = current->next;
	}
	printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

Node *link(int value) {
	Node *newLink = malloc(sizeof(struct node));
	newLink->value = value;
	return newLink;
}

//
Node *linkedList(int length) {
	int i = 0;

	Node *left = malloc(sizeof(struct node));
	Node *head = left;
	left->index = i;
	left->value = (i + 1) * 100;

	while(++i < length) {
		Node *current = malloc(sizeof(struct node));

		current->index = i;
		current->value = (i + 1) * 100;

		current->prev = left;
		left->next = current;

		left = current;
	}

	return head;
};
