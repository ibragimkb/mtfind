#include <iostream>
#include <cstring>

#include "FindApp.h"

#define APP_NAME "mtfind"
#define MAX_PATTERN_LEN (100)

void usage()
{
    std::cerr << "usage: " << APP_NAME << " INPUT_FILE.txt \"pattern\"" << std::endl;
}

int main(int argc, char* argv[])
{
    if (3 != argc) {
        usage();
        return 1;
    }
    if (strlen(argv[2]) > MAX_PATTERN_LEN)
    {
        std::cout << "pattern must be less " << MAX_PATTERN_LEN << " chars" << std::endl;
        return 1;
    }
    FindApp app(500);

    app.run(argv[1], argv[2]);
    app.printResult();
    return 0;
}
