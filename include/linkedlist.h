#include "type.h"

typedef struct LinkedList
{
    usize *head;
} LinkedList;

typedef struct Iter
{
    LinkedList *curr;
} Iter;

typedef struct ListNode
{
    usize *prev;
    usize *curr;
} ListNode;

typedef struct IterMut
{
    ListNode ln;
} IterMut;

LinkedList linkedlist_new();
bool linkedlist_is_empty(LinkedList ll);
void linkedlist_push(LinkedList *ll, usize *item);
usize *linkedlist_pop(LinkedList *ll);
Iter iter(LinkedList ll);
bool iter_has_next(const Iter *it);
LinkedList iter_next(Iter *it);
usize *list_node_pop(ListNode ln);
usize *list_node_value(ListNode ln);
IterMut iter_mut(LinkedList ll);
bool iter_mut_has_next(const IterMut *it);
ListNode iter_mut_next(IterMut *it);