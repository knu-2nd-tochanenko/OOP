# Basics of Object Oriented Programming

## Contents
### 1st term
* [Graph and IP Addresses (lab 1)](#graph-and-ip-addresses-lab-1)
* [Note App (lab 2)](#note-app-lab-2)
* [JSON to PDF converting (lab 3)](#json-to-pdf-converting-lab-3)
### 2nd Term
* [Medium UML Diagrams (lab 1.1)](#medium-uml-diagrams-lab-1.1)
* [Refactoring (lab 1.2)](#refactoring-lab-1.2)
* [Review (lab 1.3)](#review-lab-1.3)
* [Tree Visualization (lab 2)](#tree-visualization-lab-2)

## Graph and IP Addresses (lab 1)
[Go to code](https://github.com/knu-2nd-tochanenko/OOP/tree/master/Algorithms%2C%20Qt%20and%20Libraries/Graph-IP-Addresses)\
The first university project of the second year.\
The project has two major classes : Graph and IpAddress.\
The stage of it is mostly done. Please, report a bug, if you've found one.

## Note App (lab 2)
[Go to code](https://github.com/knu-2nd-tochanenko/OOP/tree/master/Algorithms%2C%20Qt%20and%20Libraries/Note-App)\
The second university project of the second year.\
**WARNING** If themes and .exe file are not presented in *Build* directory consider adding `install` as argument in `Build Settings -> Build Steps -> Add Build Step -> Make`\
This app has listed features:
* Moving notes to archive
* Notes type (work, personal)
* Tags
* Global hotkey
* Changeable themes (white - dark - orange)
* Exporting to .pdf file

## JSON to PDF converting (lab 3)
[Go to code](https://github.com/knu-2nd-tochanenko/OOP/tree/master/Algorithms%2C%20Qt%20and%20Libraries/JsonToPdf)\
This app will be used in the second university project as .exe file.\
The release version will be included in Note App directory.\
This project uses listed third-party libraries:
* [zlib](http://www.zlib.net/)
* [libPNG](http://www.libpng.org/)
* [libHARU](http://libharu.org/)
* [nlohmann/json](https://github.com/nlohmann/json)

### How to install libraries :
1. Download zlib, libpng, libharu and unzip archives. Also rename all folders to contain only lib name (delete lib version or -master postfix)
2. Install zlib :
    - Run CMake for zlib :
        Generator for project : Visual Studio 2017\
        `INSTALL_BIN_DIR` : path_to_zlib_folder/bin\
        `INSTALL_INC_DIR` : path_to_zlib_folder/include\
        `INSTALL_LIB_DIR` : path_to_zlib_folder/lib\
        `INSTALL_MAIN_DIR` : path_to_zlib_folder/share/man\
        `INSTALL_PKGCONFIG_DIR` : path_to_zlib_folder/pkgconfig
    - Open zlib.sln and build library in Release / Win32
3. Install libPNG
    - Run CMake for libPNG :
        Generator for project : Visual Studio 2017\
        Set **Advanced** to true\
        `ZLIB_INCLUDE_DIR` : path_to_zlib_folder\
        `ZLIB_LIBRARY` : path_to_zlib_folder/Release/zlib.lib\
        `ZLIB_LIBRARY_DEBUG` : ZLIB_LIBRARY_DEBUG-NOTFOUND\
        `ZLIB_LIBRARY_RELEASE` : ZLIB_LIBRARY_RELEASE-NOTFOUND\
        Set to true these configuration parameters : `PNG_SHARED`, `PNG_STATIC`, `PNG_TESTS`, `Id-version-script`
    - Open libpng.sln and build library in Release / Win32
4. Install libHARU
    - Run CMake for libHARU :
        Generator for project : Visual Studio 2017
    - Open libharu.sln and build library in Release / Win32
5. To this project:
    - VC++ Directories -> Include Directories -> paths to zlib, libpng, libharu directories
    - VC++ Directories -> Library Directories -> paths to zlib/Release, libpng/Release, libharu/Release directories
    - Linker -> Additional Dependencies -> zlib.lib;libpng16.lib;libhpdf.lib;
    - Download latest [json.hpp](https://github.com/nlohmann/json/releases) file
6. And now you are good to go!

## Medium UML Diagrams (lab 1.1)
[Visual Paradigm Project](https://github.com/knu-2nd-tochanenko/OOP/tree/master/UML/1_lab)\
[Rendered Diagrams (JPG)](https://github.com/knu-2nd-tochanenko/OOP/tree/master/UML/1_lab/images)\
The first project of second (fourth) term.\
The task is to create all of 14 diagrams of UML 2.0 standard for [Medium](https://medium.com)

## Refactoring (lab 1.2)
[Go to code](https://github.com/knu-2nd-tochanenko/OOP/tree/master/UML/2_lab)\
Refactoring of [Graph and IP Addresses lab](https://github.com/knu-2nd-tochanenko/OOP/tree/master/Algorithms%2C%20Qt%20and%20Libraries/Graph-IP-Addresses)\
This project uses third-party Unit Test library [Catch2](https://github.com/catchorg/Catch2)

## Review (lab 1.3)
* [1.1 Богдан Филоненко](https://github.com/knu-2nd-tochanenko/OOP/blob/master/UML/3_lab/1_filonenko.md)
* [1.1 Тимофій Шараєв](https://github.com/knu-2nd-tochanenko/OOP/blob/master/UML/3_lab/1_sharayev.md)
* [1.2 Кравчук Павло](https://github.com/knu-2nd-tochanenko/OOP/blob/master/UML/3_lab/2_kravchuk.md)
* [1.2 Микита Олійник](https://github.com/knu-2nd-tochanenko/OOP/blob/master/UML/3_lab/2_oliinyk.md)

## Tree Visualization (lab 2)
[Go to code](https://github.com/JustTaya/TreeVis)\
Tree visualization website made by Sergei Yarema, Taya Velychko, Vladislav Tochanenko and Mykyta Oliinyk.\
You can test it yourself by visiting [this beta version](http://wiki.tochanenko.com/tv/)