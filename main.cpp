#include <iostream>
#include <cstring>
#include "termiosutils.h"

using namespace std;

int main(int argc, char * argv[]) {
    int result = 0;

    if (argc != 2) {
        cerr << "Must specify termios file to analyze." << endl;
        return 10;
    }

    FILE * termiosFile = nullptr;

    termiosFile = fopen(argv[1], "r");
    if (termiosFile) {
        cout << "Dumping termios settings for " << argv[1] << endl;
        if (!dumpTermiosInfo(termiosFile)) {
            cerr << "An error occurred..." << endl;
            result = 15;
        }
    } else {
        cerr << "ERROR: " << strerror(errno) << endl;
    }

    return result;
}
