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
 * Convert baud rate to string representation.
 * @param baud termios baud rate value (see termios.h)
 * @return string representing the baud rate in bps
 */
const char * getBaudRate(uint32_t baud)
{
    switch (baud) {
        case B50:
            return "50";
        case B75:
            return "75";
        case B110:
            return "110";
        case B134:
            return "134";
        case B150:
            return "150";
        case B200:
            return "200";
        case B300:
            return "300";
        case B600:
            return "600";
        case B1200:
            return "1200";
        case B1800:
            return "1800";
        case B2400:
            return "2400";
        case B4800:
            return "4800";
        case B9600:
            return "9600";
        case B19200:
            return "19200";
        case B38400:
            return "38400";
        case B57600:
            return "57600";
        case B115200:
            return "115200";
        case B230400:
            return "230400";
        case B460800:
            return "460800";
        case B500000:
            return "500000";
        case B576000:
            return "576000";
        case B921600:
            return "921600";
        case B1000000:
            return "1000000";
        case B1152000:
            return "1152000";
        case B1500000:
            return "1500000";
        case B2000000:
            return "2000000";
        case B2500000:
            return "2500000";
        case B3000000:
            return "3000000";
        case B3500000:
            return "3500000";
        case B4000000:
            return "4000000";
    }
}

/**
 * Dump the termios c_iflags bitmask.
 * @param flag iflags to dump.
 */
static void dumpTermiosIflags(tcflag_t flag)
{
    // cerr << __func__ << endl;
//    cout << "iFlags = " << flag << " 0x" << std::hex << flag << std::dec << endl;
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
/**
 * Display dump files information.
 * @param fh File Handle to examine.
 * @return true on success, false if failed
 */
bool dumpTermiosInfo(FILE * fh) {
    bool bRes = false;
    struct termios tio = {};

    if (fread(&tio, sizeof(tio), 1, fh) == 1) {
        dumpTermiosIflags(tio.c_iflag);
        dumpTermiosOflags(tio.c_oflag);
        dumpTermiosCflags(tio.c_cflag);
        dumpTermiosLflags(tio.c_lflag);
        dumpTermiosCC(tio.c_cc);

        cout << "c_ispeed = " << getBaudRate(tio.c_ispeed) << endl;
        cout << "c_ospeed = " << getBaudRate(tio.c_ospeed) << endl;
    }

    return bRes;
}
