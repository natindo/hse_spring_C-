BUILD_DIR=build
BUILD_DIR_TEST=build_test
EXECUTABLE=HW1
EXECUTABLE_TEST=test

run: all
	./$(EXECUTABLE)

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make
	cp $(BUILD_DIR)/$(EXECUTABLE) .

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#test: $(BUILD_DIR_TEST)/$(EXECUTABLE_TEST)
#
#$(BUILD_DIR_TEST)/$(EXECUTABLE_TEST): | $(BUILD_DIR_TEST)
#	cd $(BUILD_DIR_TEST)
#
#$(BUILD_DIR_TEST):
#	mkdir -p $(BUILD_DIR_TEST)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
