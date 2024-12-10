TARGET := jsh

SRCS := main.c command.c file_loc.c history.c
OBJS := $(SRCS:.c=.o)

CC := gcc
CFLAGS := -Wall -Wextra 

all: $(TARGET)

debug: CFLAGS += -g
debug: all

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(TARGET)

re: fclean all