A utility for reading/writing 32-bit AXI registers via UART
---

1. Clone this repository
2. cd *repo-folder* ; make
3. Copy the resulting 'axireg' executable somewhere into your $PATH
4. Plug in your Nexys-A7, making sure its powered on
5. ls -l /dev/ttyUSB*, to figure out which TTY device it uses. (Probably /dev/ttyUSB1)
6. At the bottom of your "~/.bashrc" file, add "export axi_uart_device=*the_tty_device_name*"
7. Close the shell window you're using
