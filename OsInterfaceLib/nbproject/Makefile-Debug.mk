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
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/CBaseBlock.o \
	${OBJECTDIR}/src/CControlLogger.o \
	${OBJECTDIR}/src/CDefaultDevice.o \
	${OBJECTDIR}/src/CDefaultHardware.o \
	${OBJECTDIR}/src/CDefaultLogger.o \
	${OBJECTDIR}/src/CFileDownloader.o \
	${OBJECTDIR}/src/CGenericDevice.o \
	${OBJECTDIR}/src/CGenericHardware.o \
	${OBJECTDIR}/src/CGenericMessage.o \
	${OBJECTDIR}/src/CGenericMessageManager.o \
	${OBJECTDIR}/src/CGenericSoftware.o \
	${OBJECTDIR}/src/CGenericTestPort.o \
	${OBJECTDIR}/src/CGenericTime.o \
	${OBJECTDIR}/src/CLinuxCommPort.o \
	${OBJECTDIR}/src/CLinuxCurrentTime.o \
	${OBJECTDIR}/src/CLinuxFile.o \
	${OBJECTDIR}/src/CLinuxHardware.o \
	${OBJECTDIR}/src/CLinuxPort.o \
	${OBJECTDIR}/src/CLinuxSocketClient.o \
	${OBJECTDIR}/src/CLinuxSocketServer.o \
	${OBJECTDIR}/src/CLinuxSoftware.o \
	${OBJECTDIR}/src/CLinuxSpiPort.o \
	${OBJECTDIR}/src/CLinuxZeroTime.o \
	${OBJECTDIR}/src/CMapToPointer.o \
	${OBJECTDIR}/src/CMessageManager.o \
	${OBJECTDIR}/src/CProtocol.o \
	${OBJECTDIR}/src/CRawData.o \
	${OBJECTDIR}/src/CVxWorkCommPort.o \
	${OBJECTDIR}/src/CVxWorkHardware.o \
	${OBJECTDIR}/src/CVxWorkSoftware.o \
	${OBJECTDIR}/src/CVxWorkSpiPort.o \
	${OBJECTDIR}/src/CVxWorkThread.o \
	${OBJECTDIR}/src/CWindowsCommPort.o \
	${OBJECTDIR}/src/CWindowsCurrentTime.o \
	${OBJECTDIR}/src/CWindowsFile.o \
	${OBJECTDIR}/src/CWindowsHardware.o \
	${OBJECTDIR}/src/CWindowsPort.o \
	${OBJECTDIR}/src/CWindowsSocketClient.o \
	${OBJECTDIR}/src/CWindowsSocketServer.o \
	${OBJECTDIR}/src/CWindowsSoftware.o \
	${OBJECTDIR}/src/CWindowsSpiPort.o \
	${OBJECTDIR}/src/CWindowsZeroTime.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a

${OBJECTDIR}/src/CBaseBlock.o: src/CBaseBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CBaseBlock.o src/CBaseBlock.cpp

${OBJECTDIR}/src/CControlLogger.o: src/CControlLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CControlLogger.o src/CControlLogger.cpp

${OBJECTDIR}/src/CDefaultDevice.o: src/CDefaultDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultDevice.o src/CDefaultDevice.cpp

${OBJECTDIR}/src/CDefaultHardware.o: src/CDefaultHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultHardware.o src/CDefaultHardware.cpp

${OBJECTDIR}/src/CDefaultLogger.o: src/CDefaultLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultLogger.o src/CDefaultLogger.cpp

${OBJECTDIR}/src/CFileDownloader.o: src/CFileDownloader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CFileDownloader.o src/CFileDownloader.cpp

${OBJECTDIR}/src/CGenericDevice.o: src/CGenericDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericDevice.o src/CGenericDevice.cpp

${OBJECTDIR}/src/CGenericHardware.o: src/CGenericHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericHardware.o src/CGenericHardware.cpp

${OBJECTDIR}/src/CGenericMessage.o: src/CGenericMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericMessage.o src/CGenericMessage.cpp

${OBJECTDIR}/src/CGenericMessageManager.o: src/CGenericMessageManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericMessageManager.o src/CGenericMessageManager.cpp

${OBJECTDIR}/src/CGenericSoftware.o: src/CGenericSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericSoftware.o src/CGenericSoftware.cpp

${OBJECTDIR}/src/CGenericTestPort.o: src/CGenericTestPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericTestPort.o src/CGenericTestPort.cpp

${OBJECTDIR}/src/CGenericTime.o: src/CGenericTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericTime.o src/CGenericTime.cpp

