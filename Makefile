CXX = g++
WINDRES = windres

TARGET = main.exe

all: $(TARGET)

main.o: main.cpp
	@$(CXX) -c main.cpp -o main.o

chess.o: chess.cpp
	@$(CXX) -c chess.cpp -o chess.o

draw.o: draw.cpp
	@$(CXX) -c draw.cpp -o draw.o

resources.o: resources.rc
	@$(WINDRES) resources.rc -O coff -o resources.o

$(TARGET): main.o chess.o draw.o resources.o
	@$(CXX) main.o chess.o draw.o resources.o -o $(TARGET) -lgdi32 -lgdiplus -lole32

clean:
	@del /Q $(TARGET) main.o chess.o draw.o resources.o