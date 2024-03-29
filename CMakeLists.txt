cmake_minimum_required(VERSION 3.26)
project(Engine12)

set(CMAKE_CXX_STANDARD 23)

# Paths to sources and resources files
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(RESOURCES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources)

# Add include directory for GLFW and GLAD
include_directories(${INCLUDE_DIR})

# NOTE: Rewrite link_libraries and collect_files to one function
#       Syntax: collect_files(DIR EXT VAR)
#       DIR - path to search
#       EXT - list of extensions, e.g. "a lib dll so"
#       VAR - list where to store found files
#       After that do target_link_libraries(${PROJECT_NAME} ${VAR}) and etc.

# Recursive library searching and linking
function(link_libraries DIR)
    file(GLOB LIBRARIES ${DIR}/*.a ${DIR}/*.lib ${DIR}/*.dll ${DIR}/*.so)
    foreach(LIBRARY ${LIBRARIES})
        message(STATUS "Found library: ${LIBRARY}")
        # Linking library to project
        target_link_libraries(${PROJECT_NAME} ${LIBRARY} -lopengl32)
    endforeach()
    file(GLOB SUBDIRS ${DIR}/*)
    foreach(SUBDIR ${SUBDIRS})
        if(IS_DIRECTORY ${SUBDIR})
            link_libraries(${SUBDIR})
        endif()
    endforeach()
endfunction()

# Recursive source and resources files searching
function(collect_files DIR EXT COLLECTED_FILES)
    file(GLOB FILES ${DIR}/*.${EXT})
    file(GLOB SUBDIRS ${DIR}/*)
    foreach(SUBDIR ${SUBDIRS})
        if(IS_DIRECTORY ${SUBDIR})
            collect_files(${SUBDIR} ${EXT} SUB_FILES)
            list(APPEND FILES ${SUB_FILES})
        endif()
    endforeach()
    set(${COLLECTED_FILES} ${FILES} PARENT_SCOPE)
endfunction()


# Recursive .CPP and .H source files searching
collect_files(${SRC_DIR} "cpp" CPP_SOURCES)
collect_files(${SRC_DIR} "c" C_SOURCES)
collect_files(${INCLUDE_DIR} "h" H_SOURCES)

# Linking source files to .EXE
add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${H_SOURCES} ${C_SOURCES})

#link_libraries(${LIBS_DIR})
collect_files(${LIBS_DIR} "a" LIB_SOURCES)
target_link_libraries(${PROJECT_NAME} ${LIB_SOURCES} -lopengl32)

collect_files(${RESOURCES_DIR} "*" RESOURCE_FILES)

# Custom target to copy resources on build (if changed)
add_custom_target(copy_resources
        COMMAND ${CMAKE_COMMAND} -E remove_directory
            ${RESOURCES_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCES_DIR} ${RESOURCES_BINARY_DIR}
        DEPENDS ${RESOURCE_FILES}
)

add_dependencies(${PROJECT_NAME} copy_resources)
