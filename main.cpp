//=============================================================================
// axireg - a utility for reading and writing 32-bit AXI registers via UART
//
// Author: D. Wolf        
//=============================================================================
#include <cstdio>
#include <cstring>
#include "axi_uart.h"
using std::string;

CAxiUart AXI;

const uint64_t UNSET = 0xFFFFFFFFFFFFFFFF;

const int OM_NONE = 0;
const int OM_DEC  = 1;
const int OM_HEX  = 2;
const int OM_BOTH = 3;
int output_mode = OM_NONE;

bool isAxiWrite;
 
uint64_t address = UNSET;
uint32_t data;


//=============================================================================
// help() - Show the command line ujsage of this program
//=============================================================================
void help()
{
    printf("usage: axireg [-dec] [-hex] <address>\n");
    printf("       axireg <address> <data>\n");
    printf("\n<address> and <data> can be in hex or decimal\n");
    exit(0);
}
//=============================================================================



//=============================================================================
// parseCommandLine() - Parses the command line parameters
//
// On Exit:  isAxiWrite = true if we are performing a write, false if we're
//                        performing a read
//
//           address    = the AXI address to read/write
//
//           data       = if 'isAxiWrite' is true, the 32-bit data word to be
//                        written
//=============================================================================
void parseCommandLine(const char** argv)
{
    int i=1, index = 0;

    while (true)
    {
        // Fetch the next token from the command line
        const char* token = argv[i++];

        // If we're out of tokens, we're done
        if (token == nullptr) break;

        if (strcmp(token, "-dec") == 0)
        {
            output_mode |= OM_DEC;
            continue;
        }

        if (strcmp(token, "-hex") == 0)
        {
            output_mode |= OM_HEX;
            continue;
        }


        // Store this parameter into either "address" or "data"
        if (++index == 1)
            address = strtoull(token, 0, 0);
        else
        {
            data = strtoul(token, 0, 0);
            isAxiWrite = true;
        }
    }

    // If the user failed to give us an address, that's fatal
    if (address == UNSET) help();
}
//=============================================================================




//=============================================================================
// get_device_name() - Returns the name of the serial device, as read from
//                     an environment variable
//=============================================================================
string get_device_name()
{
    const char* evname = "axi_uart_device";

    // Fetch the environment variable that contains our device name
    char* ev = getenv(evname);
    
    // If that variable doesn't exist, tell the user
    if (ev == nullptr)
    {
        fprintf(stderr, "missing environment variable '%s'\n", evname);
        exit(1);
    }

    // Hand the device name to the caller
    return ev;
}
//=============================================================================





//=============================================================================
// main() - Command line is:
// 
// <program_name> <address> [-dec] [-hex]
//     -- or --
// <program_name> <address> <data>
//=============================================================================
int main(int argc, const char** argv)
{
    int error;

    // Parse the command line
    parseCommandLine(argv);

    // Fetch the device name of our serial port
    string device = get_device_name();

    // Connect to the FPGA via a serial port
    if (!AXI.connect(device, 115200))
    {
        printf("%s not found or no permissions\n", device.c_str());
        exit(1);        
    }

    // If the user wants to perform an AXI write...
    if (isAxiWrite)
    {
        // Perform the AXI write
        error = AXI.write(address, data);

        // If an AXI write-error occured, show the error code
        if (error) printf("Error: write-response = %i\n", error);
    }
    else
    {
        // Perform an AXI read
        error = AXI.read(address, &data);

        // If an AXI read-error occured, show the error code
        if (error) printf("Error: read-response = %i\n", error);

        // Otherwise, display the data value we read
        else switch (output_mode)
        {
            case OM_DEC:   printf("%u\n", data);
                           break;
            case OM_HEX:   printf("%08X\n", data);
                           break;
            case OM_BOTH:  printf("%u %08X\n", data, data);
                           break;
            default:       printf("0x%08X (%u)\n", data, data);
        }

    }

 
    // Tell the OS whether or not this worked
    return error;
}
//=============================================================================

