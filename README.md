# static_queues
### Task

The problem is to write a set of functions to manage a variable number of byte queues, each with variable length, in a small, fixed amount of memory. You should provide implementations of the following four functions:

Q *create_queue(); // Creates a FIFO byte queue, returning a handle to it.
void destroy_queue(Q *q); // Destroy an earlier created byte queue.
void enqueue_byte(Q *q, unsigned char b); // Adds a new byte to a queue.
unsigned char dequeue_byte(Q *q); // Pops the next byte off the FIFO queue.

So, the output from the following set of calls:

Q *q0 = create_queue();
enqueue_byte(q0, 0);
enqueue_byte(q0, 1);
Q *q1 = create_queue();
enqueue_byte(q1, 3);
enqueue_byte(q0, 2);
enqueue_byte(q1, 4);
printf("%d", dequeue_byte(q0));
printf("%d\n", dequeue_byte(q0));
enqueue_byte(q0, 5);
enqueue_byte(q1, 6);
printf("%d", dequeue_byte(q0));
printf("%d\n", dequeue_byte(q0));
destroy_queue(q0);
printf("%d", dequeue_byte(q1));
printf("%d", dequeue_byte(q1));
printf("%d\n", dequeue_byte(q1));
destroy_queue(q1);

should be:

0 1
2 5
3 4 6

You can define the type Q to be whatever you want.

Your code is not allowed to call malloc() or other heap management routines.
Instead, all storage (other than local variables in your functions) must be within a provided array:

unsigned char data[2048];

Memory efficiency is important. On average while your system is running, there will be about 15 queues with an average of 80 or so bytes in each queue. Your functions may be asked to create a larger number of queues with less bytes in each. Your functions may be asked to create a smaller number of queues with more bytes in each.

Execution speed is important. Worst-case performance when adding and removing bytes is more important than average-case performance.

If you are unable to satisfy a request due to lack of memory, your code should call a provided failure function, which will not return:

void on_out_of_memory();

If the caller makes an illegal request, like attempting to dequeue a byte from an empty queue, your code should call a provided failure function, which will not return:

void on_illegal_operation();

There may be spikes in the number of queues allocated, or in the size of an individual queue. Your code should not assume a maximum number of bytes in a queue (other than that imposed by the total amount of memory available, of course!) You can assume that no more than 64 queues will be created at once.


### The storage description
The storage contains from following parts:

* Header - an array of 3byte queue items. Each item  has: reserving queue bit, 2 7-bit addresses to the head and the tail data block and reading and writing counters describing the position in correspondent blocks.

* 4-byte counter of blocks. Of course the one 1 byte is enough. But 4 bytes counter aligns blocks in data.

* Data blocks. Each block contains 14 bytes of data and two 1-byte pointers to next and previous  data blocks. The first bit of each pointer corresponds to the destination. 0 means pointing to another data block. 1 means pointing to the header (For the first and last blocks in a queue).

|--Header: 64 * 3 bytes--|--Block counter: 4 bytes--|--Data blocks: 115 * 16 bytes--|--Free 12 bytes--|

When we need a new block we just increase the counter and change pointers.
When we delete a block we move the last block in storage to place of the deleting block.

### How to build
`make`

### How to run

`./queue_tester`

queue_tester - is a small tool to demonstrate work with the static queue.
It works in interactive mode and supports following commands:

 * `q` - create a queue. We will store 'Queue object' in vector. All '<queue number>' corresponds to  position in the vector, not in the internal storage
 * `f <queue number>` - destroy the queue
 * `a <queue number> <byte>` - append a byte to the queue.
 * `aa <queue number> <count bytes> <start byte>` - append an increasing sequence of bytes to a queue starting with 'start byte'.
 * `d <queue number>` - extract a byte from the queue
 * `dd <queue number> <count>` - extract a sequence of bytes
 * `e` - exit

### How to run tests

`./tests`

### TODO, what can improve
Add more tests
