#
# To build rexjsondfa.h use:
# rexcc rexjsondfa.rexcc -o rexjsondfa.h
#

	
	
#######################################
# binaries
#######################################
PREFIX = 
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CXX = $(GCC_PATH)/$(PREFIX)g++
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CXX = $(PREFIX)g++
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif

BASENAME = librexjson
TARGET_SO = $(BASENAME).so
TARGET_LIB = $(BASENAME).a

######################################
# building variables
######################################
# debug build?
DEBUG ?= 1
# optimization
OPT = -O2


#######################################
# paths
#######################################
# Build path
BUILD_DIR = bin
SRC_DIR = ../..

C_DEFS =

C_INCLUDES += #$(shell python3 -m pybind11 --includes)
C_INCLUDES += -I..
C_INCLUDES += $(shell python3-config --includes)
C_INCLUDES += -I../extern/pybind11/include


LDFLAGS = -shared -fPIC
CFLAGS = $(C_DEFS) $(C_INCLUDES) -Wall -fPIC --shared -std=gnu++14

ifeq ($(DEBUG), 1)
CFLAGS += -g -O0
else
CFLAGS += $(OPT)

endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


OBJECTS = $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(notdir $(wildcard ./*.c))))
vpath %.c commandline/

OBJECTS += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard ./*.cpp))))
vpath %.cpp ./

# default action: build all
all: $(BUILD_DIR)/$(TARGET_SO) $(BUILD_DIR)/$(TARGET_LIB)

$(BUILD_DIR)/$(TARGET_SO): $(OBJECTS) Makefile | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -Wall -shared -std=c++11 -o $@

$(BUILD_DIR)/$(TARGET_LIB): $(OBJECTS) Makefile | $(BUILD_DIR)
	$(AR) -cr $@ $(OBJECTS)

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX)  $< -c -o $@ -Wall -shared $(CFLAGS)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@


$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)/$(TARGET_SO) $(BUILD_DIR)/$(TARGET_A) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.lst $(BUILD_DIR)/*.d

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
	
