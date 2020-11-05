#include <stdio.h>
#include <stdlib.h>

typedef struct listnode
{
    int data;
    struct listnode *next;
}LNode;

LNode *create_LinkList(void)
{
    LNode *head = (LNode *)malloc(sizeof(LNode));
    if(NULL == head)
    exit(-1);
    head->next = NULL;
    LNode *r = head;
    int inputData;
    scanf("%d", &inputData);
    while(inputData)
    {
        LNode *p = (LNode *)malloc(sizeof(LNode));
        if(NULL == p)
        exit(-1);
        p->data = inputData;
        p->next = r->next;
        r->next = p;
        r = p;
        scanf("%d", &inputData);
    }

    return head;
}


int main(int argc, char **argv)
{
    LNode *head = create_LinkList();
    LNode *pHead = head;
    pHead = pHead->next;
    while(pHead)
    {
        printf("%d\n", pHead->data);
        pHead = pHead->next;
    }

    return 0;
}