#include "list.h"

#include <stdlib.h>

typedef struct listCDT {
    void    *head;
    listADT  tail;
} listCDT;

void *ListHead(listADT list) {
    return list->head;
}

listADT ListTail(listADT list) {
    return list->tail;
}

void ListAdd(listADT list, void *value) {
    while (list->tail) {
        list = list->tail;
    }

    listCDT *tail = malloc(sizeof(listCDT));

    tail->head = value;
    tail->tail = NULL;

    list->tail = tail;
}

int ListLength(listADT list) {
    int length = 0;
    while (list) {
        list = list->tail;
        length++;
    }

    return length;
}
