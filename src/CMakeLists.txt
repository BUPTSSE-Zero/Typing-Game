cmake_minimum_required (VERSION 2.6)

project (Typing-Game C Java)

set(JAVA_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/GlobalSettings.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/TextFileParser.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/Chronometer.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/ScrollView.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/AutoCheckDocument.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/GameInterface.java
		${PROJECT_SOURCE_DIR}/Java/buptsse/zero/MainInterface.java
		)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(CLASS_FILE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/java_out)
file(MAKE_DIRECTORY ${CLASS_FILE_OUTPUT_PATH}/buptsse/zero)
message(STATUS "Copying the Java resource files...")
file(COPY ${PROJECT_SOURCE_DIR}/Java/buptsse/zero/res DESTINATION ${CLASS_FILE_OUTPUT_PATH}/buptsse/zero)

add_custom_target(Typing-Game-Java ALL
								COMMAND javac -encoding utf-8 ${JAVA_SOURCE_FILES} -d ${CLASS_FILE_OUTPUT_PATH}
								COMMAND echo Generating JAR package...
								COMMAND jar cvfm ${EXECUTABLE_OUTPUT_PATH}/Typing-Game.jar ${PROJECT_SOURCE_DIR}/Java/MANIFEST.MF -C ${CLASS_FILE_OUTPUT_PATH} .
								)

include_directories($ENV{JAVA_HOME}/include)
if(WIN32)
	include_directories($ENV{JAVA_HOME}/include/win32)
endif()

if(MINGW)
	link_libraries(comctl32)
	set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
endif()

set(LAUNCHER_SOURCE_FILES
		C-Launcher/LauncherTool.h
		C-Launcher/LauncherMain.c)
if(WIN32)
	list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/LauncherTool-Win.c)
endif()

# Add resource files
if(WIN32)
    if(MINGW)
        set(CMAKE_RC_COMPILER_INIT windres)
        enable_language(RC)
        set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
		list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/Typing-Game-Launcher.rc)
	elseif(MSVC)
		list(APPEND LAUNCHER_SOURCE_FILES C-Launcher/Typing-Game-Launcher.rc)
	endif()
endif()

add_executable(Typing-Game-Launcher ${LAUNCHER_SOURCE_FILES})			
add_dependencies(Typing-Game-Launcher Typing-Game-Java)
					