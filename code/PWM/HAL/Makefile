# This is the command line makefile for building a unit test.
#
# setup some information about the toolchain.
#
BIN_DIR = /usr/bin

# The actual toolchain
#
CC      = $(BIN_DIR)/clang

# Most of the time, these two datum are the ones to be chagned.
# INCLUDES is used to tell the compiler where all the header files
#          are.
# OBJS is adjusted to add the files to be built.

INCLUDES = -I. -I../MCAL -I../../include
OBJS = pwm_if.o pwm_if_test.o

# tells makedepend where to find the header files as make depend is not as 
# smart as the c compiler
#
DEPEND_INCLUDES = $(INCLUDES) 

OPTIMISATION = -O0
COVERAGE = --coverage
WARNINGS = -Wall -Wextra $(COVERAGE) -D__UNITTEST__
FLAGS	= $(WARNINGS) $(INCLUDES) $(OPTIMISATION) -c
LINKER_FLAGS = $(COVERAGE)
TARGET = unittest

SRCS = $(OBJS:.o=.c)

.PHONY: src_clean clean dist_clean version md5 depend size objdump 

# defined some implicit rules to deal with .c and .s files.
#
%.o: %.c
	$(CC) $(FLAGS) $(DEFINES) -o $@ $< 

%.o: %.s
	$(AS) $(AS_FLAGS) -o $@ $<

$(TARGET): $(OBJS) $(STARTUP_OBJS)
	$(CC) $(OBJS) $(STARTUP_OBJS) $(LINKER_FLAGS) --output=$@	

#
# the various targets of cleans. Provides different level of cleaning out the
# source code.
#
# src_clean     - removes editor backup files
# clean         - removes the object files
# dist_clean    - removes the binaries, object and intermediate files.
#
src_clean:
	find . -name "*.*~" | xargs rm -rf 

clean:
	find . -name "*.o" | xargs rm -rf

dist_clean : clean src_clean	
	find . -name "*.bin" | xargs rm -rf
	find . -name "*.gc*" | xargs rm -rf
	rm -rf $(RELEASE_DIR)

depend: $(SRCS) 
	rm -rf Makefile.depend
	touch Makefile.depend
	makedepend -o.o -f Makefile.depend $(DEFINES) $(DEPEND_INCLUDES) $^

help:
	@echo Supported targets:
	@echo  help                - this help message.
	@echo  unittest	           - build the unit test
	@echo                                                                     . 
	@echo  clean               - deletes .o
	@echo  dist_clean          - deletes .bin, .o, .elf, .log etc.
	@echo  src_clean           - deletes backup files .*~
	@echo  depend              - created a dependency file, make.depend
	@echo                                                                     . 
	@echo Examples:
	@echo    make unittest 		- build the unit test


include Makefile.depend


# DO NOT DELETE
