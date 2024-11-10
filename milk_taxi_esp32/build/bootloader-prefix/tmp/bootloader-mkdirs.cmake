# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/adamj/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/tmp"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/src"
  "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/adamj/Desktop/Milk_taxi/esp_32/milk_taxi_esp32/milk_taxi_esp32/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
