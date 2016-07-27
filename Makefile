.PHONY: vim clean
CFLAGS= -g -Wall -lxcb
TARGET=out/uwm
OBJS=uwm.o
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: src/%.c
	$(CC) $(CFLAGS) -c $<
vim:
	vim -p src/*.c src/*.h
clean:
	rm $(TARGET) $(OBJS)