${OBJECTDIR}/src/CLinuxCommPort.o: src/CLinuxCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxCommPort.o src/CLinuxCommPort.cpp

${OBJECTDIR}/src/CLinuxCurrentTime.o: src/CLinuxCurrentTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxCurrentTime.o src/CLinuxCurrentTime.cpp

${OBJECTDIR}/src/CLinuxFile.o: src/CLinuxFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxFile.o src/CLinuxFile.cpp

${OBJECTDIR}/src/CLinuxHardware.o: src/CLinuxHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxHardware.o src/CLinuxHardware.cpp

${OBJECTDIR}/src/CLinuxPort.o: src/CLinuxPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxPort.o src/CLinuxPort.cpp

${OBJECTDIR}/src/CLinuxSocketClient.o: src/CLinuxSocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSocketClient.o src/CLinuxSocketClient.cpp

${OBJECTDIR}/src/CLinuxSocketServer.o: src/CLinuxSocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSocketServer.o src/CLinuxSocketServer.cpp

${OBJECTDIR}/src/CLinuxSoftware.o: src/CLinuxSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSoftware.o src/CLinuxSoftware.cpp

${OBJECTDIR}/src/CLinuxSpiPort.o: src/CLinuxSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSpiPort.o src/CLinuxSpiPort.cpp

${OBJECTDIR}/src/CLinuxZeroTime.o: src/CLinuxZeroTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxZeroTime.o src/CLinuxZeroTime.cpp

${OBJECTDIR}/src/CMapToPointer.o: src/CMapToPointer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CMapToPointer.o src/CMapToPointer.cpp

${OBJECTDIR}/src/CMessageManager.o: src/CMessageManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CMessageManager.o src/CMessageManager.cpp

${OBJECTDIR}/src/CProtocol.o: src/CProtocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CProtocol.o src/CProtocol.cpp

${OBJECTDIR}/src/CRawData.o: src/CRawData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CRawData.o src/CRawData.cpp

${OBJECTDIR}/src/CVxWorkCommPort.o: src/CVxWorkCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkCommPort.o src/CVxWorkCommPort.cpp

${OBJECTDIR}/src/CVxWorkHardware.o: src/CVxWorkHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkHardware.o src/CVxWorkHardware.cpp

${OBJECTDIR}/src/CVxWorkSoftware.o: src/CVxWorkSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkSoftware.o src/CVxWorkSoftware.cpp

${OBJECTDIR}/src/CVxWorkSpiPort.o: src/CVxWorkSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkSpiPort.o src/CVxWorkSpiPort.cpp

${OBJECTDIR}/src/CVxWorkThread.o: src/CVxWorkThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkThread.o src/CVxWorkThread.cpp

${OBJECTDIR}/src/CWindowsCommPort.o: src/CWindowsCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsCommPort.o src/CWindowsCommPort.cpp

${OBJECTDIR}/src/CWindowsCurrentTime.o: src/CWindowsCurrentTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsCurrentTime.o src/CWindowsCurrentTime.cpp

${OBJECTDIR}/src/CWindowsFile.o: src/CWindowsFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsFile.o src/CWindowsFile.cpp

${OBJECTDIR}/src/CWindowsHardware.o: src/CWindowsHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsHardware.o src/CWindowsHardware.cpp

${OBJECTDIR}/src/CWindowsPort.o: src/CWindowsPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsPort.o src/CWindowsPort.cpp

${OBJECTDIR}/src/CWindowsSocketClient.o: src/CWindowsSocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSocketClient.o src/CWindowsSocketClient.cpp

${OBJECTDIR}/src/CWindowsSocketServer.o: src/CWindowsSocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSocketServer.o src/CWindowsSocketServer.cpp

${OBJECTDIR}/src/CWindowsSoftware.o: src/CWindowsSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSoftware.o src/CWindowsSoftware.cpp

${OBJECTDIR}/src/CWindowsSpiPort.o: src/CWindowsSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSpiPort.o src/CWindowsSpiPort.cpp

${OBJECTDIR}/src/CWindowsZeroTime.o: src/CWindowsZeroTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsZeroTime.o src/CWindowsZeroTime.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/filedownloadertest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} -lpthread 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/TestProtocolclass.o ${TESTDIR}/tests/newtestrunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` -lpthread    

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/CLinuxServerSocketTest.o ${TESTDIR}/tests/testrunnerserversocket.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs` -lpthread    


