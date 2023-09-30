goal: mymovielist

main.o: main.c
	gcc -c main.c

movies.o: movies.c
	gcc -c movies.c

sorter.o: sorter.c
	gcc -c sorter.c

editor.o: editor.c
	gcc -c editor.c

mymovielist: main.o movies.o sorter.o editor.o
	gcc -Wall -Werror -std=c17 -o mymovielist main.o movies.o sorter.o editor.o

clean:
	rm -f mymovielist main.o movies.o sorter.o editor.o
