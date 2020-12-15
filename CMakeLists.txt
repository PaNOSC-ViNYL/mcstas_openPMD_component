# git clone --depth 1 git@github.com:openPMD/openPMD-api.git
# cmake .. -DopenPMD_USE_PYTHON=OFF -DopenPMD_INSTALL=OFF -DopenPMD_USE_INTERNAL_CATCH=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_CLI_TOOLS=OFF
cmake_minimum_required(VERSION 3.7.2)

project(openpmd_output LANGUAGES CXX C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS True)
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

# Options and Variants ########################################################
#
function(openpmd_option name description default)
    set(openPMD_USE_${name} ${default} CACHE STRING "${description}")
    set_property(CACHE openPMD_USE_${name} PROPERTY
        STRINGS "ON;TRUE;AUTO;OFF;FALSE"
    )
    if(openPMD_HAVE_${name})
        set(openPMD_HAVE_${name} TRUE)
    else()
        set(openPMD_HAVE_${name})
    endif()
    # list of all possible options
    set(openPMD_CONFIG_OPTIONS ${openPMD_CONFIG_OPTIONS} ${name} PARENT_SCOPE)
endfunction()

##
option(TEST "Do testing" OFF)

##
option(MPI "Parallel, Multi-Node I/O for clusters" OFF)
# MPI is used already by mcstas, don't need to further make I/O parallel with MPI on the openPMD side
if(MPI)
  find_package(MPI REQUIRED)
  # disable MPI for openPMD! use the serial version!
  set(openPMD_USE_MPI OFF)
  message("HDF5_ROOT: /${CMAKE_INSTALL_LIBDIR}")
  set(HDF5_ROOT /) # this because mpi dir is pre-pended, so it would always find the mpi version of hdf5
   
else()
  set(openPMD_USE_MPI OFF)


endif()


#########################################################
#add_subdirectory("openPMD-api")
find_package(openPMD REQUIRED)
find_package(openPMDraytrace REQUIRED)


# add_library(component SHARED src/openPMD_io.cc src/particles.cc)
# set_target_properties(component
#   PROPERTIES LINKER_LANGUAGE CXX
#   )
# target_include_directories(component PUBLIC include/)
# set_property(TARGET component PROPERTY CXX_STANDARD 17)
# target_link_libraries(component PUBLIC openPMD::openPMD)

add_library(wrap SHARED src/wrap.cc)
target_include_directories(wrap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
#target_link_libraries(wrap PRIVATE component)
target_link_libraries(wrap PRIVATE raytrace) # openPMD::openPMD)
#set_property(TARGET wrap PROPERTY CXX_STANDARD 17)

if(TEST)
  
add_custom_command(OUTPUT Test_write_openPMD.c
  DEPENDS mcstas_component/Test_write_openPMD.instr
  DEPENDS mcstas_component/write_openPMD.comp
  COMMENT "creation of Test_write_openPMD.c"
  COMMAND mcstas -t -o ${CMAKE_BINARY_DIR}/Test_write_openPMD.c ${CMAKE_CURRENT_SOURCE_DIR}/mcstas_component/Test_write_openPMD.instr -Imcstas_component/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_custom_command(OUTPUT Test_read_openPMD.c
  DEPENDS mcstas_component/Test_read_openPMD.instr
  DEPENDS mcstas_component/read_openPMD.comp
  COMMENT "creation of Test_read_openPMD.c"
  COMMAND mcstas -t -o ${CMAKE_BINARY_DIR}/Test_read_openPMD.c ${CMAKE_CURRENT_SOURCE_DIR}/mcstas_component/Test_read_openPMD.instr -Imcstas_component/
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

add_executable(Test_write_openPMD.out Test_write_openPMD.c)
target_link_libraries(Test_write_openPMD.out PRIVATE wrap)
target_link_libraries(Test_write_openPMD.out PRIVATE m)
# for MPI
#target_include_directories(Test_write_openPMD.out PRIVATE ${MPI_CXX_INCLUDE_PATH})

#target_link_libraries(Test_write_openPMD.out PUBLIC ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS})
if(MPI_FOUND)
  #    message("MPI: ${MPI_CXX_LIBRARIES}")
      message("MPI: ${MPIEXEC_EXECUTABLE}")
      target_link_libraries(Test_write_openPMD.out PUBLIC ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} MPI::MPI_C)
      target_compile_options(Test_write_openPMD.out     PRIVATE -DUSE_MPI)
      #target_compile_options(Test_write_openPMD.out     PRIVATE ${MPI_CXX_COMPILE_FLAGS})
endif()

# for testing openPMD vs MCPL
target_include_directories(Test_write_openPMD.out PRIVATE $ENV{MCSTAS}/libs/mcpl/)
target_link_libraries(Test_write_openPMD.out PRIVATE $ENV{MCSTAS}/libs/mcpl/libmcpl.a)

add_executable(Test_read_openPMD.out Test_read_openPMD.c)
target_link_libraries(Test_read_openPMD.out PRIVATE wrap)
target_link_libraries(Test_read_openPMD.out PRIVATE m)

enable_testing()
add_test(NAME short COMMAND
  ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS}
  ${MPIEXEC_PREFLAGS} ${CMAKE_BINARY_DIR}/bin/Test_write_openPMD.out ${MPIEXEC_POSTFLAGS}  -n 100 -s 654321 filename=openPMD_out.h5 
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME mid COMMAND ${MPIEXEC_EXECUTABLE} Test_write_openPMD.out -n 1e6  -s 654321  filename=openPMD_out.h5
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME long COMMAND ${MPIEXEC_EXECUTABLE} Test_write_openPMD.out -n 1e7  -s 654321 filename=openPMD_out.h5
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME read COMMAND Test_read_openPMD.out filename=openPMD_out.h5 verbose=1
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

add_test(NAME read_repeat COMMAND Test_read_openPMD.out filename=openPMD_out.h5 verbose=1 repeat_count=3
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/)

endif()


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


