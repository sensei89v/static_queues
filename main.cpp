#include <iostream>
#include <string>
#include <vector>
#include "queue.hpp"


int main() {
    std::string command;
    std::vector<Queue*> queue_list;
    std::cout << "Available commands:\n"
                 "q - create a queue. We will store 'Queue object' in vector. All '<queue number>' corresponds to  position in the vector, not in the internal storage.\n"
                 "f <queue number> - destroy the queue.\n"
                 "a <queue number> <byte> - append a byte to the queue.\n"
                 "aa <queue number> <count bytes> <start byte> - append an increasing sequence of bytes to a queue starting with 'start byte'.\n"
                 "d <queue number> - extract a byte from the queue.\n"
                 "dd <queue number> <count> - extract a sequence of bytes.\n"
                 "e - exit.\n";
    while (1)
    {
        std::cin >> command;

        if (command == "q")
        {
            Queue* q = create_queue();

            if (q)
                queue_list.push_back(q);
            else
                std::cout << "Cann't create queue\n";
        }
        else if (command == "f")
        {
            std::string snum;
            std::cin >> snum;
            unsigned int num;

            try
            {
                num = std::stoi(snum);
            }
            catch (...)
            {
                std::cout << "Invalid number\n";
                continue;
            }

            if (num >= queue_list.size())
            {
                std::cout << "Invalid queue number\n";
            }
            else
            {
                destroy_queue(queue_list[num]);
                queue_list.erase(queue_list.begin() + num);
            }
        }
        else if (command == "a")
        {
            std::string snum, value;
            unsigned int num;
            std::cin >> snum;
            std::cin >> value;

            try
            {
                num = std::stoi(snum);
            }
            catch (...)
            {
                std::cout << "Invalid number\n";
                continue;
            }

            if (num >= queue_list.size())
                std::cout << "Invalid queue number\n";
            else
                enqueue_byte(queue_list[num], std::stoi(value));
        }
        else if (command == "aa")
        {
            std::string snum, scount, sstart;
            std::cin >> snum;
            std::cin >> scount;
            std::cin >> sstart;
            unsigned int num, count, start;

            try
            {
                num = std::stoi(snum);
                count = std::stoi(scount);
                start = std::stoi(sstart);
            }
            catch (...)
            {
                std::cout << "Invalid number\n";
                continue;
            }

            if (num >= queue_list.size())
            {
                std::cout << "Invalid queue number\n";
            }
            else
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    enqueue_byte(queue_list[num], start);
                    start++;
                }
            }
        }
        else if (command == "d")
        {
            std::string snum;
            std::cin >> snum;
            unsigned int num;

            try
            {
                num = std::stoi(snum);
            }
            catch (...)
            {
                std::cout << "Invalid number\n";
                continue;
            }

            if (num >= queue_list.size())
            {
                std::cout << "Invalid queue number\n";
            }
            else
            {
                unsigned char c = dequeue_byte(queue_list[num]);
                std::cout << int(c) << "\n";
            }
        }
        else if (command == "dd")
        {
            std::string snum, scount;
            std::cin >> snum;
            std::cin >> scount;
            unsigned int num, count;

            try
            {
                num = std::stoi(snum);
                count = std::stoi(scount);
            }
            catch (...)
            {
                std::cout << "Invalid number\n";
                continue;
            }

            if (num >= queue_list.size())
            {
                std::cout << "Invalid queue number\n";
            }
            else
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    unsigned char c = dequeue_byte(queue_list[num]);
                    std::cout << int(c) << "\n";
                }
            }
        }
        else if (command == "e")
        {
            return 0;
        }
        else if (command == "pa")   // TODO: DELETE!!!!
        {
            std::string size, shift;
            std::cin >> shift;
            std::cin >> size;
            print_all(std::stoi(shift), std::stoi(size));
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }
}
