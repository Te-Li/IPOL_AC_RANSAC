CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(mmm_orsa C CXX)

ENABLE_TESTING()

INCLUDE_DIRECTORIES(
  ./demo/
  ./third_party/
  ./
)

IF (WIN32)
  INCLUDE_DIRECTORIES(
    third_party/jpeg
    third_party/zlib
    third_party/png)
ENDIF (WIN32)

MACRO (UNIT_TEST NAME EXTRA_LIBS)
  ADD_EXECUTABLE(${NAME}_test ${NAME}_test.cpp)
  TARGET_LINK_LIBRARIES(${NAME}_test
                        ${EXTRA_LIBS} # Extra libs MUST be first.
                        CppUnitLite)
  ADD_TEST(${NAME}_test ${NAME}_test)
ENDMACRO (UNIT_TEST)

ADD_SUBDIRECTORY(third_party) # Third party libraries
ADD_SUBDIRECTORY(libOrsa)     # Orsa Core library
ADD_SUBDIRECTORY(demo)        # Demo directory
