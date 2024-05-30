cmake_minimum_required(VERSION 3.5.0)
project(Tesseract VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#find_path(Stb_INCLUDE_DIR "stb.h")



find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Stb REQUIRED)

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader::tinyobjloader)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/shaders/"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/assets/"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
