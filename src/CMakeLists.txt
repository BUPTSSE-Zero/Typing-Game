cmake_minimum_required (VERSION 2.6)

project (Typing-Game C Java)

enable_language(Java)

set(JAVA_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/GlobalSettings.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/TextFileParser.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/Chronometer.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/ScrollView.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/AutoCheckDocument.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/GameInterface.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/MainInterface.java
		)

message(STATUS "Java Compiler detected:${CMAKE_Java_COMPILER}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(CLASS_FILE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/java_out)
file(MAKE_DIRECTORY ${CLASS_FILE_OUTPUT_PATH}/buptsse/zero)
message(STATUS "Copying the Java resource files...")
file(COPY ${PROJECT_SOURCE_DIR}/Java/buptsse/zero/res DESTINATION ${CLASS_FILE_OUTPUT_PATH}/buptsse/zero)

add_custom_target(Typing-Game-Java ALL
								COMMAND ${CMAKE_Java_COMPILER} -encoding utf-8 ${JAVA_SOURCE_FILES} -d ${CLASS_FILE_OUTPUT_PATH}
								COMMAND echo Generating JAR package...
								COMMAND ${CMAKE_Java_ARCHIVE} cvfm ${EXECUTABLE_OUTPUT_PATH}/Typing-Game.jar ${PROJECT_SOURCE_DIR}/Java/MANIFEST.MF -C ${CLASS_FILE_OUTPUT_PATH} .
								)

message(STATUS "Check Java Runtime Environment...")
find_package(Java REQUIRED)

message(STATUS "Try to find JNI package...")
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

if(NOT WIN32)
	message(STATUS "Try to find GTK+2 modules...")
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK2 gtk+-2.0 REQUIRED)
	include_directories(${GTK2_INCLUDE_DIRS})
	link_libraries(${GTK2_LIBRARIES})
endif()

set(JNI_RUNTIME_SOURCE
		Java/buptsse/zero/JNI_MainInterface.h
		Java/buptsse/zero/JNI_MainInterface.c
	)

	
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_library(Typing-Game-JNI-Runtime SHARED ${JNI_RUNTIME_SOURCE})
set_target_properties(Typing-Game-JNI-Runtime PROPERTIES OUTPUT_NAME "TypingGame")
if(WIN32)
	set_target_properties(Typing-Game-JNI-Runtime PROPERTIES PREFIX "")
endif()
	
	
set(LAUNCHER_SOURCE_FILES
		C-Launcher/LauncherTool.h
		C-Launcher/LauncherTool-Common.c
		C-Launcher/LauncherMain.c)
if(WIN32)
	list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/LauncherTool-Win.c)
	if(MINGW)
		link_libraries(comctl32)
		set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
	endif()	
else()
	list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/LauncherTool-Linux.c)
	link_libraries(dl)
endif()

# Add resource files
if(WIN32)
    if(MINGW)
		enable_language(RC)
        set(CMAKE_RC_COMPILER_INIT windres)
        set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
		list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/Typing-Game-Launcher.rc)
	elseif(MSVC)
		#64-bit
		if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
			add_definitions(/D "_M_AMD64")
		#32-bit
		elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
			add_definitions(/D "_M_IX86")
		endif()
		list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/Typing-Game-Launcher.rc)
	endif()
endif()

add_executable(Typing-Game-Launcher ${LAUNCHER_SOURCE_FILES})			
add_dependencies(Typing-Game-Launcher Typing-Game-Java)
add_dependencies(Typing-Game-Launcher Typing-Game-JNI-Runtime)
					
