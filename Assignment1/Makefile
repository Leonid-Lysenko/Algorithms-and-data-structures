CXX = g++
CXXFLAGS = -g -Wall -Werror -Wpedantic -pthread

SRCS = main.cpp encoder.cpp decoder.cpp test.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = ascii85_tool
TEST_TARGET = test_runner

all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o encoder.o decoder.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): test.o encoder.o decoder.o
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

