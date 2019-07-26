#include <iostream>
#include "termiosutils.h"

using namespace std;

int main(int argc, char * argv[]) {
    if (argc != 2) {
        cerr << "Must specify termios file to analyze." << endl;
        return 10;
    }

    FILE * termiosFile = nullptr;

    termiosFile = fopen(argv[1], "r");
    if (termiosFile) {
        cout << "Dumping termios settings for " << argv[1] << endl;
        dumpTermiosInfo(termiosFile);
    }

    return 0;
}
