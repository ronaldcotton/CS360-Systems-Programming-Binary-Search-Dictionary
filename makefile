# CS360 - Assignment #2 - Lang
#
# Makefile
# CC = compiler; CFLAGS = compiler flags; OBJECTS = object files
# TARGET = output file (executable)

CC = gcc
CFLAGS = -g -Wall -std=c99
OPT = -O3
OBJECTS = main.o
HFILES =
TARGET = ok

# when make executes, creates a ./obj directory if obj directory is
# missing.  If the ./obj exists, the program moves all the *.o files to
# the parent directory.  At the end of linking and compiling, all object
# files are copied to the obj folder

.SUFFIXES: .o .h

all: prebuild main postbuild

prebuild:
	@echo "---\tPrebuild for $(TARGET) makefile."
	
postbuild:
	@echo "---\tPostbuild for $(TARGET) makefile."

main : $(OBJECTS)
	$(CC) $(CFLAGS) $(OPT) $(OBJECTS) $(HFILES) -o $(TARGET)

# makes .o files for each .c file
%.o : %.c $(HFILES)
	$(CC) $(CFLAGS) $(OPT) -c -o $@ $<

# clean removes the executable and the .obj folder.
# .PHONY prevents clean from not running if the file clean exists
# (consider clean to be a reserved filename)	
.PHONY : clean
clean :
	@echo 
	@$(RM) $(TARGET)
	@$(RM) $(OBJECTS)
