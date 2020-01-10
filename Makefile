RM := rm
CC := gcc
SRCDIR := src
OBJDIR := obj
BINDIR := bin
SRC := $(wildcard src/*.c)
OBJ :=
CFLAGS := -Wall
TARGET := build/main
CFLAGS := -g
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


# print : $(OBJ)
# 	echo $(OBJ)
all: $(TARGET)

clean:
	$(RM) $(OBJ)

$(OBJ): $(SRC)
	$(CC)  $(CFLAGS) -c $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.c) -o $@

$(TARGET) : $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
#
#
#
# obj/%.o: $(SRC)
# 	$(CC) -c $(CFLAGS) -o $@ $<
