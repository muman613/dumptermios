/**
 * @file        termiosutils.cpp
 * @author      Michael A. Uman
 * @date        July 25, 2019
 */

#include <iostream>
#include <termios.h>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "termiosutils.h"

using namespace std;

typedef struct _flagEntry {
    tcflag_t    flag;
    string      name;
    string      desc;
} FLAG_ENTRY;

static vector<FLAG_ENTRY> iFlags = {
    { IGNBRK,  "IGNBRK",  "Ignore break condition on input", },
    { BRKINT,  "BRKINT",  "Generate SIGINT on break", },
    { IGNPAR,  "IGNPAR",  "Ignore framing and parity errors", },
    { PARMRK,  "PARMRK",  "Mark framing and parity errors.", },
    { INPCK,   "INPCK",   "Enable input parity checking", },
    { ISTRIP,  "ISTRIP",  "Strip off eighth bit", },
    { INLCR,   "INLCR",   "Translate NL to CR on input", },
    { IGNCR,   "IGNCR",   "Ignore CR on input", },
    { ICRNL,   "ICRNL",   "Translate CR to NL on input", },
    { IUCLC,   "IUCLC",   "Map uppercase chars to lowercase on input", },
    { IXON,    "IXON",    "Enable XON/XOFF flow control on output", },
    { IXANY,   "IXANY",   "Any char restarts stopped output", },
    { IXOFF,   "IXOFF",   "Enable XON/XOFF flow control on input", },
    { IMAXBEL, "IMAXBEL", "Ring bell when input queue is full", },
    { IUTF8,   "IUTF8",   "Input is UTF8", },
};

static vector<FLAG_ENTRY> oFlags = {
    { OPOST,   "OPOST",   "Enable implementation defined post-processing", },
    { OLCUC,   "OLCUC",   "Map lowercase chars to uppercase on output", },
    { ONLCR,   "ONLCR",   "Map NL to CR-NL on output", },
    { OCRNL,   "OCRNL",   "Map CR to NL on output", },
    { ONOCR,   "ONOCR",   "Don't output CR at column 0", },
    { ONLRET,  "ONLRET",  "Don't output CR", },
    { OFILL,   "OFILL",   "Send fill character for a delay", },
    { OFDEL,   "OFDEL",   "Fill char is ASCII DLE (0177) [Not implemented]", },
    { NLDLY,   "NLDLY",   "NL delay mask", },
    { CRDLY,   "CRDLY",   "CR delay mask", },
    { TABDLY,  "TABDLY",  "Horizontal tab delay mask", },
    { BSDLY,   "BSDLY",   "Backspace delay mask", },
    { VTDLY,   "VTDLY",   "Vertical tab delay mask", },
    { FFDLY,   "FFDLY",   "Form feed delay mask", },
};

static vector<FLAG_ENTRY> cFlags = {
    { CBAUD,   "CBAUD",   "Baud speed mask", },
    { CBAUDEX, "CBAUDEX", "Extra baud speed mask", },
    { CSIZE,   "CSIZE",   "Character size mask"},
    { CSTOPB,  "CSTOPB",  "Set two stop-bits, rather than one", },
    { CREAD,   "CREAD",   "Enable receiver", },
    { PARENB,  "PARENB",  "Enable parity generation on output, parity checking in input", },
    { PARODD,  "PARODD",  "If set, use odd parity for input & output, otherwise even parity", },
    { HUPCL,   "HUPCL",   "Lower modem control lines after last process closes device", },
    { CLOCAL,  "CLOCAL",  "Ignore modem control lines", },
    { CIBAUD,  "CIBAUD",  "Mask for input speed [Not implemented]", },
    { CMSPAR,  "CMSPAR",  "Use 'stick' parity", },
    { CRTSCTS, "CRTSCTS", "Enable RTS/CTS (hardware) flow control", },
};

