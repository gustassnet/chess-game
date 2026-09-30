
CXX = g++
WINDRES = windres

TARGET = main.exe

all: $(TARGET)

resources.o: resources.rc
	@$(WINDRES) resources.rc -O coff -o resources.o

$(TARGET): main.cpp resources.o
	@$(CXX) main.cpp resources.o -o $(TARGET) -lgdi32 -lgdiplus -lole32

clean:
	@del /Q $(TARGET) resources.o