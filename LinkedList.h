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


// 이중 연결 리스트를 초기화
void init(DListNode* phead)
{
    phead->llink = phead;
    phead->rlink = phead;
}
// 새로운 데이터를 노드 before의 오른쪽에 삽입한다.
void dinsert(DListNode* before)
{
    DListNode* newnode = (DListNode*)malloc(sizeof(DListNode));
    newnode->llink = before;
    newnode->rlink = before->rlink;
    before->rlink->llink = newnode;
    before->rlink = newnode;
}

// 노드 removed를 삭제한다.
void ddelete(DListNode* head, DListNode* removed)
{
    if (removed == head)
        return;
    removed->llink->rlink = removed->rlink;
    removed->rlink->llink = removed->llink;
    free(removed);
}


#endif