static vector<FLAG_ENTRY> lFlags = {
    { ISIG,    "ISIG",    "Generate signal on INTR, QUIT, SUSP, or DSUSP", },
    { ICANON,  "ICANON",  "Enable canonical mode", },
    { XCASE,   "XCASE",   "Convert case [Not Implemented]", },
    { ECHO,    "ECHO",    "Echo input chars", },
    { ECHOE,   "ECHOE",   "If ICANON is set, the ERASE char erases preceding input char", },
    { ECHOK,   "ECHOK",   "If ICANON is set, the KILL character erases current line", },
    { ECHONL,  "ECHONL",  "If ICANON is set, echo the NL char even if ECHO is not set", },
    { ECHOCTL, "ECHOCTL", "If ECHO is set, special chars are echoed as ^X", },
    { ECHOPRT, "ECHOPRT", "If ICANON and ECHO are set, chars are printed as they are erased", },
    { ECHOKE,  "ECHOKE",  "If ICANON is set, KILL is echoed by erasing each char on the line", },
    { FLUSHO,  "FLUSHO",  "Output is being flushed [Not supported on Linux]", },
    { NOFLSH,  "NOFLSH",  "Disable flushing the input and output queues when generating signals", },
    { TOSTOP,  "TOSTOP",  "Send the SIGTTOU signal to the process group of background process", },
    { PENDIN,  "PENDIN",  "All chars in the input queue and reprinted when the next char is read", },
    { IEXTEN,  "IEXTEN",  "Enable implementation-defined input processing.", },
};

/**
 * Baud rate lookup map.
 */
static map<speed_t, int> baudTable = {
    { B50,      50, },
    { B75,      75, },
    { B110,     110, },
    { B134,     134, },
    { B150,     150, },
    { B200,     200, },
    { B300,     300, },
    { B600,     600, },
    { B1200,    1200, },
    { B1800,    1800, },
    { B2400,    2400, },
    { B4800,    4800, },
    { B9600,    9600, },
    { B19200,   19200, },
    { B38400,   38400, },
    { B57600,   57600, },
    { B115200,  115200, },
    { B230400,  230400, },
    { B460800,  460800, },
    { B500000,  500000, },
    { B576000,  576000, },
    { B921600,  921600, },
    { B1000000, 1000000, },
    { B1152000, 1152000, },
    { B1500000, 1500000, },
    { B2000000, 2000000, },
    { B2500000, 2500000, },
    { B3000000, 3000000, },
    { B3500000, 3500000, },
    { B4000000, 4000000, },
};

static int getConsoleWidth() {
    struct winsize sz = {};

    ioctl(0, TIOCGWINSZ, &sz);

    return sz.ws_col;
}

/**
 * Convert baud rate to string representation.
 * @param baud termios baud rate value (see termios.h)
 * @return integer representing the baud rate
 */
int getBaudRate(speed_t baud)
{
    int baud_rate = -1;
    auto it = baudTable.find(baud);

    if (it != baudTable.end()) {
        baud_rate = it->second;
    } else {
        cerr << "Unknown baud value " << baud << endl;
    }

    return baud_rate;
}

/**
 * Dump the termios c_iflags bitmask.
 * @param flag iflags to dump.
 */
static void dumpTermiosIflags(tcflag_t flag)
{
    string sFlags;
    vector<string> vDesc;

    for (auto const & entry : iFlags) {
        if (flag & entry.flag) {
            sFlags += entry.name + " ";
            vDesc.push_back(entry.desc);
        }
    }
    cout << "c_iflags (0x" << setfill('0') << setw(4) << std::hex << flag << std::dec << ") : " << sFlags << endl;
    cout << "Description:" << endl;
    for (auto desc : vDesc) {
        cout << "\t * " << desc << endl;
    }
}

/**
 * Dump the termios c_oflags bitmask.
 * @param flag oflags to dump.
 */
static void dumpTermiosOflags(tcflag_t flag)
{
    string sFlags;
    vector<string> vDesc;

    for (auto const & entry : oFlags) {
        if (flag & entry.flag) {
            sFlags += entry.name + " ";
            vDesc.push_back(entry.desc);
        }
    }
    cout << "c_oflags (0x" << setfill('0') << setw(4) << std::hex << flag << std::dec << ") : " << sFlags << endl;
    cout << "Description:" << endl;
    for (auto desc : vDesc) {
        cout << "\t * " << desc << endl;
    }
}

