# Commands
RM 		:= -rm
CC 		:= gcc
CXX		:= g++
# Folders
SRCDIR 	:= src
OBJDIR 	:= obj
BINDIR 	:= bin
TSTDIR  := test
# Files
CSRC 	:= $(wildcard $(SRCDIR)/*.c)
CPPSRC  := $(wildcard $(SRCDIR)/*.cpp)
MAIN 	:= $(SRCDIR)/main.c
MAINO 	:= $(OBJDIR)/main.o
TARGET 	:= $(BINDIR)/main
COBJ 	:= $(CSRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CPPOBJ  := $(CPPSRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
# Flags
CFLAGS 	:= -Wall -g $(shell wx-config --cflags)
CXXFLAGS:= -Wall -g $(shell wx-config --cxxflags)
LIB 	:= -lsqlite3 $(shell wx-config --libs) -lwx_gtk3u_ribbon-3.0
TSTFLAG :=
# Test
NMAINOBJ:= $(filter-out $(MAINO), $(COBJ)) $(filter-out $(MAINO), $(CPPOBJ))

TSRCC	:= $(wildcard $(TSTDIR)/*.c)
TOBJC	:= $(TSRCC:$(TSTDIR)/%.c=$(OBJDIR)/%.o)

TSRCCPP := $(wildcard $(TSTDIR)/*.cpp)
TOBJCPP := $(TSRCCPP:$(TSTDIR)/%.cpp=$(OBJDIR)/%.o)
TSTGET  := $(BINDIR)/test

all: $(TARGET)

clean:
	$(RM) $(COBJ) $(TOBJC) $(CPPOBJ) $(TOBJCPP)

$(COBJ): $(CSRC)
	$(CC)  $(CFLAGS) -c $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.c) -o $@

$(CPPOBJ) : $(CPPSRC)
	$(CXX)  $(CXXFLAGS) -c $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.cpp) -o $@

$(TARGET) : $(COBJ) $(CPPOBJ)
	$(CXX) -o $(TARGET) $(COBJ) $(CPPOBJ) $(LIB)

test: $(TSTGET)

$(TSTGET): $(COBJ) $(CPPOBJ) $(TOBJC) $(TOBJCPP)
	$(CXX) $(CXXFLAGS) $(TSTFLAG) -o $(TSTGET) $(NMAINOBJ) $(TOBJC) $(TOBJCPP) $(LIB) -I $(SRCDIR)

$(TOBJC): $(TSRCC)
	$(CC) $(CFLAGS) $(TSTFLAG) -I $(SRCDIR) -c $(@:$(OBJDIR)/%.o=$(TSTDIR)/%.c) -o $@

$(TOBJCPP): $(TSRCCPP)
	$(CXX) $(CXXFLAGS) $(TSTFLAG) -I $(SRCDIR) -c $(@:$(OBJDIR)/%.o=$(TSTDIR)/%.cpp) -o $@

$(TSTSRC): $(TARGET)
