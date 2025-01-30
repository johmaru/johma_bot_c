CC = gcc
CFLAGS = -g -Wall -Wextra -pthread -no-pie
LIBS = -ldiscord -lcurl
INCLUDES = -I. -I./libs -I./include
SRCDIR = .
OBJDIR = obj
LIBSDIR = libs
COMMANDDIR = commands

SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(LIBSDIR)/*.c) $(wildcard $(COMMANDDIR)/*.c)
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
TARGET = johma_bot

$(shell mkdir -p $(OBJDIR)/libs $(OBJDIR)/commands)

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(LIBS)

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

.PHONY: clean run mem b build

clean:
	rm -rf $(OBJDIR)/* $(TARGET)

build:
	$(CC) -c libs/filecontroller.c -o obj/libs/filecontroller.o $(CFLAGS) $(INCLUDES)
	$(CC) -c commands/watch_dogs.c -o obj/commands/watch_dogs.o $(CFLAGS) $(INCLUDES)
	$(CC) -c johma_bot.c -o obj/johma_bot.o $(CFLAGS) $(INCLUDES)
	$(CC) -c cJSON.c -o obj/cJSON.o $(CFLAGS) $(INCLUDES)
	$(CC) -c libs/writerthred.c -o obj/libs/writerthred.o $(CFLAGS) $(INCLUDES)
	$(CC) obj/libs/writerthred.o obj/libs/filecontroller.o obj/commands/watch_dogs.o obj/johma_bot.o obj/cJSON.o -o $(TARGET) $(CFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

re:
	$(MAKE) clean
	$(MAKE) build
	$(MAKE) run

rem:
	$(MAKE) clean
	$(MAKE) build
	$(MAKE) mem	

mem: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)