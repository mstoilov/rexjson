
OUTDIR = bin
ROOT_DIR = ../../../..
SRC_DIR = ../..
INCLUDE += -I$(SRC_DIR)
INCLUDE += -I$(ROOT_DIR) 
TARGET = value
LIBS += -lrexjson++
LIBDIRS = -L$(ROOT_DIR)/rexjson/build/unix/lib

CPP = g++ -std=c++14
CC = gcc
LD = gcc
AR = ar

ifeq ($(BLDCFG), release)
CFLAGS = -O3 $(INCLUDE) -DUNIX
else
CFLAGS = -g -Wall $(INCLUDE) -DUNIX -DDEBUG
endif


all: $(OUTDIR)/$(TARGET)

#######################################
# dependencies
#######################################
-include $(wildcard $(OUTDIR)/*.d)

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

OBJECTS += $(addprefix $(OUTDIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard $(SRC_DIR)/*.cpp))))
vpath %.cpp $(SRC_DIR)/

$(OUTDIR)/%.o: $(SRC_DIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$*.o -c $(SRC_DIR)/$*.c

$(OUTDIR)/%.o: $(SRC_DIR)/%.cpp | $(OUTDIR)
	$(CPP) $(CFLAGS) -o $(OUTDIR)/$*.o -c $(SRC_DIR)/$*.cpp

$(OUTDIR)/$(TARGET): $(OBJECTS) 
	$(CPP) -o $@ $^ $(LIBS) $(LIBDIRS)

$(OUTDIR):
	@mkdir $@

clean:
	-rm -fr $(OUTDIR)/$(TARGET) $(OUTDIR)/*.o $(OUTDIR)/*.lst $(OUTDIR)/*.d
	-rm -fr $(OUTDIR)
		
