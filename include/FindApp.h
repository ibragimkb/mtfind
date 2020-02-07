#ifndef FINDAPP_H
#define FINDAPP_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <algorithm>
#include <fstream>

class FindApp
{
    public:
        struct resultParams
        {
            int line, position;
            std::string s;
        };

        FindApp(unsigned int queueSize = 1000);
        virtual ~FindApp();

        void run(const std::string &f, const std::string &p);
        void printResult();

        unsigned int GetCounter() { return m_Counter; }
        void SetCounter(unsigned int val) { m_Counter = val; }

    protected:
        void readFile();
        int findSubString(std::string &text, std::string &pattern);
        void findThread(int thread);
        static void staticReadFile(void* v);
        static void staticFindThread(void* v, int thread);

    private:
        struct stringParams
        {
            int line;
            std::string s;
        };
        unsigned int m_Counter;
        std::string fileName;
        std::string pattern;
        bool allRead;
        unsigned int maxQueue;
        unsigned int hwConcurrency;
        std::mutex *queueMtx;
        std::queue<stringParams> *queueData;
        std::mutex resultMtx;
        std::vector<resultParams> result;
};

#endif // FINDAPP_H
