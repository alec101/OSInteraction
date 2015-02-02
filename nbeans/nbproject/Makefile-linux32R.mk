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
CND_PLATFORM=clang-Linux-x86
CND_DLIB_EXT=so
CND_CONF=linux32R
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/812168374/osiChar.o \
	${OBJECTDIR}/_ext/812168374/osiCocoa.o \
	${OBJECTDIR}/_ext/812168374/osiDisplay.o \
	${OBJECTDIR}/_ext/812168374/osiGlExt.o \
	${OBJECTDIR}/_ext/812168374/osiInput.o \
	${OBJECTDIR}/_ext/812168374/osiRenderer.o \
	${OBJECTDIR}/_ext/812168374/osiWindow.o \
	${OBJECTDIR}/_ext/812168374/osinteraction.o \
	${OBJECTDIR}/_ext/749505625/chainList.o \
	${OBJECTDIR}/_ext/749505625/errorHandling.o \
	${OBJECTDIR}/_ext/749505625/filePNG.o \
	${OBJECTDIR}/_ext/749505625/fileTGA.o \
	${OBJECTDIR}/_ext/749505625/mzPacker.o \
	${OBJECTDIR}/_ext/749505625/segList.o \
	${OBJECTDIR}/_ext/749505625/str32.o \
	${OBJECTDIR}/_ext/749505625/str8.o \
	${OBJECTDIR}/_ext/749505625/strCommon.o


# C Compiler Flags
CFLAGS=-m32

# CC Compiler Flags
CCFLAGS=-m32
CXXFLAGS=-m32

# Fortran Compiler Flags
FFLAGS=-m32

# Assembler Flags
ASFLAGS=--32

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../output/osi32linux.lib

../output/osi32linux.lib: ${OBJECTFILES}
	${MKDIR} -p ../output
	${RM} ../output/osi32linux.lib
	${AR} -rv ../output/osi32linux.lib ${OBJECTFILES} 
	$(RANLIB) ../output/osi32linux.lib

${OBJECTDIR}/_ext/812168374/osiChar.o: ../source/osiChar.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiChar.o ../source/osiChar.cpp

${OBJECTDIR}/_ext/812168374/osiCocoa.o: ../source/osiCocoa.mm 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiCocoa.o ../source/osiCocoa.mm

${OBJECTDIR}/_ext/812168374/osiDisplay.o: ../source/osiDisplay.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiDisplay.o ../source/osiDisplay.cpp

${OBJECTDIR}/_ext/812168374/osiGlExt.o: ../source/osiGlExt.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiGlExt.o ../source/osiGlExt.cpp

${OBJECTDIR}/_ext/812168374/osiInput.o: ../source/osiInput.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiInput.o ../source/osiInput.cpp

${OBJECTDIR}/_ext/812168374/osiRenderer.o: ../source/osiRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiRenderer.o ../source/osiRenderer.cpp

${OBJECTDIR}/_ext/812168374/osiWindow.o: ../source/osiWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osiWindow.o ../source/osiWindow.cpp

${OBJECTDIR}/_ext/812168374/osinteraction.o: ../source/osinteraction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/812168374/osinteraction.o ../source/osinteraction.cpp

${OBJECTDIR}/_ext/749505625/chainList.o: ../source/util/chainList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/chainList.o ../source/util/chainList.cpp

${OBJECTDIR}/_ext/749505625/errorHandling.o: ../source/util/errorHandling.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/errorHandling.o ../source/util/errorHandling.cpp

${OBJECTDIR}/_ext/749505625/filePNG.o: ../source/util/filePNG.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/filePNG.o ../source/util/filePNG.cpp

${OBJECTDIR}/_ext/749505625/fileTGA.o: ../source/util/fileTGA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/fileTGA.o ../source/util/fileTGA.cpp

${OBJECTDIR}/_ext/749505625/mzPacker.o: ../source/util/mzPacker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/mzPacker.o ../source/util/mzPacker.cpp

${OBJECTDIR}/_ext/749505625/segList.o: ../source/util/segList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/segList.o ../source/util/segList.cpp

${OBJECTDIR}/_ext/749505625/str32.o: ../source/util/str32.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/str32.o ../source/util/str32.cpp

${OBJECTDIR}/_ext/749505625/str8.o: ../source/util/str8.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/str8.o ../source/util/str8.cpp

${OBJECTDIR}/_ext/749505625/strCommon.o: ../source/util/strCommon.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/749505625
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I.. -I../source -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/749505625/strCommon.o ../source/util/strCommon.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../output/osi32linux.lib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
