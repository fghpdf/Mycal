#ifndef GLNODE_H_INCLUDED
#define GLNODE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define AtomType double
/*
typedef enum
{
	ATOM,
	LIST
}ElemTag;

typedef struct GLNode
{
	ElemTag tag;
	union
	{
		AtomType atom;
		struct 
		{
			struct GLNode *hp,*tp;
		}LNode;
	}atom_LNode;
}GLNode,*GList;

double getvalue(GList L)
{
	if (L == NULL)
	{
		printf("No Number!\n");
		exit(1);
	}
	if (L->tag == 0) return (L->atom_LNode.atom);
}

GLNode* LSCreat(double x)
{
	GLNode *p, *q, *r, *head;

	head = (GLNode*)malloc(sizeof(GLNode));
	if (!head) return NULL;
	head->tag = ATOM;
	head->atom_LNode.atom = x;
}*/
#endif