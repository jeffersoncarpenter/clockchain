#include <iostream>
#include <string>
#include <unistd.h>

#include "crypto++/sha.h"

#include "mine.h"

using CryptoPP::SHA256;

using std::cout;
using std::endl;

std::string addLine(std::string str) {
    return str + "\n";
}

void usage() {
    std::string usageStr;
    usageStr += addLine("Clockchain");
    usageStr += addLine("");
    usageStr += addLine("Blockchain for telling the time.");
    usageStr += addLine("");
    usageStr += addLine("Options:");
    usageStr += addLine("-h: Display this help message.");
    usageStr += addLine("-m: Mine blocks.");
    cout << usageStr;
}

int main(int argc, char **argv) {
    int opt;
    bool mineMode = false;
    opterr = 0;
    while ((opt = getopt(argc, argv, "m")) != -1) {
        switch (opt) {
        case 'm':
            mineMode = true;
            break;
        }
    }
    if (mineMode) {
        mine();
    }
    else {
        usage();
    }
    return 0;
}
