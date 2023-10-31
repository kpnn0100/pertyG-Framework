#include <chrono>
#include <iostream>
#include <vector>
namespace pertyG
{
    class Timer
    {
    public:
        Timer() : isRunning(false) {}

        void start()
        {
            if (!isRunning)
            {
                isRunning = true;
                startTime = std::chrono::high_resolution_clock::now();
            }
        }

        void stop()
        {
            if (isRunning)
            {
                isRunning = false;
                endTime = std::chrono::high_resolution_clock::now();
            }
        }

        void lap(const std::string &message = "")
        {
            if (isRunning)
            {
                stop();
                log(message);
                start();
            }
        }

        void reset()
        {
            isRunning = false;
        }

        double getDuration() const
        {
            if (isRunning)
            {
                auto now = std::chrono::high_resolution_clock::now();
                return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0;
            }
            else
            {
                return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0;
            }
        }

        void log(const std::string &message = "")
        {
            double duration = getDuration();
            if (!message.empty())
            {
                std::cout << message << ": ";
            }
            std::cout << "Elapsed time: " << duration << " seconds" << std::endl;
        }

    private:
        bool isRunning;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    };
}