${TESTDIR}/tests/filedownloadertest.o: tests/filedownloadertest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Iinc -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/filedownloadertest.o tests/filedownloadertest.cpp


${TESTDIR}/tests/TestProtocolclass.o: tests/TestProtocolclass.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Iinc -Iinc `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/TestProtocolclass.o tests/TestProtocolclass.cpp


${TESTDIR}/tests/newtestrunner.o: tests/newtestrunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Iinc -Iinc `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newtestrunner.o tests/newtestrunner.cpp


${TESTDIR}/tests/CLinuxServerSocketTest.o: tests/CLinuxServerSocketTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Iinc `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/CLinuxServerSocketTest.o tests/CLinuxServerSocketTest.cpp


${TESTDIR}/tests/testrunnerserversocket.o: tests/testrunnerserversocket.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Iinc `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testrunnerserversocket.o tests/testrunnerserversocket.cpp


${OBJECTDIR}/src/CBaseBlock_nomain.o: ${OBJECTDIR}/src/CBaseBlock.o src/CBaseBlock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CBaseBlock.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CBaseBlock_nomain.o src/CBaseBlock.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CBaseBlock.o ${OBJECTDIR}/src/CBaseBlock_nomain.o;\
	fi

${OBJECTDIR}/src/CControlLogger_nomain.o: ${OBJECTDIR}/src/CControlLogger.o src/CControlLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CControlLogger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CControlLogger_nomain.o src/CControlLogger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CControlLogger.o ${OBJECTDIR}/src/CControlLogger_nomain.o;\
	fi

${OBJECTDIR}/src/CDefaultDevice_nomain.o: ${OBJECTDIR}/src/CDefaultDevice.o src/CDefaultDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CDefaultDevice.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultDevice_nomain.o src/CDefaultDevice.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CDefaultDevice.o ${OBJECTDIR}/src/CDefaultDevice_nomain.o;\
	fi

${OBJECTDIR}/src/CDefaultHardware_nomain.o: ${OBJECTDIR}/src/CDefaultHardware.o src/CDefaultHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CDefaultHardware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultHardware_nomain.o src/CDefaultHardware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CDefaultHardware.o ${OBJECTDIR}/src/CDefaultHardware_nomain.o;\
	fi

${OBJECTDIR}/src/CDefaultLogger_nomain.o: ${OBJECTDIR}/src/CDefaultLogger.o src/CDefaultLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CDefaultLogger.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CDefaultLogger_nomain.o src/CDefaultLogger.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CDefaultLogger.o ${OBJECTDIR}/src/CDefaultLogger_nomain.o;\
	fi

${OBJECTDIR}/src/CFileDownloader_nomain.o: ${OBJECTDIR}/src/CFileDownloader.o src/CFileDownloader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CFileDownloader.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CFileDownloader_nomain.o src/CFileDownloader.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CFileDownloader.o ${OBJECTDIR}/src/CFileDownloader_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericDevice_nomain.o: ${OBJECTDIR}/src/CGenericDevice.o src/CGenericDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericDevice.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericDevice_nomain.o src/CGenericDevice.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericDevice.o ${OBJECTDIR}/src/CGenericDevice_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericHardware_nomain.o: ${OBJECTDIR}/src/CGenericHardware.o src/CGenericHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericHardware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericHardware_nomain.o src/CGenericHardware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericHardware.o ${OBJECTDIR}/src/CGenericHardware_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericMessage_nomain.o: ${OBJECTDIR}/src/CGenericMessage.o src/CGenericMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericMessage.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericMessage_nomain.o src/CGenericMessage.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericMessage.o ${OBJECTDIR}/src/CGenericMessage_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericMessageManager_nomain.o: ${OBJECTDIR}/src/CGenericMessageManager.o src/CGenericMessageManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericMessageManager.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericMessageManager_nomain.o src/CGenericMessageManager.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericMessageManager.o ${OBJECTDIR}/src/CGenericMessageManager_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericSoftware_nomain.o: ${OBJECTDIR}/src/CGenericSoftware.o src/CGenericSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericSoftware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericSoftware_nomain.o src/CGenericSoftware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericSoftware.o ${OBJECTDIR}/src/CGenericSoftware_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericTestPort_nomain.o: ${OBJECTDIR}/src/CGenericTestPort.o src/CGenericTestPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericTestPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericTestPort_nomain.o src/CGenericTestPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericTestPort.o ${OBJECTDIR}/src/CGenericTestPort_nomain.o;\
	fi

