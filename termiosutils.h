//
// Created by developer on 7/25/19.
//

#ifndef DUMPTERMIOS_TERMIOSUTILS_H
#define DUMPTERMIOS_TERMIOSUTILS_H

#include <stdio.h>
#include <termios.h>
#include <string>

bool dumpTermiosInfo(FILE * fh);
bool dumpTermiosInfo(struct termios * tio);
bool dumpTermiosInfo(std::string filename);

#endif //DUMPTERMIOS_TERMIOSUTILS_H
