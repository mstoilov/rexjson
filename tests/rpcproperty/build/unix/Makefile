ROOT_DIR = ../../../..
include $(ROOT_DIR)/build/unix/config.mk

TARGET = rpcprop
CFLAGS += -I$(ROOT_DIR)
LIBS += -lrexjson++


all: $(BIN_DIR)/$(TARGET)

#######################################
# dependencies
#######################################
-include $(wildcard $(OUTDIR)/*.d)

SRC_DIR = ../..
OBJECTS += $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(notdir $(wildcard $(SRC_DIR)/*.cpp))))
vpath %.cpp $(SRC_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/$*.o -c $(SRC_DIR)/$*.c

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CFLAGS) -o $(OBJ_DIR)/$*.o -c $(SRC_DIR)/$*.cpp

$(BIN_DIR)/$(TARGET): $(OBJECTS) 
	$(CXX) -o $@ $^ $(LIBS) $(LIBDIRS)

$(BIN_DIR):
	@mkdir $@

$(OBJ_DIR):
	@mkdir $@

clean:
	-rm -fr $(BIN_DIR)/$(TARGET) $(OBJ_DIR)/*.o $(OBJ_DIR)/*.lst $(OBJ_DIR)/*.d
	-rm -fr $(OBJ_DIR)
		
