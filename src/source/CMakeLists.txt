cmake_minimum_required(VERSION 3.1)

find_package( OpenCV REQUIRED )
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
include_directories( ${OpenCV_INCLUDE_DIRS}
              ${PROJECT_SOURCE_DIR}/include/
)

set(SOURCES
    main.cpp
    Matcher.cpp
    )
    
set(HEADERS
     Matcher.hpp
    )



add_executable( DethcountBot ${SOURCES} ${HEADERS} )

target_include_directories(DethcountBot PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries( DethcountBot ${OpenCV_LIBS} )
set_target_properties(DethcountBot PROPERTIES
            CXX_STANDARD 17
)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/)
