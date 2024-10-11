build:
	gcc -o tema3 main.c functii.c graph.h
run: build
	./tema3 1
clean:
	rm tema3
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema3 1