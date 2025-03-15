BUILD_DIR=build
BUILD_DIR_TEST=build_test
EXECUTABLE=sem1
EXECUTABLE_TEST=test

run: all
	./$(EXECUTABLE)

all: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make
	cp $(BUILD_DIR)/$(EXECUTABLE) .

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)