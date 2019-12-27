cmake_minimum_required(VERSION 3.1)

find_package( OpenCV REQUIRED )



FIND_PATH(TESSERACT_INCLUDE_DIRS 
  baseapi.h
  /usr/include/tesseract
  /usr/local/include
  /usr/local/include/tesseract
  )
MESSAGE(STATUS "TESSERACT_INCLUDE_DIRS: ${TESSERACT_INCLUDE_DIRS}")

FIND_LIBRARY(TESSERACT_LIBRARY 
  NAMES tesseract tesseract_api
  PATHS 
  /usr/lib
  /usr/lib64
)
MESSAGE(STATUS "TESSERACT_LIBRARY ${TESSERACT_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set TESSERACT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TESSERACT  DEFAULT_MSG TESSERACT_LIBRARY TESSERACT_INCLUDE_DIRS)

IF(TESSERACT_FOUND)
#  MESSAGE(STATUS "TESSERACT found")
  SET( TESSERACT_LIBRARIES ${TESSERACT_LIBRARY} )
ENDIF(TESSERACT_FOUND)

MARK_AS_ADVANCED(TESSERACT_INCLUDE_DIR TESSERACT_LIBRARY)

FIND_PATH(LEPTONICA_INCLUDE_DIRS 
  allheaders.h
  /usr/local/include
  /usr/include/leptonica
)


MESSAGE(STATUS "LEPTONICA_INCLUDE_DIRS   ${LEPTONICA_INCLUDE_DIRS}")



    include_directories( ${OpenCV_INCLUDE_DIRS}
                        ${Tesseract_INCLUDE_DIRS}
                        ${PROJECT_SOURCE_DIR}/include/
                        ${LEPTONICA_INCLUDE_DIRS}    
    )



set(SOURCES
    main.cpp
    Matcher.cpp
    HeatMap.cpp
    lodepng.cpp
    EastDetector.cpp
    Detector.cpp
    )
#idk why headers are not needed loool
set(HEADERS
   # Matcher.hpp
    )


add_executable( DethcountBot ${SOURCES} ${HEADERS} )



#target_include_directories(DethcountBot PUBLIC ../include)

# Link each target with other targets or add options, etc.


FIND_LIBRARY(LEPTONICA_LIBRARY 
  NAMES 
  liblept.a
  liblept.so 
  liblept.so.5
  liblept.so.5.0.2
  libjpeg.so.8
  PATHS 
  /usr/lib
  /usr/lib64
  /usr/lib/x86_64-linux-gnu
)
FIND_LIBRARY(LIB_JPEG 
  NAMES 
  libjpeg.so.8
  PATHS 
  /usr/lib/x86_64-linux-gnu
)

FIND_LIBRARY(LIB_PNG 
  NAMES 
  libpng16.so.16
  PATHS 
  /usr/lib/x86_64-linux-gnu
)

FIND_LIBRARY(LIB_Z 
  NAMES 
  libz.so.1
  PATHS 
  /usr/lib/x86_64-linux-gnu
)
FIND_LIBRARY(LIB_TIFF 
  NAMES 
  libtiff.so.5
  PATHS 
  /usr/lib/x86_64-linux-gnu
)

FIND_LIBRARY(LIB_GIF
  NAMES 
  libgif.so.7
  PATHS 
  /usr/lib/x86_64-linux-gnu
)


FIND_LIBRARY(LIB_OPENJP2
  NAMES 
  libopenjp2.so.7
  PATHS 
  /usr/lib/x86_64-linux-gnu
)

FIND_LIBRARY(LIB_CURL 
  NAMES 
libcurl.so
libcurl.so.4
libcurl.so.4.5.0
libcurl-gnutls.so.3
libcurl-gnutls.so.4
libcurl-gnutls.so.4.5.0
libcurl.a
libcurl.la
  PATHS 
  /usr/lib/x86_64-linux-gnu
)

# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(  DethcountBot
                        PRIVATE
                        ${OpenCV_LIBS}
                        ${TESSERACT_LIBRARY}
                        ${LEPTONICA_LIBRARY}
                        ${LIB_JPEG}
                        ${LIB_PNG}
                        ${LIB_Z}
                        ${LIB_TIFF}
                        ${LIB_GIF}
                        ${LIB_OPENJP2}
                        ${LIB_CURL}
                        )





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


