HEADERS = Headers/
COMPILER = g++
SOURCE = main.cpp
TARGET = Riceify
FLAGS = -Wall -Wextra -I $(HEADERS)
$(TARGET): $(SOURCE)
compile:
	$(COMPILER) -o $(TARGET) $(SOURCE) -I $(HEADERS) $(FLAGS)
install: $(TARGET)
	sudo cp $(TARGET) /usr/bin/$(TARGET)
run: $(TARGET)
	./$(TARGET)
debug: $(TARGET)
	gdb $(TARGET)
clean:
	rm $(TARGET)
uninstall:
	sudo rm /usr/bin/$(TARGET)
repair:
	make clean
	make
