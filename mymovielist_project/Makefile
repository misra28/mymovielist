goal: mymovielist

main.o: main.c
	gcc -g -c main.c

movies.o: movies.c
	gcc -g -c movies.c

sorter.o: sorter.c
	gcc -g -c sorter.c

editor.o: editor.c
	gcc -g -c editor.c

mymovielist: main.o movies.o sorter.o editor.o
	gcc -Wall -Werror -std=c17 -g -o mymovielist main.o movies.o sorter.o editor.o

clean:
	rm -f mymovielist main.o movies.o sorter.o editor.o
