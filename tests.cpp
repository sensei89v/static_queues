#include <vector>
#include <string>
#include "queue.hpp"


#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)
extern int tests_run;

// int main(int argc, char **argv) {
//     char *result = all_tests();
//     if (result != 0) {
//         printf("%s\n", result);
//     }
//     else {
//         printf("ALL TESTS PASSED\n");
//     }
//     printf("Tests run: %d\n", tests_run);
//
//     return result != 0;
// }

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

int main()
{
    std::string error;
    unsigned int code;
    code = test_empty_many_queues(error);
    return 0;
}
