DEBUG ?= yes
PLATFORM ?= unix
OBJ_DIR ?= obj
PROJECT_BIN_DIR ?= build/$(PLATFORM)/bin
BIN_DIR = $(ROOT_DIR)/$(PROJECT_BIN_DIR)
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-implicit-fallthrough
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS = -std=c++14
LDFLAGS = 
LIBS = -L$(ROOT_DIR)/$(PROJECT_BIN_DIR)

ifeq ($(DEBUG), no)
CFLAGS += -O2
else
CFLAGS += -O0 -ggdb
LDFLAGS += -ggdb
endif

ifeq ($(CCBLD), yes)
CFLAGS += -fprofile-arcs -ftest-coverage
endif


#######################################
# binaries
#######################################
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CXX ?= $(GCC_PATH)/$(PREFIX)g++
CC ?= $(GCC_PATH)/$(PREFIX)gcc
AS ?= $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
SZ ?= $(GCC_PATH)/$(PREFIX)size
OBJCOPY ?= $(GCC_PATH)/$(PREFIX)objcopy
OBJDUMP ?= $(GCC_PATH)/$(PREFIX)objdump
READELF ?= $(GCC_PATH)/$(PREFIX)readelf
STRIP ?= $(GCC_PATH)/$(PREFIX)strip
GDB ?= $(GCC_PATH)/$(PREFIX)gdb
AR ?= $(GCC_PATH)/$(PREFIX)ar
NM ?= $(GCC_PATH)/$(PREFIX)nm
else
CXX ?= $(PREFIX)g++
CC ?= $(PREFIX)gcc
AS ?= $(PREFIX)gcc -x assembler-with-cpp
SZ ?= $(PREFIX)size
OBJCOPY ?= $(PREFIX)objcopy
OBJDUMP ?= $(PREFIX)objdump
READELF ?= $(PREFIX)readelf
STRIP ?= $(PREFIX)strip
GDB ?= $(PREFIX)gdb
AR ?= $(PREFIX)ar
NM ?= $(PREFIX)nm
endif
