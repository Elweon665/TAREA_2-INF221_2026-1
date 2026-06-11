CXX = g++
CXXFLAGS = -std=c++17
TARGET = general
PRIN_DIR = .
DIR = algorithms
SOURCES = $(PRIN_DIR)/general.cpp \
		  $(DIR)/brute-force.cpp \
		  $(DIR)/dynamic-programming.cpp \
		  $(DIR)/greedy1.cpp \
		  $(DIR)/greedy2.cpp
HEADER = $(DIR)/general.h

OBJS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)
run: $(TARGET)
	./$(TARGET)