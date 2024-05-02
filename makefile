#-----------------------------------------------------------------------------
# Base name of the program 
#-----------------------------------------------------------------------------
EXE = axireg
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Name of the x86 executable file
#-----------------------------------------------------------------------------
X86EXE = $(EXE)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Find out what kind of hardware we're compiling on
#-----------------------------------------------------------------------------
PLATFORM := $(shell uname)

#-----------------------------------------------------------------------------
# For x86, declare whether to emit 32-bit or 64-bit code
#-----------------------------------------------------------------------------
X86_TYPE = 64

#-----------------------------------------------------------------------------
# Declare where the object files get created
#-----------------------------------------------------------------------------
X86_OBJ_DIR = obj_x86

#-----------------------------------------------------------------------------
# Tell 'make' what suffixes will appear in make rules
#-----------------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .cpp

#-----------------------------------------------------------------------------
# Declare the compile-time flags that are common between all platforms
#-----------------------------------------------------------------------------
CXXFLAGS =	\
-O2 -g -Wall \
-Wno-write-strings \
-Wno-sign-compare \
-Wno-unused-result \
-Wno-strict-aliasing \
-std=c++11 \
-fpermissive \
-fcommon \
-DLINUX 

#-----------------------------------------------------------------------------
# If there was no goal on the command line, the default goal is this
#-----------------------------------------------------------------------------
.DEFAULT_GOAL := x86

#-----------------------------------------------------------------------------
# Define the name of the compiler and what "build all" means for our platform
#-----------------------------------------------------------------------------
ifeq ($(PLATFORM), Linux)
    ALL    = x86
endif


#-----------------------------------------------------------------------------
# We're going to compile every .cpp file in this folder
#-----------------------------------------------------------------------------
SRC_FILES := $(shell ls *.cpp)

#-----------------------------------------------------------------------------
# Create the base-names of the object files
#-----------------------------------------------------------------------------
OBJ_FILES = $(SRC_FILES:.cpp=.o)

#-----------------------------------------------------------------------------
# We are going to keep x86 and ARM object files in separate sub-directories
#-----------------------------------------------------------------------------
X86_OBJS = $(addprefix $(X86_OBJ_DIR)/,$(OBJ_FILES))

#-----------------------------------------------------------------------------
# This rules tells how to compile an X86 .o object file from a .cpp source
#-----------------------------------------------------------------------------
$(X86_OBJ_DIR)/%.o : %.cpp
	$(CXX) -m$(X86_TYPE) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------
# This rule builds the x86 executable from the object files
#-----------------------------------------------------------------------------
$(X86EXE) : $(X86_OBJS)
	$(CXX) -m$(X86_TYPE) -pthread -o $@ $(X86_OBJS)
	strip $(X86EXE)

 .PHONY : clean x86

#-----------------------------------------------------------------------------
# This target builds all executables supported by this platform
#-----------------------------------------------------------------------------
all:	$(ALL)

#-----------------------------------------------------------------------------
# This target builds just the x86 executable
#-----------------------------------------------------------------------------
x86:	mkdirs $(X86EXE)

#-----------------------------------------------------------------------------
# This target configures the object file directories
#-----------------------------------------------------------------------------
$(X86_OBJ_DIR):
	@mkdir $(X86_OBJ_DIR) $(X86_OBJ_DIR)/common
	

#-----------------------------------------------------------------------------
# This target configured the object file directories
#-----------------------------------------------------------------------------
mkdirs:	$(X86_OBJ_DIR)
	@chmod 777 $(X86_OBJ_DIR) $(X86_OBJ_DIR)/common
	@touch     $(X86_OBJ_DIR)/CACHEDIR.TAG $(X86_OBJ_DIR)/common/CACHEDIR.TAG

#-----------------------------------------------------------------------------
# This target removes all files that are created at build time
#-----------------------------------------------------------------------------
clean:
	rm -rf Makefile.bak makefile.bak $(X86EXE)
	rm -rf $(X86_OBJ_DIR) 

    
#-----------------------------------------------------------------------------
# This target appends/updates the dependencies list at the end of this file
#-----------------------------------------------------------------------------
depend:
	makedepend -p$(X86_OBJ_DIR)/ -Y *.cpp 2>/dev/null


# DO NOT DELETE

obj_x86/axi_uart.o: axi_uart.h serial_port.h
obj_x86/main.o: axi_uart.h serial_port.h
obj_x86/serial_port.o: serial_port.h