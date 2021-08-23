#include <vector>
#include <string>
#include <iostream>
#include "queue.hpp"


typedef int (*testfunc)(std::string& error);

typedef struct {
    testfunc func;
    const char* func_name;
} teststruct;

#define TEST_STRUCT(func) {func, #func}


int test_empty_many_queues(std::string& error)
{
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

    for(auto q_iter = queues.begin(); q_iter != queues.end(); q_iter++)
        destroy_queue(*q_iter);

    queues.clear();
    return 0;
}

int test_big_one_queue(std::string& error)
{
    Queue* q;
    q = create_queue();
    unsigned char c = 0, expected_c = 0;

    for (unsigned int i = 0; i < 1610; i++, c++)
    {
        enqueue_byte(q, c);
    }
    c = 0;

    for (unsigned int i = 0; i < 1610; i++, expected_c++)
    {
        c = dequeue_byte(q);
        if (c != expected_c)
        {
            error = "Unexpected dequeue byte";
            return 1;
        }
    }

    destroy_queue(q);
    // TODO: add + 1
    return 0;
}

int main()
{
    std::string error;
    unsigned int code;

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
