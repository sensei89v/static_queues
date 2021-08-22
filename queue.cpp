#include "queue.hpp"
#include <string.h>
#include <iostream>
#include <bitset>

#define BLOCK_SIZE 14

typedef unsigned int TConuter;
typedef struct {
    unsigned char data[BLOCK_SIZE];
    unsigned char prev;
    unsigned char next;
} Block;

#define BUFFER_SIZE 2048
#define QUEUE_COUNT 64
#define TABLE_SIZE (QUEUE_COUNT * sizeof(Queue))
#define SHIFT (TABLE_SIZE + sizeof(TConuter))
#define MAX_BLOCK_COUNT (BUFFER_SIZE - SHIFT) / sizeof(Block)
#define NULL_ADDRESS 0x7F
#define get_queue_address(q) (((q) - (Queue*)data) | 0x80)
#define is_queue_address(address) ((address) & 0x80)
#define get_queue(address) (((Queue*)data) + ((address) & 0x7F))


unsigned char data[BUFFER_SIZE];

//------------------------------------------
void on_out_of_memory()
{
    std::cout << "Out of memory\n";
}

void on_illegal_operation()
{
    std::cout << "Illegal operation\n";
}
//-----------------------------------------
TConuter* get_counter()
{
    return (TConuter*)(data + TABLE_SIZE);
}

Block* get_block(unsigned int num)
{
    if (num > MAX_BLOCK_COUNT)
        return NULL;

    return (Block*)(data + SHIFT) + num;
}

unsigned int get_block_address(Block* block)
{
    return block - ((Block*)(data + SHIFT));
}


void delete_block_from_queue(Queue* q, Block* block)
{
    if (q->head == NULL_ADDRESS)
        return;

    if (is_queue_address(block->next)) // last block
    {
        q->head = q->tail = NULL_ADDRESS;
    }
    else
    {
        Block* next_block = get_block(block->next);
        q->head = block->next;
        next_block->prev = get_queue_address(q);
    }
}


void free_block(Block* block)
{
    TConuter* counter = get_counter();
    Block* last_block;
    unsigned int block_address = get_block_address(block);

    if (!*counter)
        return;

    if (block_address != *counter - 1)
    {
        last_block = get_block(*counter - 1);

        if (is_queue_address(last_block->prev))
        {
            Queue* q = get_queue(last_block->prev);
            q->head = block_address;
        }
        else
        {
            Block* prev_last_block = get_block(last_block->prev);
            prev_last_block->next = block_address;
        }

        if (is_queue_address(last_block->next))
        {
            Queue* q = get_queue(last_block->next);
            q->tail = block_address;
        }
        else
        {
            Block* next_last_block = get_block(last_block->next);
            next_last_block->prev = block_address;
        }

        *block = *last_block;
    }
    else
    {
        last_block = block;
    }

    *counter -= 1;
    bzero(last_block, sizeof(Block));
}


Queue *create_queue() // Creates a FIFO byte queue, returning a handle to it.
{
    Queue* q = (Queue*)data;

    for (unsigned int i = 0; i < QUEUE_COUNT; i++)
    {
        if (!q->reserved)
        {
            q->reserved = 1;
            q->head = NULL_ADDRESS;
            q->tail = NULL_ADDRESS;
            q->read_counter = 0;
            q->write_counter = 0;
            return q;
        }
        q++;
    }
    // TODO: think
    on_out_of_memory();
    return NULL;
}


void destroy_queue(Queue *q) // Destroy an earlier created byte queue.
{
    if (!q || !q->reserved) // TODO: another chechs
    {
        on_illegal_operation();
        return;
    }

    while (q->head != NULL_ADDRESS)
    {
        Block* block = get_block(q->head);
        delete_block_from_queue(q, block);
        free_block(block);
    }

    q->reserved = 0;
    q->head = 0;
    q->tail = 0;
    q->read_counter = 0;
    q->write_counter = 0;
}


void enqueue_byte(Queue *q, unsigned char b) // Adds a new byte to a queue.
{
    TConuter* counter;
    Block* block;

    if (!q || !q->reserved) // TODO: another chechs
    {
        on_illegal_operation();
        return;
    }

    if (!q->write_counter)
    {
        counter = get_counter();
        block = get_block(*counter);

        if (!block)
        {
            on_out_of_memory();
            return;
        }

        if (q->tail != NULL_ADDRESS) // non first block
        {
            Block* last_block = get_block(q->tail);
            last_block->next = *counter;
            block->prev = q->tail;
        }
        else
        {
            block->prev = get_queue_address(q);
            q->head = *counter;
        }
        q->tail = *counter;
        block->next = get_queue_address(q);
        *counter += 1;
    }
    else
    {
        block = get_block(q->tail);
    }

    block->data[q->write_counter] = b;
    q->write_counter = (q->write_counter + 1) % BLOCK_SIZE;
}


unsigned char dequeue_byte(Queue *q) // Pops the next byte off the FIFO queue.
{
    TConuter* counter;
    Block* block;
    unsigned char result = 0;

    if (!q || !q->reserved) // TODO: another chechs
    {
        on_illegal_operation();
        return 0;
    }

    if ((q->head == q->tail) && (q->read_counter == q->write_counter))
    {
        on_illegal_operation();
        return 0;
    }

    block = get_block(q->head);
    result = block->data[q->read_counter];
    q->read_counter = (q->read_counter + 1) % BLOCK_SIZE;

    if (!q->read_counter) // free block
    {
        delete_block_from_queue(q, block);
        free_block(block);
    }

    return result;
}



//-------------------------------------------------
void print_all(unsigned int shift, unsigned int size)
{
    for(unsigned int i=0; i < size; i++)
    {
        if (i > 0 && i % 8 == 0)
            std::cout << "\n";

        unsigned char iter = *(data + shift + i);
        std::cout << (std::bitset<8>(iter)) << " ";
    }
    std::cout << "\n";

}
//void print_item(unsigned int shift);
