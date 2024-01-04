#pragma once
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;
struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<double> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        double ms = duration.count() * 1000.0f;
        std::cout << "t = " << ms << " ms\n\n";
    }

    static void printTime()
    {
        auto start = std::chrono::high_resolution_clock::now();
        int h = 0;
        int min = 0;
        int s = 0;
        while (true)
        {
            std::this_thread::sleep_for(1s);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;
            s = duration.count();
            h = s / 3600;
            min = s % 3600 / 60;
            s = s % 60;

            if (h != 0)
            {
                std::cout <<h<<" h, "<<min<<" min, "<<s << " s" << std::endl;
            }
            else if (min != 0)
            {
                std::cout << min << " min, " << s << " s" << std::endl;
            }
            else
            {
                std::cout << s << " s" << std::endl;
            }
        }
    }

    static void timeThread()
    {
        auto t = std::thread(printTime);
        t.detach();
    }
};
