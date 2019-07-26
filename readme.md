# dump termios

This repository contains the sourcecode for the `dumptermios` utility.

The tool reads a binary dump of the `termios` structure stored in a file.

## Sample output
```
$> ./dumptermios /tmp/devicelocator-tio.dat

Dumping termios settings for /tmp/devicelocator-tio.dat
------------------------------------------------------------------------------------
c_iflags (0x0004) : IGNPAR 
Description:
	 * Ignore framing and parity errors
------------------------------------------------------------------------------------
c_oflags (0x0004) : ONLCR 
Description:
	 * Map NL to CR-NL on output
------------------------------------------------------------------------------------
c_cflags (0x1cb2) : CBAUD CBAUDEX CSIZE CREAD HUPCL CLOCAL 
Parameter : 8N1
Description:
	 * Baud speed mask
	 * Extra baud speed mask
	 * Character size mask
	 * Enable receiver
	 * Lower modem control lines after last process closes device
	 * Ignore modem control lines
------------------------------------------------------------------------------------
c_lflags (0x0a30) : ECHOE ECHOK ECHOCTL ECHOKE 
Description:
	 * If ICANON is set, the ERASE char erases preceding input char
	 * If ICANON is set, the KILL character erases current line
	 * If ECHO is set, special chars are echoed as ^X
	 * If ICANON is set, KILL is echoed by erasing each char on the line
------------------------------------------------------------------------------------
c_cc characters:
       VINTR : 0x03
       VQUIT : 0x1c
      VERASE : 0x7f
       VKILL : 0x15
        VEOF : 0x04
       VTIME : 0x00
        VMIN : 0x00
       VSWTC : 0x00
      VSTART : 0x11
       VSTOP : 0x13
       VSUSP : 0x1a
        VEOL : 0x00
    VREPRINT : 0x12
    VDISCARD : 0x0f
     VWERASE : 0x17
      VLNEXT : 0x16
       VEOL2 : 0x00
------------------------------------------------------------------------------------
c_ispeed = 115200
c_ospeed = 115200

```
## Building tool

This project uses `cmake` to build the project.

Simply create a build directory and execute cmake:

```
$> mkdir build
$> cd build
$> cmake ..
$> make
```
