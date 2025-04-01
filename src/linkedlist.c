#include "type.h"

typedef struct LinkedList
{
    usize *head;
} LinkedList;

LinkedList new()
{
    LinkedList ll = {.head = NULL};
}

bool is_empty(LinkedList *ll)
{
    if (ll->head == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void push(LinkedList *ll, usize *item)
{
    *item = ll->head;
    ll->head = item;
}

usize *pop(LinkedList *ll)
{
    switch (is_empty(ll))
    {
    case true:
        return NULL;
    case false:
        usize *item = ll->head;
        ll->head = *item;
        return item;
    default:
        return NULL;
    }
}

typedef struct Iter
{
    LinkedList *curr;
} Iter;

Iter iter(LinkedList ll)
{
    Iter it;
    it.curr = ll.head;
    return it;
}

bool iter_has_next(const Iter *it)
{
    return it->curr != NULL;
}

LinkedList iter_next(Iter *it)
{
    if (!list_iterator_has_next(it))
    {
        LinkedList ll = {.head = NULL};
        return ll;
    }
    LinkedList ll = {.head = it->curr};
    it->curr = it->curr->head;
    return ll;
}

typedef struct ListNode
{
    usize *prev;
    usize *curr;
} ListNode;

usize *list_node_pop(ListNode ln)
{
    *ln.prev = *ln.curr;
    return ln.curr;
}

usize *list_node_value(ListNode ln)
{
    return ln.curr;
}

typedef struct IterMut
{
    ListNode ln;
} IterMut;

IterMut iter_mut(LinkedList ll)
{
    IterMut it;
    it.ln.prev = (usize *)&(ll.head);
    it.ln.curr = ll.head;
    return it;
}

bool iter_mut_has_next(const IterMut *it)
{
    return it->ln.curr != NULL;
}

ListNode iter_mut_next(IterMut *it)
{
    if (!list_iterator_has_next(it))
    {
        ListNode ll = {.prev = NULL,
                       .curr = NULL};
        return ll;
    }
    ListNode ll = {.prev = it->ln.prev,
                   .curr = it->ln.curr};
    it->ln.prev = it->ln.curr;
    it->ln.curr = (usize *)*it->ln.curr;
    return ll;
}