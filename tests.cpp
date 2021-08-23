#include <vector>
#include <string>
#include <iostream>
#include "queue.hpp"

unsigned int on_out_of_memory_counter = 0;
unsigned int on_illegal_operation_counter = 0;

void test_on_out_of_memory()
{
    on_out_of_memory_counter++;
}

void test_on_illegal_operation()
{
    on_illegal_operation_counter++;
}

typedef int (*testfunc)(std::string& error);

typedef struct {
    testfunc func;
    const char* func_name;
} teststruct;

void clean_counters()
{
    on_out_of_memory_counter = 0;
    on_illegal_operation_counter = 0;
}

#define TEST_STRUCT(func) {func, #func}
#define TEST_ASSERT(test, message) do { if (!(test)) { error = message; return 1; }} while (0)

int test_usual_work(std::string& error)
{
    clean_counters();
    Queue* q0 = create_queue();
    enqueue_byte(q0, 0);
    enqueue_byte(q0, 1);
    Queue*q1 = create_queue();
    enqueue_byte(q1, 3);
    enqueue_byte(q0, 2);
    enqueue_byte(q1, 4);
    TEST_ASSERT(dequeue_byte(q0) == 0, "unexpected dequeued byte");
    TEST_ASSERT(dequeue_byte(q0) == 1, "unexpected dequeued byte");
    enqueue_byte(q0, 5);
    enqueue_byte(q1, 6);
    TEST_ASSERT(dequeue_byte(q0) == 2, "unexpected dequeued byte");
    TEST_ASSERT(dequeue_byte(q0) == 5, "unexpected dequeued byte");
    destroy_queue(q0);
    TEST_ASSERT(dequeue_byte(q1) == 3, "unexpected dequeued byte");
    TEST_ASSERT(dequeue_byte(q1) == 4, "unexpected dequeued byte");
    TEST_ASSERT(dequeue_byte(q1) == 6, "unexpected dequeued byte");
    destroy_queue(q1);
    TEST_ASSERT(on_out_of_memory_counter == 0, "unexpected on_out_of_memory_counter");
    TEST_ASSERT(on_illegal_operation_counter == 0, "unexpected on_illegal_operation_counter");
    return 0;
}

int test_destroyed_queue(std::string& error)
{
    clean_counters();
    Queue* q = create_queue();
    enqueue_byte(q, 1);
    destroy_queue(q);
    dequeue_byte(q);
    TEST_ASSERT(on_illegal_operation_counter == 1, "unexpected on_illegal_operation_counter");
    enqueue_byte(q, 3);
    TEST_ASSERT(on_illegal_operation_counter == 2, "unexpected on_illegal_operation_counter");
    destroy_queue(q);
    TEST_ASSERT(on_illegal_operation_counter == 3, "unexpected on_illegal_operation_counter");
    destroy_queue(0);
    enqueue_byte(0, 3);
    destroy_queue(0);
    TEST_ASSERT(on_illegal_operation_counter == 6, "unexpected on_illegal_operation_counter");
    return 0;
}

int test_block_checking(std::string& error)
{
    clean_counters();
    unsigned int q0_counter = 0;
    unsigned int q0_counter_reader = 0;
    unsigned int q1_counter = 0;
    unsigned int q1_counter_reader = 0;
    unsigned int q2_counter = 0;
    unsigned int q2_counter_reader = 0;
    Queue* q0 = create_queue();

    for(unsigned int i = 0; i < 42 /* three blocks */; i++, q0_counter++)
        enqueue_byte(q0, q0_counter);

    Queue* q1 = create_queue();
    for(unsigned int i = 0; i < 28 /* two blocks */; i++, q1_counter++)
        enqueue_byte(q1, q1_counter);

    Queue* q2 = create_queue();
    for(unsigned int i = 0; i < 28 /* two blocks */; i++, q2_counter++)
        enqueue_byte(q2, q2_counter);

    for(unsigned int i = 0; i < 7 /* a half of block */; i++, q0_counter++)
        enqueue_byte(q0, q0_counter);

    for(unsigned int i = 0; i < 7 /* a half of block */; i++, q1_counter++)
        enqueue_byte(q1, q1_counter);

    for(unsigned int i = 0; i < 28 /* two blocks */; i++, q2_counter++)
        enqueue_byte(q2, q2_counter);

    for(unsigned int i = 0; i < 7 /* a half of block */; i++, q1_counter_reader++)
    {
        unsigned char c = dequeue_byte(q1);
        TEST_ASSERT(c == q1_counter_reader, "unexpected dequeued byte");
    }

    for(unsigned int i = 0; i < 28 /* two blocks */; i++, q2_counter++)
        enqueue_byte(q2, q2_counter);

    for(unsigned int i = 0; i < 14 /* a block */; i++, q0_counter_reader++)
    {
        unsigned char c = dequeue_byte(q0);
        TEST_ASSERT(c == q0_counter_reader, "unexpected dequeued byte");
    }

    // q0 = 42 + 7 - 14 = 35
    // q1 = 28 + 7 - 7 = 28
    // q2 = 28 + 28 + 28 = 74
    for(unsigned int i = 0; i < 84; i++, q2_counter_reader++)
    {
        unsigned char c = dequeue_byte(q2);
        TEST_ASSERT(c == q2_counter_reader, "unexpected dequeued byte");
    }

    for(unsigned int i = 0; i < 28; i++, q1_counter_reader++)
    {
        unsigned char c = dequeue_byte(q1);
        TEST_ASSERT(c == q1_counter_reader, "unexpected dequeued byte");
    }

    for(unsigned int i = 0; i < 35; i++, q0_counter_reader++)
    {
        unsigned char c = dequeue_byte(q0);
        TEST_ASSERT(c == q0_counter_reader, "unexpected dequeued byte");
    }

    TEST_ASSERT(on_out_of_memory_counter == 0, "unexpected on_out_of_memory_counter");
    TEST_ASSERT(on_illegal_operation_counter == 0, "unexpected on_illegal_operation_counter");
    dequeue_byte(q0);
    dequeue_byte(q1);
    dequeue_byte(q2);
    TEST_ASSERT(on_illegal_operation_counter == 3, "unexpected on_illegal_operation_counter");
    destroy_queue(q0);
    destroy_queue(q1);
    destroy_queue(q2);
    return 0;
}

