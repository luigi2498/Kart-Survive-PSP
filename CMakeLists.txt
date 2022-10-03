cmake_minimum_required(VERSION 3.17)
project(KartSurvive)

set(CMAKE_C_STANDARD 11)

add_executable(KartSurvive main.c)
add_subdirectory(./QuickGame QuickGame)

target_link_libraries(KartSurvive PUBLIC QuickGame)
target_include_directories(KartSurvive PUBLIC QuickGame/include)

create_pbp_file(
    TARGET KartSurvive
    TITLE Kart Survive PSP
    BUILD_PRX ON
)