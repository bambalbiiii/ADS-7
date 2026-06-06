include(FetchContent)

set(header_path "${${PROJECT_NAME}_SOURCE_DIR}/include")
set(header ${header_path}/train.h)
set(src train.cpp)

add_library(${PROJECT_NAME} SHARED
${header}
${src})

target_include_directories(${PROJECT_NAME}
PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
${${PROJECT_NAME}_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
pthread)

FetchContent_Declare(
  lodepng
  GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(lodepng)

add_executable(${PROJECT_NAME}.info main.cpp)

target_include_directories(${PROJECT_NAME}.info
PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
${${PROJECT_NAME}_SOURCE_DIR}/include
${lodepng_SOURCE_DIR})

target_sources(${PROJECT_NAME}.info PRIVATE ${lodepng_SOURCE_DIR}/lodepng.cpp)

target_link_libraries(${PROJECT_NAME}.info
${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
LIBRARY DESTINATION "${lib_dest}"
ARCHIVE DESTINATION "${lib_dest}"
COMPONENT library)

install(TARGETS ${PROJECT_NAME}.info
RUNTIME DESTINATION "${tool_dest}"
COMPONENT library)

install(FILES ${header} DESTINATION "${include_dest}")