int test_empty_many_queues(std::string& error)
{
    clean_counters();
    unsigned int test_number = 23;
    std::vector<Queue*> queues;
    Queue* q;

    for (unsigned int i = 0; i < 64; i++)
    {
        q = create_queue();
        TEST_ASSERT(q, "Can't create queue");
        queues.push_back(q);
    }

    q = create_queue();
    TEST_ASSERT(!q, "Unexpected creating queue");
    TEST_ASSERT(on_out_of_memory_counter == 1, "Unexpected on_out_of_memory_counter counter");

    q = queues[test_number]; // Change queue in middle
    destroy_queue(q);
    queues.erase(queues.begin() + test_number);

    q = create_queue();
    TEST_ASSERT(q, "Can't create queue");
    queues.push_back(q);

    q = create_queue();
    TEST_ASSERT(!q, "Unexpected creating queue");
    TEST_ASSERT(on_out_of_memory_counter == 2, "Unexpected on_out_of_memory_counter counter");

    for(auto q_iter = queues.begin(); q_iter != queues.end(); q_iter++)
        destroy_queue(*q_iter);

    queues.clear();
    TEST_ASSERT(on_out_of_memory_counter == 2, "Unexpected on_out_of_memory_counter counter");
    return 0;
}

int test_big_one_queue(std::string& error)
{
    Queue* q;
    clean_counters();
    q = create_queue();
    unsigned char c = 0, expected_c = 0;

    for (unsigned int i = 0; i < 1611; i++, c++)
    {
        enqueue_byte(q, c);
    }

    TEST_ASSERT(on_out_of_memory_counter == 1, "Unexpected on_out_of_memory_counter counter");

    for (unsigned int i = 0; i < 1610; i++, expected_c++)
    {
        c = dequeue_byte(q);
        TEST_ASSERT(c == expected_c, "Unexpected dequeue byte");
    }

    dequeue_byte(q);
    TEST_ASSERT(on_illegal_operation_counter == 1, "Unexpected on_out_of_memory_counter counter");
    // Test after long sequence
    enqueue_byte(q, 44);
    c = dequeue_byte(q);
    TEST_ASSERT(c == 44, "Unexpected dequeue byte");
    destroy_queue(q);
    return 0;
}

int main()
{
    std::string error;
    unsigned int code;

    if (set_on_illegal_operation(test_on_illegal_operation))
    {
        std::cout << "Fail on setting illegal operation\n";
        return 1;
    }

    if (set_on_out_of_memory(test_on_out_of_memory))
    {
        std::cout << "Fail on setting out of memory\n";
        return 1;
    }

    teststruct testfuncs[] = {
        TEST_STRUCT(test_usual_work),
        TEST_STRUCT(test_destroyed_queue),
        TEST_STRUCT(test_block_checking),
        TEST_STRUCT(test_empty_many_queues),
        TEST_STRUCT(test_big_one_queue)
    };

    for(unsigned int i = 0; i < sizeof(testfuncs) / sizeof(teststruct); i++)
    {
        teststruct current_test_struct = testfuncs[i];
        testfunc test = current_test_struct.func;

        code = test(error);
        std::cout << current_test_struct.func_name << ": ";

        if (code)
        {
            std::cout << "FAIL. " << error << "\n";
            return 1;
        }
        else
        {
            std::cout << "OK\n";
        }
    }

    std::cout << "All test complited\n";
    return 0;
}
