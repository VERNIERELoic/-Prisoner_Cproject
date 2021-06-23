#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc 
# define any compile-time flags #-DNDEBUG
CFLAGS	:= -Wall -Wextra -g -std=c11 -D_GNU_SOURCE 

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -pthread 

# define output directory
OUTPUT	:= src

# define source directory
SRC_SERVER		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib


MAIN := server
SOURCEDIRS	:= $(shell find $(SRC_SERVER) -type d)

INCLUDEDIRS	:= #$(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files 
OBJECTS		:= $(SOURCES:.c=.o)
#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))



$(OUTPUT):
	$(MD) $(OUTPUT)


all: $(OUTPUT) $(MAIN) $(SERVER_MAIN)
	@echo All compilation completed !


# Client compilation
$(MAIN): $(OUTPUT) $(OBJECTS)
	$(CC) $(CFLAGS) $(GTK1) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS) $(GTK2)



# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(GTK1) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean



clean:
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(SERVER_OBJECTS))
	@echo Cleanup complete!

run: all
	timeout 2 ./$(SERVER_OUTPUTMAIN)&
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!

documentation:
	@xdg-open html/index.html
