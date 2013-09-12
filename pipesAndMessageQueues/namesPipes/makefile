all: my_npipe_reader my_npipe_writer

my_npipe_reader: my_npipe_reader.c
	gcc -Wall -o my_npipe_reader my_npipe_reader.c

my_npipe_writer: my_npipe_writer.c
	gcc -Wall -o my_npipe_writer my_npipe_writer.c

clean:
	rm my_npipe_writer my_npipe_reader MYFIFO
