cmake_minimum_required(VERSION 3.1)

find_package( OpenCV REQUIRED )

    include_directories( ${OpenCV_INCLUDE_DIRS}
               ${PROJECT_SOURCE_DIR}/include/
               
    )



set(SOURCES
    main.cpp
    Matcher.cpp
    HeatMap.cpp
    )
    
set(HEADERS
   # Matcher.hpp
    )


add_executable( DethcountBot ${SOURCES} ${HEADERS} )



#target_include_directories(DethcountBot PUBLIC ../include)

# Link each target with other targets or add options, etc.


# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(DethcountBot PRIVATE ${OpenCV_LIBS})





#target_link_libraries( DethcountBot ${OpenCV_LIBS} )

set_target_properties(DethcountBot PROPERTIES
            CXX_STANDARD 17
)


message("Include directories:")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)


