//============================================================================
// axi_uart.h - Defines an API for reading/writing AXI registers via UART
//============================================================================
#include <string>
#include "serial_port.h"

class CAxiUart
{
public:

    // Call this to open the connection
    bool    connect(std::string device, uint32_t baud = 115200);

    // Read an AXI register
    int     read(uint64_t address, uint32_t* p_result);

    // Write a value to an AXI register
    int     write(uint64_t address, uint32_t data);

    // Forces the FPGA's serial buffer into a known good state
    void    reset();

    // Closes the serial connection
    void    disconnect();

protected:

    // This provides an interface to a serial port
    CSerialPort sp_;
};
