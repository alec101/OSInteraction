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
CCC=clang++-3.5
CXX=clang++-3.5
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=linux64R
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1360937237/osiChar.o \
	${OBJECTDIR}/_ext/1360937237/osiCocoa.o \
	${OBJECTDIR}/_ext/1360937237/osiDisplay.o \
	${OBJECTDIR}/_ext/1360937237/osiGlExt.o \
	${OBJECTDIR}/_ext/1360937237/osiInput.o \
	${OBJECTDIR}/_ext/1360937237/osiRenderer.o \
	${OBJECTDIR}/_ext/1360937237/osiWindow.o \
	${OBJECTDIR}/_ext/1360937237/osinteraction.o \
	${OBJECTDIR}/_ext/1018862404/chainList.o \
	${OBJECTDIR}/_ext/1018862404/errorHandling.o \
	${OBJECTDIR}/_ext/1018862404/filePNG.o \
	${OBJECTDIR}/_ext/1018862404/fileTGA.o \
	${OBJECTDIR}/_ext/1018862404/mzPacker.o \
	${OBJECTDIR}/_ext/1018862404/segList.o \
	${OBJECTDIR}/_ext/1018862404/str32.o \
	${OBJECTDIR}/_ext/1018862404/str8.o \
	${OBJECTDIR}/_ext/1018862404/strCommon.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=-m64

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../lib/osi64lin.so

../lib/osi64lin.so: ${OBJECTFILES}
	${MKDIR} -p ../lib
	${RM} ../lib/osi64lin.so
	${AR} -rv ../lib/osi64lin.so ${OBJECTFILES} 
	$(RANLIB) ../lib/osi64lin.so

${OBJECTDIR}/_ext/1360937237/osiChar.o: ../src/osiChar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiChar.o ../src/osiChar.cpp

${OBJECTDIR}/_ext/1360937237/osiCocoa.o: ../src/osiCocoa.mm 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiCocoa.o ../src/osiCocoa.mm

${OBJECTDIR}/_ext/1360937237/osiDisplay.o: ../src/osiDisplay.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiDisplay.o ../src/osiDisplay.cpp

${OBJECTDIR}/_ext/1360937237/osiGlExt.o: ../src/osiGlExt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiGlExt.o ../src/osiGlExt.cpp

${OBJECTDIR}/_ext/1360937237/osiInput.o: ../src/osiInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiInput.o ../src/osiInput.cpp

${OBJECTDIR}/_ext/1360937237/osiRenderer.o: ../src/osiRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiRenderer.o ../src/osiRenderer.cpp

${OBJECTDIR}/_ext/1360937237/osiWindow.o: ../src/osiWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osiWindow.o ../src/osiWindow.cpp

${OBJECTDIR}/_ext/1360937237/osinteraction.o: ../src/osinteraction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360937237
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1360937237/osinteraction.o ../src/osinteraction.cpp

${OBJECTDIR}/_ext/1018862404/chainList.o: ../src/util/chainList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/chainList.o ../src/util/chainList.cpp

${OBJECTDIR}/_ext/1018862404/errorHandling.o: ../src/util/errorHandling.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/errorHandling.o ../src/util/errorHandling.cpp

${OBJECTDIR}/_ext/1018862404/filePNG.o: ../src/util/filePNG.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/filePNG.o ../src/util/filePNG.cpp

${OBJECTDIR}/_ext/1018862404/fileTGA.o: ../src/util/fileTGA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/fileTGA.o ../src/util/fileTGA.cpp

${OBJECTDIR}/_ext/1018862404/mzPacker.o: ../src/util/mzPacker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/mzPacker.o ../src/util/mzPacker.cpp

${OBJECTDIR}/_ext/1018862404/segList.o: ../src/util/segList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/segList.o ../src/util/segList.cpp

${OBJECTDIR}/_ext/1018862404/str32.o: ../src/util/str32.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/str32.o ../src/util/str32.cpp

${OBJECTDIR}/_ext/1018862404/str8.o: ../src/util/str8.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/str8.o ../src/util/str8.cpp

${OBJECTDIR}/_ext/1018862404/strCommon.o: ../src/util/strCommon.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1018862404
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1018862404/strCommon.o ../src/util/strCommon.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../lib/osi64lin.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
