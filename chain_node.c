#include <stdio.h>
#include <stdlib.h>

struct ChainNode {
    int data;
    struct ChainNode *next;
};

struct ChainNode *chainInit(int *data, size_t len) {

    struct ChainNode *head = NULL;
    struct ChainNode *pos;
    for (size_t i = 0; i < len; ++i) {
        if (head == NULL) {
            head = (struct ChainNode *) malloc(sizeof(struct ChainNode));
            head->data = data[i];
            pos = head;
        } else {
            pos->next = (struct ChainNode *) malloc(sizeof(struct ChainNode));
            pos->next->data = data[i];
            pos = pos->next;
        }
    }
    if (head) pos->next = NULL;
    return head;
}

size_t chainLength(struct ChainNode *chain) {
    size_t size = 0;
    struct ChainNode *pos = chain;
    while (pos != NULL) {
        ++size;
        pos = pos->next;
    }
    return size;
}

int chainAt(struct ChainNode *chain, size_t _pos, int *buffer) {

    struct ChainNode *pos = chain;
    for (size_t i = 0; i < _pos; ++i) {
        pos = pos->next;
    }
    *buffer = pos->data;
    return 0;
}

int chain2Array(struct ChainNode *chain, int *buffer, size_t len) {
    size_t _len = chainLength(chain);
    size_t i;
    struct ChainNode *pos = chain;
    if (len < _len) {
        return -1;
    }
    for (i = 0; i < _len; ++i) {
        buffer[i] = pos->data;
        pos = pos->next;
    }
    return i + 1;
}

int chainInsert(struct ChainNode *chain, size_t _pos, int data) {

    struct ChainNode *pos = chain;
    for (size_t i = 0; i < _pos-1; ++i) {
        pos = pos->next;
    }
    struct ChainNode *ins = (struct ChainNode *) malloc(sizeof(struct ChainNode));
    ins->data = data;
    if (_pos == 0) {
        ins->next = chain;
        chain = ins;
    } else {
        ins->next = pos->next;
        pos->next = ins;
    }
    return 0;
}

int chainDelete(struct ChainNode *chain, size_t _pos) {

    struct ChainNode *pos = chain;
    for (size_t i = 0; i < _pos - 1; ++i) {
        pos = pos->next;
    }
    if (_pos == 1) {
        pos = chain->next;
        free(chain);
        chain = pos;
    } else {
        struct ChainNode *rpos = pos->next->next;
        free(pos->next);
        pos->next = rpos;
    }
    return 0;
}

void chainDestory(struct ChainNode *chain) {
    if (chain == NULL) {
        return;
    }
    struct ChainNode *pos = chain->next, *ele = chain;
    do {
        free(ele);
        ele = pos;
        pos = pos->next;
    } while (pos != NULL);
}
int main(void)
{
    int a[6] = {0x22, 0x3c, 0x3d ,0x3e , 0x40, 0x3E};
    int pos = 0;

    struct ChainNode *chain = chainInit(a, 6);
   
        chainInsert(chain, 1, 0x34);

        for (size_t i = 0; i < chainLength(chain); ++i) {
            int buffer;
             chainAt(chain, i, &buffer);
             printf("%x ", buffer);
        }
        chainDestory(chain);
        printf("\nPress Any Key To Close Window.");
}