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
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/board_test.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/board.o

# C Compiler Flags
CFLAGS=-std=c99

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
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/fourinarow

dist/Debug/GNU-Linux-x86/fourinarow: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	gcc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fourinarow ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/parser.o: nbproject/Makefile-${CND_CONF}.mk parser.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/parser.o parser.c

${OBJECTDIR}/board_test.o: nbproject/Makefile-${CND_CONF}.mk board_test.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board_test.o board_test.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/board.o: nbproject/Makefile-${CND_CONF}.mk board.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Werror -MMD -MP -MF $@.d -o ${OBJECTDIR}/board.o board.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/fourinarow

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
