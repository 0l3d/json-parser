CC = cc

filenames := arrays checks compare integer_parsing main memory parser strings

src_fpaths = $(addprefix src/, $(filenames))
SRCS = $(addsuffix .c, $(src_fpaths))
CFLAGS = -Wall -Wextra -Wconversion -Wpedantic -std=c89 -Wshadow -Wswitch-enum -g

all: base
base:
	$(CC) $(SRCS) $(CFLAGS) -o parser -Werror

base-e:
	$(CC) $(SRCS) $(CFLAGS) -o parser
