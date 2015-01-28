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
void insertLeft(Node **head, Node *right, Node *insert);
void insertRight(Node **head, Node *left, Node *insert);
void insertIndex(Node **head, Node *insert, int index);

// Node Delete
void deleteNode(Node **head, Node **node);
void deleteIndex(Node **head, int index);
void deleteValue(Node **head, int value);

// Node Search
Node *findIndex(Node **head, int index);
Node *findValue(Node **head, int value);

// Helpers
void printNodes(char *title, Node **head);
void updateIndexes(Node **head);

int main() {
	
    int listLength = 3;
	Node *link_a = link(111);
	Node *link_b = link(222);
	Node *link_c = link(200);
	Node *link_d = link(333);

	// Initial list
	Node *list = linkedList(listLength);
	printNodes("Initial list", &list);

	// Adding to end of linked list
	insertIndex(&list, link_a, listLength+1);
	printNodes("Adding link_a(value: 111) to TAIL", &list);

	// Adding to left of head (new head)
	insertLeft(&list, list, link_b);
	printNodes("Adding link_b(value: 222) to HEAD", &list);
	
	// Adding to right of list[2]
	insertRight(&list, findIndex(&list, 4), link_c);
	printNodes("Adding link_c(value: 200) to list[4]->next", &list);

	// Insert after the first Node of value '111'
	insertRight(&list, findValue(&list, 111), link_d);
	printNodes("Adding link_d(value: 333) to (value: 111)->next", &list);
 

	// Delete list head
	deleteIndex(&list, 0);
	printNodes("Deleting list head link_b(value: 222)", &list);

    // Delete list[3]
	deleteIndex(&list, 3);
	printNodes("Deleting list[3] link_a(value: 111)", &list);

	// Delete all nodes with value 200
	deleteValue(&list, 200);
	printNodes("Deleting all nodes satisfying (value: 200)", &list);

	return 0;
}

//
void insertLeft(Node **head, Node *right, Node *insert) {
	if (right->prev) {
		Node *left = right->prev;
		left->next = insert;
		insert->prev = left;
	} else {
		*head = insert;
	}

	right->prev = insert;
	insert->next = right;

	updateIndexes(head);
}

//
void insertRight(Node **head, Node *left, Node *insert) {
	if (left->next) {
		Node *right = left->next;
		right->prev = insert;
		insert->next = right;
	}

	left->next = insert;
	insert->prev = left;

	updateIndexes(head);
}

//
void insertIndex(Node **head, Node *insert, int index) {
	updateIndexes(head);
	Node *oldNode = *head;
	
	if (index == 0) {
		insertLeft(head, *head, insert);
        return;
	}

	while(oldNode->next) {
		if (oldNode->index == index) {
			insertLeft(head, oldNode, insert);
            return;
		}
		oldNode = oldNode->next;
	}

	insertRight(head, oldNode, insert);
}

//
Node *findValue(Node **head, int value) {
	updateIndexes(head);
    Node *current = *head;
	while(current) {
		if (current->value == value) {
			return current;
		}
		current = current->next;
	}
	
	return NULL;
}

//
Node *findIndex(Node **head, int index) {
	updateIndexes(head);
    Node *current = *head;
	while(current) {
		if (current->index == index) {
			return current;
		}
		current = current->next;
	}

	return NULL;
}

//
void deleteIndex(Node **head, int index) {
    Node *node = findIndex(head, index);
	deleteNode(head, &node);
}

//
void deleteValue(Node **head, int value) {
    Node *node = findValue(head, value);
	while( node ) {
	    deleteNode(head, &node);
        node = findValue(head, value);
	}
}

//
void deleteNode(Node **head, Node **node) {
	Node *left, *right;
    
    // Invalid node
	if(!(*node)) {
		return;
	}

	// Only head
	if(*node == *head && !(*head)->next) {
		free(*head);
		return;
	}

	// Removing head
	if(*node == *head) {
		right = (*head)->next;
		right->prev = NULL;
        *head = right;
		free(*node);
		updateIndexes(head);
		return;
	}

	// Removing tail 
	if(!(*node)->next) {
		left = (*node)->prev;
		left->next = NULL;
		free(*node);
		return;
	}

	// Remove body section
    left = (*node)->prev;
    right = (*node)->next;
	left->next = right;
	right->prev = left;
	free(*node); // << --- Uncomment this line and hell breaks loose....
	updateIndexes(head);
}

//
void updateIndexes(Node **head) {
	int i = 0;
    Node *current = *head;
	while(current) {
		current->index = i++;
		current = current->next;
	}
}

//
void printNodes(char *title, Node **head) {
	Node *current = *head;
	printf("\n@@@ %s\n", title);
	while(current) {
		printf( "\n" );
		if (current->prev) {
			printf( "  ^[%p]^\n", current->prev );
		} else {
            printf( " ____________________\n" );
        }

		printf(	"|                    |\n"
				"|    index: % 5d    |\n"
				"|    value: % 5d    |\n" 
				"|                    |\n",
				current->index, current->value );
		if (current->next) {
			printf( "  v[%p]v\n", current->next );
		} else {
            printf( "|____________________|\n" );
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
