cmake_minimum_required(VERSION 3.1)
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( DethcountBot main.cpp )

target_link_libraries( DethcountBot ${OpenCV_LIBS} )
set_target_properties(DethcountBot PROPERTIES
            CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/)
