/*
 * biTree.h
 *
 *  Created on: 2018. 10. 25.
 *      Author: finegs
 */

#ifndef INCLUDE_BITREE_H_
#define INCLUDE_BITREE_H_

#include <stdlib.h>

typedef struct BiTreeNode_ {
	void	*data;
	struct	BiTreeNode_ *left;
	struct	BiTreeNode_ *right;
} BiTreeNode;

typedef struct BiTree_ {
	int		size;
	int		(*compare)(const void* key1, const void* key2);
	void	(*destroy)(void *data);
	BiTreeNode	*root;
} BiTree;

void biTreeInit(BiTree *tree, void (*destroy)(void *data));
void biTreeDestroy(BiTree *tree);
int biTreeInsertLeft(BiTree *tree, BiTreeNode *node, const void *data);
int biTreeInsertRight(BiTree *tree, BiTreeNode *node, const void *data);
void biTreeRemoveLeft(BiTree *tree, BiTreeNode *node);
void biTreeRemoveRight(BiTree *tree, BiTreeNode *node);
int biTreeMerge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#define biTreeSize(tree)	((tree)->size)
#define	biTreeRoot(tree)	((tree)->root)
#define biTreeIsEob(node)	((node) == NULL)
#define biTreeIsLeaf(node)	((node)->left == NULL && (node)->right == NULL)
#define biTreeData(node)	((node)->data)
#define biTreeLeft(node)	((node)->left)
#define biTreeRight(node)	((node)->right)

#endif /* INCLUDE_BITREE_H_ */
