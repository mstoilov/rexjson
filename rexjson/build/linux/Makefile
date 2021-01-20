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
OUTDIR = bin
ROOT_DIR = ../../..
SRC_DIR = ../..

INCLUDES += #$(shell python3 -m pybind11 --includes)
INCLUDES += -I$(ROOT_DIR)
INCLUDES += $(shell python3-config --includes)
INCLUDES += -I../extern/pybind11/include
C_DEFS =
CFLAGS = $(C_DEFS) $(INCLUDES) -Wall -fPIC --shared -std=gnu++14
LDFLAGS = -shared -fPIC


ifeq ($(DEBUG), 1)
CFLAGS += -g -O0
else
CFLAGS += $(OPT)

endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


OBJECTS = $(addprefix $(OUTDIR)/, $(patsubst %.c,%.o,$(notdir $(wildcard $(SRC_DIR)/*.c))))
vpath %.c $(SRC_DIR)

OBJECTS += $(addprefix $(OUTDIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard $(SRC_DIR)/*.cpp))))
vpath %.cpp $(SRC_DIR)

# default action: build all
all: $(OUTDIR)/$(TARGET_SO) $(OUTDIR)/$(TARGET_LIB)

$(OUTDIR)/$(TARGET_SO): $(OBJECTS) Makefile | $(OUTDIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(OUTDIR)/$(TARGET_LIB): $(OBJECTS) Makefile | $(OUTDIR)
	$(AR) -cr $@ $(OBJECTS)

$(OUTDIR)/%.o: %.cpp Makefile | $(OUTDIR)
	$(CXX)  $< -c -o $@ -Wall -shared $(CFLAGS)

$(OUTDIR)/%.o: %.c Makefile | $(OUTDIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(OUTDIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(OUTDIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(OUTDIR)/$(TARGET_SO) $(OUTDIR)/$(TARGET_LIB) $(OUTDIR)/*.o $(OUTDIR)/*.lst $(OUTDIR)/*.d $(OUTDIR)
	-rm -d $(OUTDIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(OUTDIR)/*.d)
	
