CXX = g++

CXXFLAGS = -Wall -g

TARGET = my_program

SRCS = kde.cpp interface.cpp test-kde.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)


test-kde.o: test-kde.cpp 
	$(CXX) $(CXXFLAGS) -c test-kde.cpp

interface.o: interface.cpp 
	$(CXX) $(CXXFLAGS) -c interface.cpp

kde.o: kde.cpp 
	$(CXX) $(CXXFLAGS) -c kde.cpp

clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: all clean

