ROOT_DIR = ../../..
include $(ROOT_DIR)/build/arm/config.mk

VERSION = 1.0
TARGET = librexjson++
TARGET_SO = $(TARGET).so.$(VERSION)
TARGET_LIB = $(TARGET).a

CTARGET = librexjson
CTARGET_SO = $(CTARGET).so.$(VERSION)
CTARGET_LIB = $(CTARGET).a


CFLAGS += -I../../..

OBJECTS += $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard ../../*.cpp))))
vpath %.cpp ../..

COBJECTS += $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(notdir $(wildcard ../../*.c))))
vpath %.c ../..


# default action: build all
all: $(BIN_DIR)/$(TARGET_LIB) $(BIN_DIR)/$(CTARGET_LIB)

$(OBJ_DIR)/$(TARGET_SO): $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDFLAGS) -shared -Wl,-soname,$(TARGET).so -o $@
	ln -f -r -s $(OBJ_DIR)/$(TARGET_SO) $(OBJ_DIR)/$(TARGET).so

$(BIN_DIR)/$(TARGET_LIB): $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR)
	$(AR) -cr $@ $^

$(BIN_DIR)/$(CTARGET_LIB): $(COBJECTS) | $(OBJ_DIR) $(BIN_DIR)
	$(AR) -cr $@ $^

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: %.c Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(OBJ_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	@rm -f $(BIN_DIR)/$(TARGET_SO) $(BIN_DIR)/$(CTARGET_SO) 
	@rm -f $(BIN_DIR)/$(TARGET_LIB) $(BIN_DIR)/$(CTARGET_LIB) 
	@rm -f -R $(OBJ_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BIN_DIR)/*.d)





