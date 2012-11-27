CXX = u++								# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD				# compiler flags

OBJECTS = soda.o config.o Printer.o NameServer.o Bank.o WATCardOffice.o Student.o
EXEC = soda
DEPENDS = ${OBJECTS:.o=.d}						# substitute ".o" with ".d"
EXECS = ${EXEC}						# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}								# build all executables

${EXEC} : ${OBJECTS}							# link step
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}						# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}							# include *.d files containing program dependences

clean :									# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
