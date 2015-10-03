# Typing-Game
A simple typing game based on Java Swing.

## Builid Status

+ Windows: [![Build statuss](https://ci.appveyor.com/api/projects/status/s2gtfab69jsx043f?svg=true)](https://ci.appveyor.com/project/BUPTSSE-Zero/typing-game)

## Supported System Platform
+ Microsoft Windows XP or later.
+ Linux with GTK supported.

## Runtime Environment Requirements

### Windows:
+ JRE 1.6 or later.

### Linux
+ JRE 1.7 or later.
+ GTK+ 2.0

## License
GPLv3

## Build
+ Install [CMake](http://www.cmake.org/download).
+ Install [Java SE Development Kit](http://www.oracle.com/technetwork/java/javase/downloads/index.html)(1.6 or later).
+ (Linux) Install [GTK+2.0 Develop Library](http://www.gtk.org/download/index.php).(You can also install it through the software package manager in your system)
+ Switch to the root directory of the source code of `Typing-Game` project and execute the following commands to use `CMake` to generate project files and make this project.

```
mkdir build
cd build
cmake .. -G "${GENERATOR}" 
cmake --build .
```

The `${GENERATOR}` represents the generator which you want to use to generate the project files.You can execute the following command to find the available generators on your platform.

```
cmake --help
``` 

If you intend to use `Visual Studio` to make `64-bit` programs,you can append the identifier `Win64` to `${GENERATOR}`.

+ After building,all the productions can be find in `build/bin` directory.

##### Attenttion:if you modify the java resource files,please run `CMake` to generate the project files again.

## Text File Format
+ All the texts used in this game are stored in XML files.
+ The encoding of text files must be UTF-8.
+ The root element is `<typing-game>`.
+ Each  `<text>` element under the root element represents the text in each row.

#### For example:
``` 
<?xml version="1.0" encoding="UTF-8"?>
<typing-game>
	<text>Text in Row 1.</text>
	<text>Text in Row 2.</text>
	<text>Text in Row 3.</text>
</typing-game>
```
