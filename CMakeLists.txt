# git clone --depth 1 git@github.com:openPMD/openPMD-api.git
# cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
cmake_minimum_required(VERSION 3.7.2)

project(CPP-helloworld LANGUAGES CXX C)

include(GNUInstallDirs)


add_subdirectory("openPMD-api")



#add_library(hello SHARED hello.cpp)
#target_link_libraries(hello PUBLIC openPMD::openPMD)

add_library(component SHARED src/openPMD_io.cc src/particles.cc)
set_target_properties(component
  PROPERTIES LINKER_LANGUAGE CXX
  )
target_include_directories(component PUBLIC include/)
set_property(TARGET component PROPERTY CXX_STANDARD 17)
target_link_libraries(component PUBLIC openPMD::openPMD)

add_library(wrap SHARED src/wrap.cc)
target_include_directories(wrap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(wrap PRIVATE component)
set_property(TARGET wrap PROPERTY CXX_STANDARD 17)

add_custom_command(OUTPUT Test_CppWrap.c
  DEPENDS mcstas_component/Test_CppWrap.instr
  DEPENDS mcstas_component/CppWrap.comp
  COMMENT "creation of Test_CppWrap.c"
  COMMAND mcstas -t -o ${CMAKE_BINARY_DIR}/Test_CppWrap.c ${CMAKE_CURRENT_SOURCE_DIR}/mcstas_component/Test_CppWrap.instr -Imcstas_component/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_custom_command(OUTPUT Test_read_openPMD.c
  DEPENDS mcstas_component/Test_read_openPMD.instr
  DEPENDS mcstas_component/read_openPMD.comp
  COMMENT "creation of Test_read_openPMD.c"
  COMMAND mcstas -t -o ${CMAKE_BINARY_DIR}/Test_read_openPMD.c ${CMAKE_CURRENT_SOURCE_DIR}/mcstas_component/Test_read_openPMD.instr -Imcstas_component/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_executable(Test_CppWrap.out Test_CppWrap.c)
target_link_libraries(Test_CppWrap.out PRIVATE wrap)
target_link_libraries(Test_CppWrap.out PRIVATE m)

add_executable(Test_read_openPMD.out Test_read_openPMD.c)
target_link_libraries(Test_read_openPMD.out PRIVATE wrap)
target_link_libraries(Test_read_openPMD.out PRIVATE m)

enable_testing()
add_test(NAME short COMMAND Test_CppWrap.out -n 100 -s 654321 --exe $<TARGET_FILE:Test_CppWrap.out>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME mid COMMAND Test_CppWrap.out -n 1e6  -s 654321 --exe $<TARGET_FILE:Test_CppWrap.out>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME long COMMAND Test_CppWrap.out -n 1e8  -s 654321 --exe $<TARGET_FILE:Test_CppWrap.out>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME read COMMAND Test_read_openPMD.out --exe $<TARGET_FILE:Test_read_openPMD.out>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)




################ Doc
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  
  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")
  
  # note the option ALL which allows to build the docs together with the application
  add_custom_target( doc 
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)


