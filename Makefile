# Commands
RM 		:= -rm
CC 		:= gcc
CXX		:= g++
WXHOME  := # /C/wxWidgets-3.1.3/build-release/
WXCONF  := $(WXHOME)wx-config
WNDRES  := windres
# Folders
SRCDIR 	:= src
OBJDIR 	:= obj
BINDIR 	:= bin
TSTDIR  := test
INCDIR  := inc
LIBDIR  := lib
# Files
CSRC 	:= $(wildcard $(SRCDIR)/*.c)
CPPSRC  := $(wildcard $(SRCDIR)/*.cpp)
MAIN 	:= $(SRCDIR)/main.c
MAINO 	:= $(OBJDIR)/main.o
TARGET 	:= $(BINDIR)/main
LIBO	:= $(wildcard $(LIBDIR)/*.o)
COBJ 	:= $(CSRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CPPOBJ  := $(CPPSRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.O)
# Flags
CFLAGS 	:= -Iinc -Wall -g $(shell $(WXCONF) --cflags)
CXXFLAGS:= -Iinc -Wall -g $(shell $(WXCONF) --cxxflags)
LIB 	:= -lsqlite3 $(shell $(WXCONF) --libs ) -lwx_gtk3u_ribbon-3.0
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

obj/%.o: src/%.c
	$(CC)  $(CFLAGS) -c $(@:$(OBJDIR)/%.o=$(SRCDIR)/%.c) -o $@

obj/%.O : src/%.cpp
	$(CXX)  $(CXXFLAGS) -c $(@:$(OBJDIR)/%.O=$(SRCDIR)/%.cpp) -o $@

$(TARGET) : $(COBJ) $(CPPOBJ)
	$(CXX) -o $(TARGET) $(COBJ) $(CPPOBJ) $(LIB) $(LIBO)

test: $(TSTGET)

$(TSTGET): $(COBJ) $(CPPOBJ) $(TOBJC) $(TOBJCPP)
	$(CXX) $(CXXFLAGS) $(TSTFLAG) -o $(TSTGET) $(NMAINOBJ) $(TOBJC) $(TOBJCPP) $(LIB) -I $(SRCDIR)

$(TOBJC): $(TSRCC)
	$(CC) $(CFLAGS) $(TSTFLAG) -I $(SRCDIR) -c $(@:$(OBJDIR)/%.o=$(TSTDIR)/%.c) -o $@

$(TOBJCPP): $(TSRCCPP)
	$(CXX) $(CXXFLAGS) $(TSTFLAG) -I $(SRCDIR) -c $(@:$(OBJDIR)/%.o=$(TSTDIR)/%.cpp) -o $@

$(TSTSRC): $(TARGET)
