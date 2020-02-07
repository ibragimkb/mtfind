#include "FindApp.h"

static bool resultSort(FindApp::resultParams i, FindApp::resultParams j) { return (i.line<j.line); }

FindApp::FindApp(unsigned int queueSize)
{
    allRead = false;
    maxQueue = queueSize;
    hwConcurrency = std::thread::hardware_concurrency() - 1;
    queueMtx = new std::mutex[hwConcurrency];
    queueData = new std::queue<stringParams>[hwConcurrency];
}

FindApp::~FindApp()
{
    delete[] queueMtx;
    delete[] queueData;
}

void FindApp::run(const std::string &f, const std::string &p)
{
    fileName = f;
    pattern = p;

    std::thread loadthread(staticReadFile, (void*)this);
    std::thread *findthreads;

    findthreads = new std::thread[hwConcurrency];
    for (unsigned int i = 0; i < hwConcurrency; i++)
    {
        findthreads[i] = std::thread(staticFindThread, (void*)this, i);
    }

    loadthread.join();
    for (unsigned int i = 0; i < hwConcurrency; i++)
    {
        findthreads[i].join();
    }
    delete[] findthreads;

    std::sort(result.begin(), result.end(), resultSort);
}

void FindApp::printResult()
{
    std::cout << result.size() << std::endl;
    for (unsigned int i = 0; i < result.size(); i++)
    {
        std::cout << result[i].line << ' ' << result[i].position << ' ' << result[i].s << std::endl;
    }
}

int FindApp::findSubString(std::string &text, std::string &pattern)
{
    int n = text.size();
    int m = pattern.size();
    int i = 0;
    int j = 0, position = 0;

    while (position < n)
    {
        if (text[position] == pattern[j])
        {
            i++;
            j++;
        }
        else if (j < m && pattern[j] == '?')
        {
            i++;
            j++;
        }
        else if (j == m && i == j)
        {
            return (position - i);
        }
        else
        {
            position -= i;
            i = 0;
            j = 0;
        }
        position++;
    }
    return -1;
}


void FindApp::readFile()
{
    std::ifstream file(fileName);
    if (file.is_open())
    {
        std::string text;
        int line = 1;
        bool q;
        unsigned int thread=0;
        while (getline(file, text))
        {
            q = true;
            while (q)
            {
                queueMtx[thread].lock();
                if (queueData[thread].size() < maxQueue)
                {
                    queueData[thread].push({line, text});
                    q = false;
                }
                queueMtx[thread].unlock();
                thread++;
                if (hwConcurrency == thread) thread = 0;
            }
            line++;
            /*std::this_thread::sleep_for(std::chrono::milliseconds(100));*/
        }
        file.close();
    }
    allRead = true;
}

void FindApp::findThread(int thread)
{
    stringParams obj;
    int p;

    while (!allRead || queueData[thread].size())
    {
        if (queueMtx[thread].try_lock()) {
            if (!queueData[thread].empty()) {
                obj = queueData[thread].front();
                queueData[thread].pop();
            }
            queueMtx[thread].unlock();
        }
        if (!obj.s.empty())
        {
            p = findSubString(obj.s, pattern);

            if (p != -1) {
                resultMtx.lock();
                result.push_back({obj.line, p+1, obj.s.substr(p, pattern.size())});
                resultMtx.unlock();
            }
            obj.s.clear();
        }

    }
}

void FindApp::staticReadFile(void* v)
{
    ((FindApp*)v)->readFile();
}

void FindApp::staticFindThread(void* v, int thread)
{
    ((FindApp*)v)->findThread(thread);
}
