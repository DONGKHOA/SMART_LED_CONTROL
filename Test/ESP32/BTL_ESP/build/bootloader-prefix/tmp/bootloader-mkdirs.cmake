# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.2.1/components/bootloader/subproject"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/tmp"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/src/bootloader-stamp"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/src"
  "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/BTL_ESP/Test/ESP32/BTL_ESP/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
