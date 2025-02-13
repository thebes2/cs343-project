CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wextra -MMD -DSTRSTREAM # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = config.o soda.o printer.o parent.o bank.o groupoff.o \
			watCard.o bottlingPlant.o truck.o vendingMachine.o nameServer.o watCardOffice.o student.o # list of object files
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXEC}					# build all executables
.PHONY : ${EXEC}

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJECTS}

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d ${OBJECTS} ${EXEC}
