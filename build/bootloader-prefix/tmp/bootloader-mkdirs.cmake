# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Application/ESP-IDF/Espressif/components/bootloader/subproject"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/tmp"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/src"
  "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Desktop/ESP32Project/10_Elysia/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
