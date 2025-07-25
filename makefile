CXX = g++
CXXFLAGS = -std=c++17 -g -O2

ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXEC_EXT = .exe
else
    RM = rm -f
    EXEC_EXT =
endif

INCLUDES = -I. -IUtils -IBicoreIndex -IIndex

TARGET = main$(EXEC_EXT)

SRCS = main.cpp \
    Graph.cpp \
    casestudy.cpp \
    dyn.rebuild.cpp \
    Utils/graph_utils.cpp \
    BicoreIndex/bicore_index.cpp \
    Index/VC_index.cpp \
    Index/AC_index.cpp \
    Index/MAC_index_build_maintenance.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o */*.o $(TARGET)
