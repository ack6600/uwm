all: build run
build:
	gcc -Wall -o out/uwm src/uwm.c -lxcb
run:
	out/uwm
vim:
	vim -p src/*.c src/*.h
