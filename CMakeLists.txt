# git clone --depth 1 git@github.com:openPMD/openPMD-api.git
# cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
cmake_minimum_required(VERSION 3.7.2)

project(CPP-helloworld LANGUAGES CXX C)

include(GNUInstallDirs)


add_subdirectory("openPMD-api")



#add_library(hello SHARED hello.cpp)
#target_link_libraries(hello PUBLIC openPMD::openPMD)

add_library(component SHARED src/component.cc src/particle.cc)
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

add_executable(Test_CppWrap.out Test_CppWrap.c)
target_link_libraries(Test_CppWrap.out PRIVATE wrap)
target_link_libraries(Test_CppWrap.out PRIVATE m)

enable_testing()
add_test(NAME ten COMMAND Test_CppWrap.out -n 10 --exe $<TARGET_FILE:Test_CppWrap.out>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)
