//============================================================================
// axi_uart.cpp - Implements an API for reading/writing AXI registers via UART
//============================================================================
#include <unistd.h>
#include <string.h>
#include "axi_uart.h"
#include "endian_types.h"
using std::string;

enum {CMD_READ32 = 1, CMD_WRITE32 = 2, CMD_READ64 = 3, CMD_WRITE64 =4 };

// All 1-bit in a 32-bit number
const uint32_t MASK32 = 0xFFFFFFFF;

//============================================================================
// connect() - Opens a connection to the serial port
//============================================================================
bool CAxiUart::connect(string device, uint32_t baud)
{
    // We want serial read/write to be a blocking operation
    sp_.setDefaultReadTimeout(SP_NO_TIMEOUT);
    
    // Open a connection to the specified serial port
    return sp_.open(device, baud);
}
//============================================================================


//============================================================================
// read() - Performs an AXI read transaction
//
// Passed: address  = AXI address to read from
//         p_result = Address where the data read should be stored
//
// Returns: AXI-read-response value. 0 = OKAY
//============================================================================
int CAxiUart::read(uint64_t address, uint32_t* p_result)
{
    #pragma pack(push, 1)
    struct {uint8_t cmd; be_uint32_t addr;} cmd32;
    struct {uint8_t cmd; be_uint64_t addr;} cmd64;
    struct {uint8_t rsp; be_uint32_t data;} response;
    #pragma pack(pop)

    // If this address fits into 32-bits...
    if ((address & MASK32) == address)
    {
        // Fill in the command structure
        cmd32.cmd  = CMD_READ32;
        cmd32.addr = address;

        // Write the AXI-read command to the serial port
        sp_.write(&cmd32, sizeof cmd32);
    }

    // Otherwise, use the version for 64-bit addresses
    else
    {
        // Fill in the command structure
        cmd64.cmd  = CMD_READ64;
        cmd64.addr = address;

        // And write the AXI-read command to the serial port
        sp_.write(&cmd64, sizeof cmd64);        
    }

    // Fetch the response
    sp_.read(&response, sizeof response);

    // Fill in the caller's result field
    *p_result = response.data;

    // And return the error code to the caller
    return response.rsp;
}
//============================================================================


//============================================================================
// write() - Performs an AXI write transaction
//
// Passed: address = AXI address to write to
//         data    = Data to be written to the specified address
//
// Returns: AXI-write-response value. 0 = OKAY
//============================================================================
int CAxiUart::write(uint64_t address, uint32_t data)
{
    #pragma pack(push, 1)
    struct {uint8_t cmd; be_uint32_t addr; be_uint32_t data;} cmd32;
    struct {uint8_t cmd; be_uint64_t addr; be_uint32_t data;} cmd64;
    #pragma pack(pop)

    uint8_t response;

    // If this address fits into 32-bits...
    if ((address & MASK32) == address)
    {
        // Fill in the command structure
        cmd32.cmd  = CMD_WRITE32;
        cmd32.addr = address;
        cmd32.data = data;

        // Write the AXI-write command to the serial port
        sp_.write(&cmd32, sizeof cmd32);
    }

    // Otherwise, use the version for 64-bit addresses
    else
    {
        // Fill in the command structure
        cmd64.cmd  = CMD_WRITE64;
        cmd64.addr = address;
        cmd64.data = data;

        // And write the AXI-write command to the serial port
        sp_.write(&cmd64, sizeof cmd64);        
    }

    // Fetch the response
    sp_.read(&response, 1);

    // And return the error code to the caller
    return response;
}
//============================================================================


//============================================================================
// reset() - Forces the FPGA serial input buffer into a known-good state
//============================================================================
void CAxiUart::reset()
{
    const char* reset_string = "XXXXXXXXXXXXXXXX";

    // Send the reset string to the FPGA
    sp_.write(reset_string, strlen(reset_string));

    // Wait for the FPGA serial-buffer manager to timeout
    usleep(150000);

    // And throw away any response bytes
    sp_.drainInput(100);
}
//============================================================================



