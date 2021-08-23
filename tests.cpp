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


int test_empty_many_queues(std::string& error)
{
    clean_counters();
    error = "";
    unsigned int test_number = 23;
    std::vector<Queue*> queues;
    Queue* q;

    for (unsigned int i = 0; i < 64; i++)
    {
        q = create_queue();

        if (!q)
        {
            error = "Can't create queue";
            return 1;
        }
        queues.push_back(q);
    }

    q = create_queue();

    if (q)
    {
        error = "Unexpected creating queue";
        return 1;
    }

    if (on_out_of_memory_counter != 1)
    {
        error = "Unexpected on_out_of_memory_counter counter";
        return 1;
    }

    q = queues[test_number]; // Change queue in middle
    destroy_queue(q);
    queues.erase(queues.begin() + test_number);
    q = create_queue();

    if (!q)
    {
        error = "Can't create queue";
        return 1;
    }
    queues.push_back(q);

    q = create_queue();

    if (q)
    {
        error = "Unexpected creating queue";
        return 1;
    }

    if (on_out_of_memory_counter != 2)
    {
        error = "Unexpected on_out_of_memory_counter counter";
        return 1;
    }

    for(auto q_iter = queues.begin(); q_iter != queues.end(); q_iter++)
        destroy_queue(*q_iter);

    queues.clear();

    if (on_out_of_memory_counter != 2)
    {
        error = "Unexpected on_out_of_memory_counter counter";
        return 1;
    }
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
    c = 0;

    if (on_out_of_memory_counter != 1)
    {
        error = "Unexpected on_out_of_memory_counter counter";
        return 1;
    }

    for (unsigned int i = 0; i < 1610; i++, expected_c++)
    {
        c = dequeue_byte(q);
        if (c != expected_c)
        {
            error = "Unexpected dequeue byte";
            return 1;
        }
    }

    dequeue_byte(q);
    if (on_illegal_operation_counter != 1)
    {
        error = "Unexpected on_out_of_memory_counter counter";
        return 1;
    }
    destroy_queue(q);
    // TODO: add + 1
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