/**
 * Dump the termios c_cflags bitmask.
 * @param flag cflags to dump.
 */
static void dumpTermiosCflags(tcflag_t flag)
{
    string sFlags;
    vector<string> vDesc;

    for (auto const & entry : cFlags) {
        if (flag & entry.flag) {
            sFlags += entry.name + " ";
            vDesc.push_back(entry.desc);
        }
    }

    cout << "c_cflags (0x" << setfill('0') << setw(4) << std::hex << flag << std::dec << ") : " << sFlags << endl;

    // Create a '8N1' type string representing # of bits, parity, and stop bits.
    {
        string param;

        tcflag_t csmask = (CSIZE & flag);

        switch (csmask) {
            case CS5:
                param += "5";
                break;
            case CS6:
                param += "6";
                break;
            case CS7:
                param += "7";
                break;
            case CS8:
                param += "8";
                break;
        }

        if (flag & PARENB) {
            if (flag & PARODD) {
                param += "O";
            } else {
                param += "E";
            }
        } else {
            param += "N";
        }

        if (flag & CSTOPB) {
            param += "2";
        }  else {
            param += "1";
        }

        cout << "Parameter : " << param << endl;
        cout << "Description:" << endl;
        for (auto desc : vDesc) {
            cout << "\t * " << desc << endl;
        }
    }
}

/**
 * Dump the termios c_lflags bitmask.
 * @param flag lflags to dump.
 */
static void dumpTermiosLflags(tcflag_t flag)
{
    string sFlags;
    vector<string> vDesc;

    for (auto const & entry : lFlags) {
        if (flag & entry.flag) {
            sFlags += entry.name + " ";
            vDesc.push_back(entry.desc);
        }
    }
    cout << "c_lflags (0x" << setfill('0') << setw(4) << std::hex << flag << std::dec << ") : " << sFlags << endl;
    cout << "Description:" << endl;
    for (auto desc : vDesc) {
        cout << "\t * " << desc << endl;
    }
}

vector<string> cc_names = {
    "VINTR",
    "VQUIT",
    "VERASE",
    "VKILL",
    "VEOF",
    "VTIME",
    "VMIN",
    "VSWTC",
    "VSTART",
    "VSTOP",
    "VSUSP",
    "VEOL",
    "VREPRINT",
    "VDISCARD",
    "VWERASE",
    "VLNEXT",
    "VEOL2",
};

void dumpTermiosCC(cc_t * cc) {
    int index = 0;

    cout << "c_cc characters:" << endl;

    for (auto ccname : cc_names) {
        cout << setfill(' ') << setw(12) << ccname << " : 0x" << std::hex << setfill('0') << setw(2) << (int)cc[index++] << endl;
    }
}

static void drawLine() {
    int width = getConsoleWidth();
    // If not able to get width from console, assume it's 80 columns...
    if (width == 0) {
        width = 80;
    }

    cout << string(width - 2, '-') << endl;
}

/**
 * Display dump files information.
 * @param fh File Handle to examine.
 * @return true on success, false if failed
 */
bool dumpTermiosInfo(FILE * fh) {
    bool bRes = false;
    struct termios tio = {};
    std::ios oldState(nullptr);
    oldState.copyfmt(cout);

    if (fread(&tio, sizeof(tio), 1, fh) == 1) {
        drawLine();
        dumpTermiosIflags(tio.c_iflag);
        drawLine();
        dumpTermiosOflags(tio.c_oflag);
        drawLine();
        dumpTermiosCflags(tio.c_cflag);
        drawLine();
        dumpTermiosLflags(tio.c_lflag);
        drawLine();
        dumpTermiosCC(tio.c_cc);

        drawLine();
        cout.copyfmt(oldState);
        cout << "c_ispeed = " << getBaudRate(tio.c_ispeed) << endl;
        cout << "c_ospeed = " << getBaudRate(tio.c_ospeed) << endl;

        bRes = true;
    }

    return bRes;
}