${OBJECTDIR}/src/CGenericTime_nomain.o: ${OBJECTDIR}/src/CGenericTime.o src/CGenericTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CGenericTime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CGenericTime_nomain.o src/CGenericTime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CGenericTime.o ${OBJECTDIR}/src/CGenericTime_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxCommPort_nomain.o: ${OBJECTDIR}/src/CLinuxCommPort.o src/CLinuxCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxCommPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxCommPort_nomain.o src/CLinuxCommPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxCommPort.o ${OBJECTDIR}/src/CLinuxCommPort_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxCurrentTime_nomain.o: ${OBJECTDIR}/src/CLinuxCurrentTime.o src/CLinuxCurrentTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxCurrentTime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxCurrentTime_nomain.o src/CLinuxCurrentTime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxCurrentTime.o ${OBJECTDIR}/src/CLinuxCurrentTime_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxFile_nomain.o: ${OBJECTDIR}/src/CLinuxFile.o src/CLinuxFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxFile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxFile_nomain.o src/CLinuxFile.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxFile.o ${OBJECTDIR}/src/CLinuxFile_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxHardware_nomain.o: ${OBJECTDIR}/src/CLinuxHardware.o src/CLinuxHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxHardware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxHardware_nomain.o src/CLinuxHardware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxHardware.o ${OBJECTDIR}/src/CLinuxHardware_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxPort_nomain.o: ${OBJECTDIR}/src/CLinuxPort.o src/CLinuxPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxPort_nomain.o src/CLinuxPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxPort.o ${OBJECTDIR}/src/CLinuxPort_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxSocketClient_nomain.o: ${OBJECTDIR}/src/CLinuxSocketClient.o src/CLinuxSocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxSocketClient.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSocketClient_nomain.o src/CLinuxSocketClient.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxSocketClient.o ${OBJECTDIR}/src/CLinuxSocketClient_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxSocketServer_nomain.o: ${OBJECTDIR}/src/CLinuxSocketServer.o src/CLinuxSocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxSocketServer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSocketServer_nomain.o src/CLinuxSocketServer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxSocketServer.o ${OBJECTDIR}/src/CLinuxSocketServer_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxSoftware_nomain.o: ${OBJECTDIR}/src/CLinuxSoftware.o src/CLinuxSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxSoftware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSoftware_nomain.o src/CLinuxSoftware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxSoftware.o ${OBJECTDIR}/src/CLinuxSoftware_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxSpiPort_nomain.o: ${OBJECTDIR}/src/CLinuxSpiPort.o src/CLinuxSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxSpiPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxSpiPort_nomain.o src/CLinuxSpiPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxSpiPort.o ${OBJECTDIR}/src/CLinuxSpiPort_nomain.o;\
	fi

${OBJECTDIR}/src/CLinuxZeroTime_nomain.o: ${OBJECTDIR}/src/CLinuxZeroTime.o src/CLinuxZeroTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CLinuxZeroTime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CLinuxZeroTime_nomain.o src/CLinuxZeroTime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CLinuxZeroTime.o ${OBJECTDIR}/src/CLinuxZeroTime_nomain.o;\
	fi

${OBJECTDIR}/src/CMapToPointer_nomain.o: ${OBJECTDIR}/src/CMapToPointer.o src/CMapToPointer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CMapToPointer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CMapToPointer_nomain.o src/CMapToPointer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CMapToPointer.o ${OBJECTDIR}/src/CMapToPointer_nomain.o;\
	fi

${OBJECTDIR}/src/CMessageManager_nomain.o: ${OBJECTDIR}/src/CMessageManager.o src/CMessageManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CMessageManager.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CMessageManager_nomain.o src/CMessageManager.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CMessageManager.o ${OBJECTDIR}/src/CMessageManager_nomain.o;\
	fi

${OBJECTDIR}/src/CProtocol_nomain.o: ${OBJECTDIR}/src/CProtocol.o src/CProtocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CProtocol.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CProtocol_nomain.o src/CProtocol.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CProtocol.o ${OBJECTDIR}/src/CProtocol_nomain.o;\
	fi

${OBJECTDIR}/src/CRawData_nomain.o: ${OBJECTDIR}/src/CRawData.o src/CRawData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CRawData.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CRawData_nomain.o src/CRawData.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CRawData.o ${OBJECTDIR}/src/CRawData_nomain.o;\
	fi

${OBJECTDIR}/src/CVxWorkCommPort_nomain.o: ${OBJECTDIR}/src/CVxWorkCommPort.o src/CVxWorkCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CVxWorkCommPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkCommPort_nomain.o src/CVxWorkCommPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CVxWorkCommPort.o ${OBJECTDIR}/src/CVxWorkCommPort_nomain.o;\
	fi

