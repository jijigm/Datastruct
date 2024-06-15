#include <stdio.h>
#include <stdlib.h>
#ifndef LINKEDLIST_C
#define LINKEDLIST_C

#ifndef ELEMENT_TYPE
typedef int element;
#endif

typedef struct DListNode
{
    element data;
    struct DListNode* llink;
    struct DListNode* rlink;
} DListNode;


// ���� ���� ����Ʈ�� �ʱ�ȭ
void init(DListNode* phead)
{
    phead->llink = phead;
    phead->rlink = phead;
}
// ���ο� �����͸� ��� before�� �����ʿ� �����Ѵ�.
void dinsert(DListNode* before)
{
    DListNode* newnode = (DListNode*)malloc(sizeof(DListNode));
    newnode->llink = before;
    newnode->rlink = before->rlink;
    before->rlink->llink = newnode;
    before->rlink = newnode;
}

// ��� removed�� �����Ѵ�.
void ddelete(DListNode* head, DListNode* removed)
{
    if (removed == head)
        return;
    removed->llink->rlink = removed->rlink;
    removed->rlink->llink = removed->llink;
    free(removed);
}


#endif