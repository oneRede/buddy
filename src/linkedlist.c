#include "type.h"

typedef struct LinkedList
{
    usize *head;
} LinkedList;

LinkedList new()
{
    LinkedList ll = {.head = NULL};
}

i32 is_empty(LinkedList* ll)
{
    if (ll->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void push(LinkedList* ll, usize* item) {
    *item = ll->head;
    ll->head = item;
}