${OBJECTDIR}/src/CVxWorkHardware_nomain.o: ${OBJECTDIR}/src/CVxWorkHardware.o src/CVxWorkHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CVxWorkHardware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkHardware_nomain.o src/CVxWorkHardware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CVxWorkHardware.o ${OBJECTDIR}/src/CVxWorkHardware_nomain.o;\
	fi

${OBJECTDIR}/src/CVxWorkSoftware_nomain.o: ${OBJECTDIR}/src/CVxWorkSoftware.o src/CVxWorkSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CVxWorkSoftware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkSoftware_nomain.o src/CVxWorkSoftware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CVxWorkSoftware.o ${OBJECTDIR}/src/CVxWorkSoftware_nomain.o;\
	fi

${OBJECTDIR}/src/CVxWorkSpiPort_nomain.o: ${OBJECTDIR}/src/CVxWorkSpiPort.o src/CVxWorkSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CVxWorkSpiPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkSpiPort_nomain.o src/CVxWorkSpiPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CVxWorkSpiPort.o ${OBJECTDIR}/src/CVxWorkSpiPort_nomain.o;\
	fi

${OBJECTDIR}/src/CVxWorkThread_nomain.o: ${OBJECTDIR}/src/CVxWorkThread.o src/CVxWorkThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CVxWorkThread.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CVxWorkThread_nomain.o src/CVxWorkThread.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CVxWorkThread.o ${OBJECTDIR}/src/CVxWorkThread_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsCommPort_nomain.o: ${OBJECTDIR}/src/CWindowsCommPort.o src/CWindowsCommPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsCommPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsCommPort_nomain.o src/CWindowsCommPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsCommPort.o ${OBJECTDIR}/src/CWindowsCommPort_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsCurrentTime_nomain.o: ${OBJECTDIR}/src/CWindowsCurrentTime.o src/CWindowsCurrentTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsCurrentTime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsCurrentTime_nomain.o src/CWindowsCurrentTime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsCurrentTime.o ${OBJECTDIR}/src/CWindowsCurrentTime_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsFile_nomain.o: ${OBJECTDIR}/src/CWindowsFile.o src/CWindowsFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsFile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsFile_nomain.o src/CWindowsFile.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsFile.o ${OBJECTDIR}/src/CWindowsFile_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsHardware_nomain.o: ${OBJECTDIR}/src/CWindowsHardware.o src/CWindowsHardware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsHardware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsHardware_nomain.o src/CWindowsHardware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsHardware.o ${OBJECTDIR}/src/CWindowsHardware_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsPort_nomain.o: ${OBJECTDIR}/src/CWindowsPort.o src/CWindowsPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsPort_nomain.o src/CWindowsPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsPort.o ${OBJECTDIR}/src/CWindowsPort_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsSocketClient_nomain.o: ${OBJECTDIR}/src/CWindowsSocketClient.o src/CWindowsSocketClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsSocketClient.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSocketClient_nomain.o src/CWindowsSocketClient.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsSocketClient.o ${OBJECTDIR}/src/CWindowsSocketClient_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsSocketServer_nomain.o: ${OBJECTDIR}/src/CWindowsSocketServer.o src/CWindowsSocketServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsSocketServer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSocketServer_nomain.o src/CWindowsSocketServer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsSocketServer.o ${OBJECTDIR}/src/CWindowsSocketServer_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsSoftware_nomain.o: ${OBJECTDIR}/src/CWindowsSoftware.o src/CWindowsSoftware.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsSoftware.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSoftware_nomain.o src/CWindowsSoftware.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsSoftware.o ${OBJECTDIR}/src/CWindowsSoftware_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsSpiPort_nomain.o: ${OBJECTDIR}/src/CWindowsSpiPort.o src/CWindowsSpiPort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsSpiPort.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsSpiPort_nomain.o src/CWindowsSpiPort.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsSpiPort.o ${OBJECTDIR}/src/CWindowsSpiPort_nomain.o;\
	fi

${OBJECTDIR}/src/CWindowsZeroTime_nomain.o: ${OBJECTDIR}/src/CWindowsZeroTime.o src/CWindowsZeroTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CWindowsZeroTime.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -DENABLE_TEST -DUSE_LINUX -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CWindowsZeroTime_nomain.o src/CWindowsZeroTime.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CWindowsZeroTime.o ${OBJECTDIR}/src/CWindowsZeroTime_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libosinterfacelib.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
