cmake_minimum_required(VERSION 3.22)
project(suzuki_sdl VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(PROJECT_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wformat=2
    -Wundef
    -Wnull-dereference
    -Wwrite-strings
    -Wcast-qual
    -Wcast-align
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wdouble-promotion
    -Wconversion
)
set(TEST_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wformat=2
    -Wundef
    -Wnull-dereference
    -Wwrite-strings
    -Wcast-qual
    -Wcast-align
    -Wdouble-promotion
    -Wconversion
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(${PROJECT_NAME}_static STATIC src/suzuki_sdl.c)
add_library(${PROJECT_NAME}_shared SHARED src/suzuki_sdl.c)
set_target_properties(
    ${PROJECT_NAME}_static
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)
set_target_properties(
    ${PROJECT_NAME}_shared
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)
foreach(target ${PROJECT_NAME}_static ${PROJECT_NAME}_shared)
    target_include_directories(${target} PUBLIC include PRIVATE src)
    target_compile_options(${target} PRIVATE ${PROJECT_COMPILE_OPTIONS})
endforeach()

if(BUILD_TESTING)
    enable_testing()
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.6.1
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(unity)
    add_executable(test_lib tests/test_lib.c)
    add_executable(test_calc tests/test_calc.c)
    target_link_libraries(test_lib PRIVATE ${PROJECT_NAME}_static unity m)
    target_link_libraries(test_calc PRIVATE ${PROJECT_NAME}_static unity m)
    target_compile_options(test_lib PRIVATE ${TEST_COMPILE_OPTIONS})
    target_compile_options(test_calc PRIVATE ${TEST_COMPILE_OPTIONS})
    add_test(NAME SuzukiSDLProtocolLibTests COMMAND test_lib --verbose)
    add_test(NAME SuzukiSDLProtocolCalcTests COMMAND test_calc --verbose)
endif()

install(
    TARGETS ${PROJECT_NAME}_static ${PROJECT_NAME}_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
