#include <stdio.h>
#include <malloc.h>

struct List{
    int vertex;
    struct List *next;
};

typedef struct List *llink;

void insert_list(llink *list, int item)
{
    llink new = malloc(sizeof(struct List));
    if(!new)
        return;
    new->vertex = item;
    new->next = NULL;
    if(!*list)
    {
        *list = new;
        return;
    }
    llink cur = *list;
    while(cur->next)
        cur = cur->next;
    cur->next = new;
}

llink get_middle_item(llink list)
{
    llink one_p=list, two_p=list;
    while(two_p)
    {
        two_p = two_p->next;
        if(two_p)
            two_p = two_p->next;
        if(two_p)
            one_p = one_p->next;
    }
    return one_p;
}

llink get_klast_item(llink list, int k)
{
    int i=1;
    llink cur=list, curk=list;
    while(i++ <= k && cur)
        curk = curk->next;
    if(i>k && !curk)
        return cur;
    if(!curk)
        return NULL;
    while(curk)
    {
        cur = cur->next;
        curk = curk->next;
    }
    return cur;
}

int main()
{
    int data[] = {1,2,3,4,5,6,7};
    int size = sizeof(data)/sizeof(int);
    llink list = NULL;
    int i;
    for(i=0; i<size; i++)
        insert_list(&list, data[i]);
    llink mid = get_middle_item(list);
    printf("middle=%d\n", mid->vertex);
    llink k;
    for(i=1; i<=size; i++)
    {
        k = get_klast_item(list, i);
        if(k)
            printf("%dlast=%d\n", i, k->vertex);
    }

    return 0;
}
