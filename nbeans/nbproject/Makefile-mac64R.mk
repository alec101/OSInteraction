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
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=mac64R
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1263054685/chainList.o \
	${OBJECTDIR}/_ext/1263054685/errorHandling.o \
	${OBJECTDIR}/_ext/1263054685/segList.o \
	${OBJECTDIR}/_ext/1263054685/stringClass32.o \
	${OBJECTDIR}/_ext/1263054685/stringClass8.o \
	${OBJECTDIR}/_ext/812168374/OSchar.o \
	${OBJECTDIR}/_ext/812168374/OScocoa.o \
	${OBJECTDIR}/_ext/812168374/OSdisplay.o \
	${OBJECTDIR}/_ext/812168374/OSinput.o \
	${OBJECTDIR}/_ext/812168374/OSinteraction.o \
	${OBJECTDIR}/_ext/812168374/pch.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=-m64

# Assembler Flags
ASFLAGS=-arch x86_64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../output/OSInteraction.mac

../output/OSInteraction.mac: ${OBJECTFILES}
	${MKDIR} -p ../output
	${LINK.cc} -o ../output/OSInteraction.mac ${OBJECTFILES} ${LDLIBSOPTIONS} -framework OpenGL -framework cocoa

${OBJECTDIR}/_ext/1263054685/chainList.o: ../../!utilClasses/chainList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263054685
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1263054685/chainList.o ../../!utilClasses/chainList.cpp

${OBJECTDIR}/_ext/1263054685/errorHandling.o: ../../!utilClasses/errorHandling.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263054685
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1263054685/errorHandling.o ../../!utilClasses/errorHandling.cpp

${OBJECTDIR}/_ext/1263054685/segList.o: ../../!utilClasses/segList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263054685
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1263054685/segList.o ../../!utilClasses/segList.cpp

${OBJECTDIR}/_ext/1263054685/stringClass32.o: ../../!utilClasses/stringClass32.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263054685
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1263054685/stringClass32.o ../../!utilClasses/stringClass32.cpp

${OBJECTDIR}/_ext/1263054685/stringClass8.o: ../../!utilClasses/stringClass8.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263054685
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1263054685/stringClass8.o ../../!utilClasses/stringClass8.cpp

${OBJECTDIR}/_ext/812168374/OSchar.o: ../source/OSchar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/OSchar.o ../source/OSchar.cpp

${OBJECTDIR}/_ext/812168374/OScocoa.o: ../source/OScocoa.mm 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/OScocoa.o ../source/OScocoa.mm

${OBJECTDIR}/_ext/812168374/OSdisplay.o: ../source/OSdisplay.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/OSdisplay.o ../source/OSdisplay.cpp

${OBJECTDIR}/_ext/812168374/OSinput.o: ../source/OSinput.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/OSinput.o ../source/OSinput.cpp

${OBJECTDIR}/_ext/812168374/OSinteraction.o: ../source/OSinteraction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/OSinteraction.o ../source/OSinteraction.cpp

${OBJECTDIR}/_ext/812168374/pch.o: ../source/pch.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DOS_MAC -I.. -I../source -I../../!utilClasses -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/pch.o ../source/pch.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../output/OSInteraction.mac

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
