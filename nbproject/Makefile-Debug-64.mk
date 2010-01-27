#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug-64
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/stats.o \
	${OBJECTDIR}/board_test.o \
	${OBJECTDIR}/board.o \
	${OBJECTDIR}/search_test.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/eval_test.o \
	${OBJECTDIR}/benchmark.o \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/ttable.o \
	${OBJECTDIR}/search.o \
	${OBJECTDIR}/parser_test.o \
	${OBJECTDIR}/board2eps.o \
	${OBJECTDIR}/eval.o \
	${OBJECTDIR}/util.o

# C Compiler Flags
CFLAGS=-m64 -std=c99 -Wextra -pedantic-errors -DDEBUG

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug-64.mk dist/Debug-64/GNU-Linux-x86/fourinarow

dist/Debug-64/GNU-Linux-x86/fourinarow: ${OBJECTFILES}
	${MKDIR} -p dist/Debug-64/GNU-Linux-x86
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fourinarow ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/stats.o: nbproject/Makefile-${CND_CONF}.mk stats.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/stats.o stats.c

${OBJECTDIR}/board_test.o: nbproject/Makefile-${CND_CONF}.mk board_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board_test.o board_test.c

${OBJECTDIR}/board.o: nbproject/Makefile-${CND_CONF}.mk board.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board.o board.c

${OBJECTDIR}/search_test.o: nbproject/Makefile-${CND_CONF}.mk search_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/search_test.o search_test.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/eval_test.o: nbproject/Makefile-${CND_CONF}.mk eval_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/eval_test.o eval_test.c

${OBJECTDIR}/benchmark.o: nbproject/Makefile-${CND_CONF}.mk benchmark.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/benchmark.o benchmark.c

${OBJECTDIR}/parser.o: nbproject/Makefile-${CND_CONF}.mk parser.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/parser.o parser.c

${OBJECTDIR}/ttable.o: nbproject/Makefile-${CND_CONF}.mk ttable.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/ttable.o ttable.c

${OBJECTDIR}/search.o: nbproject/Makefile-${CND_CONF}.mk search.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/search.o search.c

${OBJECTDIR}/parser_test.o: nbproject/Makefile-${CND_CONF}.mk parser_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/parser_test.o parser_test.c

${OBJECTDIR}/board2eps.o: nbproject/Makefile-${CND_CONF}.mk board2eps.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board2eps.o board2eps.c

${OBJECTDIR}/eval.o: nbproject/Makefile-${CND_CONF}.mk eval.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/eval.o eval.c

${OBJECTDIR}/util.o: nbproject/Makefile-${CND_CONF}.mk util.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/util.o util.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug-64
	${RM} dist/Debug-64/GNU-Linux-x86/fourinarow

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
