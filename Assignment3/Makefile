CXX := g++
CXXFLAGS := -std=c++14 -Iinclude
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests
TARGET := $(BUILD_DIR)/shannon_coder
TEST_TARGET := $(BUILD_DIR)/shannon_tests

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
TEST_SOURCE := $(TEST_DIR)/tests.cpp
TEST_OBJECT := $(BUILD_DIR)/tests.o

.PHONY: all clean test

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_OBJECT): $(TEST_SOURCE)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(BUILD_DIR) $(TEST_TARGET)
	$(TEST_TARGET)

$(TEST_TARGET): $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS)) $(TEST_OBJECT)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lgtest_main -lpthread

clean:
	rm -rf $(BUILD_DIR)
