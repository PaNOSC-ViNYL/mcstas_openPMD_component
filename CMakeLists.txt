# git clone --depth 1 git@github.com:openPMD/openPMD-api.git
# cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
cmake_minimum_required(VERSION 3.7.2)

project(CPP-helloworld LANGUAGES CXX C)

include(GNUInstallDirs)


add_subdirectory("openPMD-api")



#add_library(hello SHARED hello.cpp)
#target_link_libraries(hello PUBLIC openPMD::openPMD)

add_library(component SHARED component.cc particle.cc)
set_target_properties(component
  PROPERTIES LINKER_LANGUAGE CXX
  )
target_link_libraries(component PUBLIC openPMD::openPMD)

add_library(hellowrap SHARED hellowrap.cpp)
target_include_directories(hellowrap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(hellowrap PUBLIC m)
#target_link_libraries(hellowrap PUBLIC openPMD::openPMD)
target_link_libraries(hellowrap PRIVATE component)

add_custom_command(OUTPUT Test_CppWrap.c
  DEPENDS Test_CppWrap.instr
  DEPENDS CppWrap.comp
  COMMENT "creation of Test_CppWrap.c"
  COMMAND mcstas -t -o ${CMAKE_BINARY_DIR}/Test_CppWrap.c ${CMAKE_CURRENT_SOURCE_DIR}/Test_CppWrap.instr
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_executable(Test_CppWrap.out Test_CppWrap.c)
target_link_libraries(Test_CppWrap.out PRIVATE hellowrap)
target_link_libraries(Test_CppWrap.out PRIVATE m)
