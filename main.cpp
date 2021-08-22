#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include "queue.hpp"


int main() {
    std::string command;
    std::vector<Queue*> queue_list;

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
            unsigned int num = std::stoi(snum);

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
            std::cin >> snum;
            std::cin >> value;

            unsigned int num = std::stoi(snum);

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

            unsigned int num = std::stoi(snum);
            unsigned int count = std::stoi(scount);
            unsigned int start = std::stoi(sstart);

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
            std::string tnum;
            std::cin >> tnum;
            unsigned int num = std::stoi(tnum);

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

            unsigned int num = std::stoi(snum);
            unsigned int count = std::stoi(scount);
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
        else if (command == "pa")
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
