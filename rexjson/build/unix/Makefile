DEBUG ?= yes
PLATFORM ?= unix
BIN_DIR ?= lib
PROJECT_BIN_DIR ?= build/$(PLATFORM)/bin
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-implicit-fallthrough
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS = -std=gnu++14
LDFLAGS = 

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



VERSION = 1.0
TARGET = librexjson++
TARGET_SO = $(TARGET).so.$(VERSION)
TARGET_LIB = $(TARGET).a

CTARGET = librexjson
CTARGET_SO = $(CTARGET).so.$(VERSION)
CTARGET_LIB = $(CTARGET).a


CFLAGS += -I../../..

OBJECTS += $(addprefix $(BIN_DIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard ../../*.cpp))))
vpath %.cpp ../..

COBJECTS += $(addprefix $(BIN_DIR)/, $(patsubst %.c,%.o,$(notdir $(wildcard ../../*.c))))
vpath %.c ../..


# default action: build all
all: $(BIN_DIR)/$(TARGET_LIB) $(BIN_DIR)/$(CTARGET_LIB)

$(BIN_DIR)/$(TARGET_SO): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) -shared -Wl,-soname,$(TARGET).so -o $@
	ln -f -r -s $(BIN_DIR)/$(TARGET_SO) $(BIN_DIR)/$(TARGET).so

$(BIN_DIR)/$(TARGET_LIB): $(OBJECTS) | $(BIN_DIR)
	$(AR) -cr $@ $^

$(BIN_DIR)/$(CTARGET_LIB): $(COBJECTS) | $(BIN_DIR)
	$(AR) -cr $@ $^

$(BIN_DIR)/%.o: %.cpp Makefile | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BIN_DIR)/%.o: %.c Makefile | $(BIN_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(BIN_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	@rm -f $(BIN_DIR)/$(TARGET_SO) $(BIN_DIR)/$(TARGET).so $(BIN_DIR)/$(TARGET_LIB) $(BIN_DIR)/*.o $(BIN_DIR)/*.d $(BIN_DIR)/*.lst
	@rm -f -R $(BIN_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BIN_DIR)/*.d)





