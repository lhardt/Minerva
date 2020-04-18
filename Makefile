# Commands
RM 		:= -rm
CC 		:= gcc
# Folders
SRCDIR 	:= src
OBJDIR 	:= obj
BINDIR 	:= bin
TSTDIR  := test
# Files
SRC 	:= $(wildcard $(SRCDIR)/*.c)
MAIN 	:= $(SRCDIR)/main.c
TARGET 	:= $(BINDIR)/main
OBJ 	:= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
# Flags
CFLAGS 	:= -Wall -g
LIB 	:= -lsqlite3
TSTFLAG :=
# Test
NMAIN	:= $(filter-out $(MAIN), $(SRC))
NMAINOB := $(NMAIN:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TSTSRC	:= $(wildcard $(TSTDIR)/*.c)
TSTOBJ	:= $(TSTSRC:$(TSTDIR)/%.c=$(OBJDIR)/%.o)
TSTGET  := $(BINDIR)/test

# print : $(OBJ)
# 	echo $(OBJ)
all: $(TARGET)

clean:
	$(RM) $(OBJ) $(TSTOBJ)

$(OBJ): $(SRC)
	$(CC)  $(CFLAGS) -c $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.c) -o $@

$(TARGET) : $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIB)

test: $(TSTGET)

$(TSTGET): $(TSTOBJ)
	$(CC) $(CFLAGS) $(TSTFLAG) -o $(TSTGET) $(TSTOBJ) $(NMAINOB) $(LIB)

$(TSTOBJ): $(TSTSRC)
	$(CC) $(CFLAGS) $(TSTFLAG) -I $(SRCDIR) -c $(@:$(OBJDIR)/%.o=$(TSTDIR)/%.c) -o $@

$(TSTSRC): $(TARGET)

theo:
	$(shell echo $(NMAINOB))
