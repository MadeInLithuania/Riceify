# ...existing code...
HEADERS = Headers/
CXX = clang++
SRCS = $(wildcard *.cpp)
TARGET = Riceify
CXXFLAGS = -Wall -Wextra -I$(HEADERS) -std=gnu++20
LDFLAGS =
INSTALL_DIR = /usr/bin

.PHONY: all compile install run debug clean uninstall repair

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

compile: $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) $(INSTALL_DIR)/$(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb $(TARGET)

clean:
	-rm -f $(TARGET)

uninstall:
	sudo rm -f $(INSTALL_DIR)/$(TARGET)

repair: 
	@$(MAKE) clean
	@$(MAKE)