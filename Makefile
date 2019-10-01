CC:=g++

PROJDIR:=$(HOME)/Projects/SocketsIO
SRCDIR:=src
INCDIR:=include
BUILDDIR:=build
TESTDIR:=test
TARGETDIR:=bin
INSTALLDIR:=/usr/local
NAME:=sockio
TARGET:=$(TARGETDIR)/lib$(NAME).so

SRCEXT:=cpp
SOURCES:=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS:=$(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
TESTS:=$(shell find $(TESTDIR) -type f -name *.$(SRCEXT))

CFLAGS:=-g -Wall -Wextra -std=c++17 #-DSOCKETIO_DEBUG -DBOOST_LOG_DYN_LINK
INC:=-I include
LIB:=-l$(NAME) -lgtest -lstdc++ -pthread

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking ..."
	@echo "$(CC) -shared $(INC) $^ -o $(TARGET)"; $(CC) -shared $(INC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) -c -fPIC -D _DEFAULT_SOURCE $(INC) $< -o $@"; $(CC) $(CFLAGS) -c -fPIC -D _DEFAULT_SOURCE $(INC) $< -o $@

install: $(TARGET)
	@echo "Installing ..."
	@echo "cp $(TARGET) $(INSTALLDIR)/lib"; cp $(TARGET) $(INSTALLDIR)/lib
	@mkdir $(INSTALLDIR)/include/socket_io
	@echo "cp $(INCDIR)/* $(INSTALLDIR)/include/socket_io"; cp $(INCDIR)/* $(INSTALLDIR)/include/socket_io

clean:
	@echo "Cleaning ..."
	@echo "$(RM) -r $(BUILDDIR) $(TARGETDIR)"; $(RM) -r $(BUILDDIR) $(TARGETDIR)

uninstall:
	@echo "Uninstalling ..."
	@echo "$(RM) -r $(INSTALLDIR)/include/socket_io $(INSTALLDIR)/lib/lib$(NAME).so"; $(RM) -r $(INSTALLDIR)/include/socket_io $(INSTALLDIR)/lib/lib$(NAME).so

test: $(TARGET) $(TESTS)
	$(CC) $(CFLAGS) -Lbin -Wl,-rpath=bin $(LIB) $(INC) $(TESTS) -o bin/tester
