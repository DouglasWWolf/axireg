//============================================================================
// serial_port.h - Defines an API for raw serial I/O services
//============================================================================
#pragma once
#include <termios.h>
#include <string>

//============================================================================
// Handy constants used for describing timeout values
//============================================================================
#define SP_DEFAULT_TIMEOUT -2
#define SP_NO_TIMEOUT      -1
//============================================================================


//============================================================================
// Class CSerialPort - Provides an API to a UART
//============================================================================
class CSerialPort
{
public:

    // Constructor and destructor
    CSerialPort();
    ~CSerialPort();

    // Call this to set the default timeout for functions that read data
    void    setDefaultReadTimeout(uint32_t milliseconds);

    // Call this to open a connection.  Returns 'false' on error
    bool    open(std::string device, uint32_t baud);

    // Call this to close a connection
    void    close();

    // Throws away data coming from the serial port
    void    drainInput(int timeout_ms);

    // Writes a line of text to the serial port. Caller append
    // carriage return or line feed if needed
    void    putLine(const void* line);

    // Writes a line of printf()-style text to the serial port.  Caller
    // appends cr/lf if needed
    void    printf(const char* fmt, ...);

    // Fetches a line of text from the serial port. Strips cr/lf off the end
    bool    getLine(void* buffer, int timeout_ms = SP_DEFAULT_TIMEOUT);

    // Call this to fetch the file descriptor of the UART
    int     getFD() {return fd_;}

    // Fetches one character from the serial port
    int     getChar(int timeout_ms = SP_DEFAULT_TIMEOUT);

    // Puts a single character to the serial port
    void    putChar(int byte);

    // Reads a specified number of bytes from the serial port
    bool    read(void* buffer, int count, int timeout_ms = SP_DEFAULT_TIMEOUT);

    // Writes a specified number of bytes from the serial port
    void    write(const void* buffer, int count);

    // Enable sniffing
    void    enableSniffing(bool flag) {sniff_ = flag;}

protected:

    // This returns 'true' if data is available to be read in.
    // If timeout_ms = -1, this routine will wait forever for data to
    // be available
    bool    dataIsAvailable(int timeout_ms);

    // Converts an integer baud-rate to one of the termios speed constants
    speed_t baudToConstant(uint32_t baud_rate);

    // File descriptor we use to read/write serial data
    int     fd_;

    // If this is 'true', all incoming characters will be printed
    bool    sniff_;

    // This is the default timeout in milliseconds
    uint32_t default_timeout_ms_ = SP_NO_TIMEOUT;
};
//============================================================================


