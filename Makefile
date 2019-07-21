CC:=clang-6.0

PROJDIR:=$(HOME)/Projects/SocketsIO
SRCDIR:=src
BUILDDIR:=build
TESTDIR:=test
NAME:=sockIO
TARGET:=bin/lib$(NAME).so

EXT:=cpp
SOURCES:=$(shell find $(SRCDIR) -type f -name *.$(EXT))
OBJECTS:=$(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(EXT)=.o))
TESTS:=$(shell find $(TESTDIR) -type f -name *.$(EXT))

CFLAGS:=-g -Wall -Wextra -std=c++17 #-DSOCKETIO_DEBUG -DBOOST_LOG_DYN_LINK
INC:=-I include
LIB:=-l$(NAME) -lgtest -lstdc++ -pthread #-lboost_log -lboost_log_setup

$(TARGET): $(OBJECTS)
	@echo "Linking ... "
	@echo "$(CC) -shared $(INC) $^ -o $(TARGET)"; $(CC) -shared $(INC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(EXT)	
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) -fPIC -D _DEFAULT_SOURCE -c $(INC) -o $@ $<"; $(CC) $(CFLAGS) -fPIC -D _DEFAULT_SOURCE -c $(INC) -o $@ $<

clean:
	@echo "Cleaning..."
	@echo "$(RM) -r $(BUILDDIR) bin/*"; $(RM) -r $(BUILDDIR) bin/*

test: $(TARGET) $(TESTS)
	$(CC) $(CFLAGS) -Lbin -Wl,-rpath=bin $(LIB) $(INC) $(TESTS) -o bin/tester
