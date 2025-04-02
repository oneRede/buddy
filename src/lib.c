#include "type.h"
#include "layout.h"
#include "linkedlist.h"

#define ORDER 33

typedef struct Heap
{
    LinkedList free_list[ORDER];
    usize user;
    usize allocated;
    usize total;
} Heap;

Heap heap_new()
{
    Heap hp = {
        .free_list = {0},
        .user = 0,
        .allocated = 0,
        .total = 0};
    for (i32 i = 0; i < ORDER; i++)
    {
        hp.free_list[i] = linkedlist_new();
    }
    return hp;
}

Heap heap_empty()
{
    Heap hp = {
        .free_list = {0},
        .user = 0,
        .allocated = 0,
        .total = 0};
    return hp;
}

void add_to_heap(Heap *hp, usize start, usize end)
{
    start = (start + sizeof(usize) - 1) & (!sizeof(usize) + 1);
    end &= !sizeof(usize) + 1;
    assert(start <= end);

    usize total = 0;
    usize current_start = start;

    while ((current_start + sizeof(usize)) <= end)
    {
        usize lowbit = current_start & (!current_start + 1);
        usize size = min(lowbit, prev_power_of_two(end - current_start));

        usize order = __builtin_ctz(size);
        if (order > ORDER - 1)
        {
            order = ORDER - 1;
            size = 1 << order;
        }
        total += size;
        linkedlist_push(&hp->free_list[order], (usize *)current_start);
        current_start += size;
    }
    hp->total += total;
}

void init(Heap *hp, usize start, usize end)
{
    add_to_heap(hp, start, end);
}

u8 *alloc(Heap *hp, Layout layout)
{
    usize size = max(next_power_of_two(layout.size), max(layout.align, sizeof(usize)));
    usize class = __builtin_ctz(size);

    for (i32 i = class; i < ORDER; i++)
    {
        if (!linkedlist_is_empty(hp->free_list[i]))
        {
            for (i32 j = i + 1; j > class + 1; j--)
            {
                usize *block = linkedlist_pop(&hp->free_list[j]);
                if (block != NULL)
                {
                    linkedlist_push(&hp->free_list[j - 1], (usize *)((usize)block + (1 << (j - 1))));
                    linkedlist_push(&hp->free_list[j - 1], block);
                }
                else
                {
                    return NULL;
                }
            }
            u8 *result = linkedlist_pop(&hp->free_list[class]);
            if (result != NULL)
            {
                hp->user += layout.size;
                hp->allocated += size;
            }
            else
            {
                return NULL;
            }
        }
    }
}

void dealloc(Heap *hp, u8 *ptr, Layout layout)
{
    usize size = max(next_power_of_two(layout.size), max(layout.align, sizeof(usize)));
    usize class = __builtin_ctz(size);

    linkedlist_push(&hp->free_list[class], ptr);

    usize current_ptr = ptr;
    usize current_class = class;

    while (current_class < ORDER - 1)
    {
        usize buddy = current_ptr ^ (1 << current_class);
        bool flag = false;
        IterMut imt = iter_mut(hp->free_list[class]);

        while (true)
        {
            ListNode block = iter_mut_next(&imt);
            if (block.prev == NULL && block.curr == NULL)
            {
                break;
            }
            if (list_node_value(block) == buddy)
            {
                list_node_pop(block);
                flag = true;
                break;
            }
        }

        if (flag)
        {
            linkedlist_pop(&hp->free_list[current_class]);
            current_ptr = min(current_ptr, buddy);
            current_class += 1;
            linkedlist_push(&hp->free_list[current_class], (usize *)current_ptr);
        }
        else
        {
            break;
        }
    }
    hp->user -= layout.size;
    hp->allocated -= size;
}

usize stats_alloc_user(Heap *hp) {
    return hp->user;
}

usize stats_alloc_actual(Heap *hp) {
    return hp->allocated;
}

usize stats_total_bytes(Heap * hp) {
    return hp->total;
}