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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=lin32.dbg
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/511e4115/osiChar.o \
	${OBJECTDIR}/_ext/511e4115/osiCocoa.o \
	${OBJECTDIR}/_ext/511e4115/osiDisplay.o \
	${OBJECTDIR}/_ext/511e4115/osiGlExt.o \
	${OBJECTDIR}/_ext/511e4115/osiGlRenderer.o \
	${OBJECTDIR}/_ext/511e4115/osiInput.o \
	${OBJECTDIR}/_ext/511e4115/osiVulkan.o \
	${OBJECTDIR}/_ext/511e4115/osiWindow.o \
	${OBJECTDIR}/_ext/511e4115/osinteraction.o \
	${OBJECTDIR}/_ext/c34564bc/errorHandling.o \
	${OBJECTDIR}/_ext/c34564bc/fileOp.o \
	${OBJECTDIR}/_ext/c34564bc/filePNG.o \
	${OBJECTDIR}/_ext/c34564bc/fileTGA.o \
	${OBJECTDIR}/_ext/c34564bc/imgClass.o \
	${OBJECTDIR}/_ext/c34564bc/mzPacker.o \
	${OBJECTDIR}/_ext/c34564bc/segList.o \
	${OBJECTDIR}/_ext/c34564bc/str16.o \
	${OBJECTDIR}/_ext/c34564bc/str32.o \
	${OBJECTDIR}/_ext/c34564bc/str8.o \
	${OBJECTDIR}/_ext/c34564bc/strCommon.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../lib/osi.lin32.dbg.so

../lib/osi.lin32.dbg.so: ${OBJECTFILES}
	${MKDIR} -p ../lib
	${RM} ../lib/osi.lin32.dbg.so
	${AR} -rv ../lib/osi.lin32.dbg.so ${OBJECTFILES} 
	$(RANLIB) ../lib/osi.lin32.dbg.so

${OBJECTDIR}/_ext/511e4115/osiChar.o: ../src/osiChar.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiChar.o ../src/osiChar.cpp

${OBJECTDIR}/_ext/511e4115/osiCocoa.o: ../src/osiCocoa.mm
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiCocoa.o ../src/osiCocoa.mm

${OBJECTDIR}/_ext/511e4115/osiDisplay.o: ../src/osiDisplay.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiDisplay.o ../src/osiDisplay.cpp

${OBJECTDIR}/_ext/511e4115/osiGlExt.o: ../src/osiGlExt.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiGlExt.o ../src/osiGlExt.cpp

${OBJECTDIR}/_ext/511e4115/osiGlRenderer.o: ../src/osiGlRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiGlRenderer.o ../src/osiGlRenderer.cpp

${OBJECTDIR}/_ext/511e4115/osiInput.o: ../src/osiInput.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiInput.o ../src/osiInput.cpp

${OBJECTDIR}/_ext/511e4115/osiVulkan.o: ../src/osiVulkan.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiVulkan.o ../src/osiVulkan.cpp

${OBJECTDIR}/_ext/511e4115/osiWindow.o: ../src/osiWindow.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osiWindow.o ../src/osiWindow.cpp

${OBJECTDIR}/_ext/511e4115/osinteraction.o: ../src/osinteraction.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/osinteraction.o ../src/osinteraction.cpp

${OBJECTDIR}/_ext/c34564bc/errorHandling.o: ../src/util/errorHandling.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/errorHandling.o ../src/util/errorHandling.cpp

${OBJECTDIR}/_ext/c34564bc/fileOp.o: ../src/util/fileOp.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/fileOp.o ../src/util/fileOp.cpp

${OBJECTDIR}/_ext/c34564bc/filePNG.o: ../src/util/filePNG.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/filePNG.o ../src/util/filePNG.cpp

${OBJECTDIR}/_ext/c34564bc/fileTGA.o: ../src/util/fileTGA.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/fileTGA.o ../src/util/fileTGA.cpp

${OBJECTDIR}/_ext/c34564bc/imgClass.o: ../src/util/imgClass.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/imgClass.o ../src/util/imgClass.cpp

${OBJECTDIR}/_ext/c34564bc/mzPacker.o: ../src/util/mzPacker.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/mzPacker.o ../src/util/mzPacker.cpp

${OBJECTDIR}/_ext/c34564bc/segList.o: ../src/util/segList.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/segList.o ../src/util/segList.cpp

${OBJECTDIR}/_ext/c34564bc/str16.o: ../src/util/str16.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/str16.o ../src/util/str16.cpp

${OBJECTDIR}/_ext/c34564bc/str32.o: ../src/util/str32.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/str32.o ../src/util/str32.cpp

${OBJECTDIR}/_ext/c34564bc/str8.o: ../src/util/str8.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/str8.o ../src/util/str8.cpp

${OBJECTDIR}/_ext/c34564bc/strCommon.o: ../src/util/strCommon.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/c34564bc
	${RM} "$@.d"
	$(COMPILE.cc) -g -DOSI_USE_OPENGL -DOSI_USE_VULKAN -I../.. -I../include -I../../Vulkan-Headers/include/vulkan -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c34564bc/strCommon.o ../src/util/strCommon.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
