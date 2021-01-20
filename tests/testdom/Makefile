
ROOT_DIR = ../..
OUTDIR = bin
SRCDIR = .
INCLUDE += -I$(SRCDIR)
INCLUDE += -I$(ROOT_DIR) 
TARGET = testdom
LIBS += -lrexjson -static
LIBDIRS = -L$(ROOT_DIR)/rexjson/build/linux/bin

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

OBJECTS += $(addprefix $(OUTDIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard ./*.cpp))))
vpath %.cpp ./

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$*.o -c $(SRCDIR)/$*.c

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp | $(OUTDIR)
	$(CPP) $(CFLAGS) -o $(OUTDIR)/$*.o -c $(SRCDIR)/$*.cpp

$(OUTDIR)/$(TARGET): $(OBJECTS) 
	$(CPP) -o $@ $^ $(LIBS) $(LIBDIRS)

$(OUTDIR):
	@mkdir $@

clean:
	-rm -fR $(OUTDIR)/$(TARGET) $(OUTDIR)/*.o $(OUTDIR)/*.lst $(OUTDIR)/*.d
