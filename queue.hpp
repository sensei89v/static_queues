#ifndef QUEUE_HPP
#define QUEUE_HPP

typedef struct __attribute__((__packed__)) {
    unsigned char head: 7;
    unsigned char reserved: 1;
    unsigned char tail;
    unsigned char read_counter: 4;
    unsigned char write_counter: 4;
} Queue;

typedef void (*queue_error_fn)();

Queue *create_queue(); // Creates a FIFO byte queue, returning a handle to it.
void destroy_queue(Queue *q); // Destroy an earlier created byte queue.
void enqueue_byte(Queue *q, unsigned char b); // Adds a new byte to a queue.
unsigned char dequeue_byte(Queue *q); // Pops the next byte off the FIFO queue.

int set_on_out_of_memory(queue_error_fn fn);
int set_on_illegal_operation(queue_error_fn fn);
#endif
