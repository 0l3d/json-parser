CC = cc

filenames := arrays checks compare integer_parsing main memory parser strings

src_fpaths = $(addprefix src/, $(filenames))
SRCS = $(addsuffix .c, $(src_fpaths))
CFLAGS = -Wall -Wextra -Wconversion -Wpedantic -std=c89 -Wshadow -Wswitch-enum -g
TIME = $(shell date +%H:%M:%S)

all: base
base:
	$(CC) $(SRCS) $(CFLAGS) -o parser -Werror

base-e:
	$(CC) $(SRCS) $(CFLAGS) -o parser

mem:
	valgrind --leak-check=full --log-file=$(TIME)-file.log ./parser test.json

debug:
	valgrind --leak-check=full ./parser test.json >> out.txt
