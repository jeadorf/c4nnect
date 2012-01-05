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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug-64-AB
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/benchmark.o \
	${OBJECTDIR}/search_test.o \
	${OBJECTDIR}/board.o \
	${OBJECTDIR}/stats_test.o \
	${OBJECTDIR}/ttable.o \
	${OBJECTDIR}/parser_test.o \
	${OBJECTDIR}/board_test.o \
	${OBJECTDIR}/util.o \
	${OBJECTDIR}/board2eps.o \
	${OBJECTDIR}/ttable_test.o \
	${OBJECTDIR}/stats.o \
	${OBJECTDIR}/eval_test.o \
	${OBJECTDIR}/eval.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/search.o \
	${OBJECTDIR}/parser.o


# C Compiler Flags
CFLAGS=-m64 -std=gnu99 -Wextra -pedantic-errors -DDISABLE_ABCUTS -DDEBUG

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c4nnect

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c4nnect: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c4nnect ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/benchmark.o: benchmark.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/benchmark.o benchmark.c

${OBJECTDIR}/search_test.o: search_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/search_test.o search_test.c

${OBJECTDIR}/board.o: board.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board.o board.c

${OBJECTDIR}/stats_test.o: stats_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/stats_test.o stats_test.c

${OBJECTDIR}/ttable.o: ttable.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/ttable.o ttable.c

${OBJECTDIR}/parser_test.o: parser_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/parser_test.o parser_test.c

${OBJECTDIR}/board_test.o: board_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board_test.o board_test.c

${OBJECTDIR}/util.o: util.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/util.o util.c

${OBJECTDIR}/board2eps.o: board2eps.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board2eps.o board2eps.c

${OBJECTDIR}/ttable_test.o: ttable_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/ttable_test.o ttable_test.c

${OBJECTDIR}/stats.o: stats.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/stats.o stats.c

${OBJECTDIR}/eval_test.o: eval_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/eval_test.o eval_test.c

${OBJECTDIR}/eval.o: eval.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/eval.o eval.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/search.o: search.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/search.o search.c

${OBJECTDIR}/parser.o: parser.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/parser.o parser.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/c4nnect

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
