# Huffman algorithm

Implemented a library and utility to compress using the Huffman algorithm.

The repository contains:
1. The library that implements compression and decompression algorithms;
2. The utility that compresses or decompresses files on disk using the library.

## Features

* The _library_ compiles into a dynamic library. 
* The _library_ is capable of (de-)compressing files with a size larger than the available RAM.
* The _library_ is covered by unit tests.
* The _utility_ has the following interface: 
  * --comress&emsp;&emsp;&emsp;&nbsp;\[-c]&emsp;|&emsp;Activate compress state
  * --decompress&emsp;&ensp;\[-d]&emsp;|&emsp;Activate decompress state
  * --input \<string\>&emsp;\[-i]&emsp;|&emsp;Name of the input file
  * --output \<string\> \[-o]&emsp;|&emsp;Name of the output file
  * --status&emsp;&emsp;&emsp;&emsp;&nbsp;\[-s]&emsp;|&emsp;Enable status output to the console
  * --help&emsp;&emsp;&emsp;&emsp;&emsp;\[-h]&emsp;|&emsp;Display usage information  
* The _utility_ print an error message to the standard error stream and exit with a non-zero code if the file being unpacked is corrupted, an unexpected I/O problem has occurred, or any similar problem.

## Dependencies and some usage info

* The _utility_ requires "Templatized C++ Command Line Parser Library" (TCLAP)
* Unit-tests require "Gtest"
* The _library_ requires nothing
* It is **recommended** to install vcpkg to manage tclap and gtest
* With vcpkg installed in the directory above the project directory you can build the project with the script ci-extra/build.sh (you can change path to vcpkg in this script if necessary)
* ./ci-extra/buid.sh \[Release, Debug, SanitizedDebug, RelWithDebInfo] to build the project
* ./ci-extra/test.sh \[Release, Debug, SanitizedDebug, RelWithDebInfo] to test the library
 