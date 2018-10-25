/*
 * bitree.c
 *
 *  Created on: 2018. 10. 25.
 *      Author: finegs
 */
#include <stdlib.h>
#include <string.h>

#include "bitree.h"

void biTreeInit(BiTree *tree, void (*destroy)(void *data)) {
	tree->size  = 0;
	tree->destroy  = destroy;
	tree->root = NULL;
}

void biTreeDestroy(BiTree *tree) {
	biTreeRemoveLeft(tree, NULL);
	memset(tree, 0, sizeof(BiTree));
	return;
}

int biTreeInsertLeft(BiTree *tree, BiTreeNode *node, const void *data) {
	BiTreeNode *new_node,
				**position;

	if(NULL == node) {
		if(biTreeSize(tree) > 0) return -1;
		position = &tree->root;
	}
	else {
		if(biTreeLeft(node) != NULL) return -1;
		position = &node->left;
	}


	if((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
		return -1;


	new_node->data = (void*)data;
	new_node->left  = NULL;
	new_node->right = NULL;
	*position = new_node;

	tree->size++;

	return 0;
}

int biTreeInsertRight(BiTree *tree, BiTreeNode *node, const void *data) {
	BiTreeNode *newNode,
				**position;

	/****************************************
	 * Determine where to insert the node
	 ****************************************/
	if(NULL == node) {
		/***********
		 * Allow insertion at the root only in an empty tree.
		 * **********************/
		if(biTreeSize(tree) > 0) {
			return -1;
		}
		position = &tree->root;
	}
	else {
		if(biTreeRight(node) != NULL) {
			return -1;
		}
		position = &node->right;
	}

	if((newNode = (BiTreeNode*)malloc(sizeof(BiTreeNode))) == NULL)
		return-1;

	newNode->data = (void*)data;
	newNode->left = NULL;
	newNode->right = NULL;
	*position = newNode;

	tree->size++;
	return 0;
}


void biTreeRemoveLeft(BiTree *tree, BiTreeNode *node) {
	BiTreeNode **position;

	if(biTreeSize(tree) == 0) {
		return;
	}

	if(NULL==node) position = &tree->root;
	else position = &node->left;

	if(NULL != *position) {
		biTreeRemoveLeft(tree, *position);
		biTreeRemoveRight(tree, *position);
		if(tree->destroy != NULL) {
			tree->destroy((*position)->data);
		}
	}

	free(*position);
	*position = NULL;

	tree->size--;
	return;
}
void biTreeRemoveRight(BiTree *tree, BiTreeNode *node) {
	BiTreeNode **position;

	if(biTreeSize(tree) == 0) {
		return;
	}

	if(NULL==node) position = &tree->root;
	else position = &node->right;

	if(NULL != *position) {
		biTreeRemoveLeft(tree, *position);
		biTreeRemoveRight(tree, *position);
		if(tree->destroy != NULL) {
			tree->destroy((*position)->data);
		}
	}

	free(*position);
	*position = NULL;

	tree->size--;
	return;
}

int biTreeMerge(BiTree *merge, BiTree *left, BiTree *right, const void *data) {
	biTreeInit(merge, left->destroy);

	if(biTreeInsertLeft(merge, NULL, data) != 0) {
		biTreeDestroy(merge);
		return -1;
	}

	biTreeRoot(merge)->left = biTreeRoot(left);
	biTreeRoot(merge)->right = biTreeRoot(right);

	merge->size = merge->size + biTreeSize(left) + biTreeSize(right);

	left->root = NULL;
	left->size = 0;
	right->root = NULL;
	right->size = 0;

	return 0;
}

