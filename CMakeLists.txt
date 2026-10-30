cmake_minimum_required(VERSION 3.10)
project(CSP2104_Assignment2 CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(Dictionary main.cpp Word.cpp Dictionary.cpp AdvancedDictionary.cpp
               HammingDistance.cpp LevenshteinDistance.cpp JaroWinklerDistance.cpp)
# Copy the dictionary next to the executable so "Load new Dictionary" finds it
configure_file(dictionary_2026S2.txt ${CMAKE_BINARY_DIR}/dictionary_2026S2.txt COPYONLY)
