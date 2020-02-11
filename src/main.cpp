#include <iostream>
#include <cstring>

#include "FindApp.h"

#define APP_NAME "mtfind"
#define MAX_PATTERN_LEN (100)
#define QUEUE_LIMIT (1000)

void usage()
{
    std::cerr << "usage: " << APP_NAME << " INPUT_FILE.txt \"pattern\"" << std::endl;
}

bool checkPattern(const char * argv)
{
    if (strlen(argv) < 1)
    {
        std::cout << "pattern cannot be empty" << std::endl;
        return 0;
    }

    if (strlen(argv) > MAX_PATTERN_LEN)
    {
        std::cout << "pattern must be less " << MAX_PATTERN_LEN << " chars" << std::endl;
        return 0;
    }
    if (strchr(argv, '\n') != NULL)
    {
        std::cout << "pattern cannot contain line feed character" << std::endl;
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    if (3 != argc) {
        usage();
        return 0;
    }
    if (!checkPattern(argv[2])) return 0;

    FindApp app(QUEUE_LIMIT);

    app.run(argv[1], argv[2]);
    app.printResult();
    return 1;
}
