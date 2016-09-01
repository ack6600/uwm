.PHONY: vim clean
CFLAGS= -g -Wall -lxcb -lxcb-keysyms
TARGET=out/uwm
OBJS=uwm.o
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $<
vim:
	vim -p src/*.c src/*.h
clean:
	rm $(TARGET) $(OBJS)
