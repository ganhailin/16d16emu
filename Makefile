
CFLAGS = -Wall
TARGET = ./3DClock
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp %.h
	$(CXX) $(CFLAGS) -c $< -o $@
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)   -lSDL
clean:
	rm -rf *.o 3DClock