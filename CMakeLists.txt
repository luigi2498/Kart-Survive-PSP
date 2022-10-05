cmake_minimum_required(VERSION 3.17)
project(KartSurvive)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(KartSurvive main.c)
add_subdirectory(./QuickGame/ QuickGame)

target_link_libraries(KartSurvive PUBLIC QuickGame)
target_include_directories(KartSurvive PUBLIC QuickGame/include)

if (PLATFORM_PSP)
    create_pbp_file(
        TARGET KartSurvive
        TITLE Kart_Survive_PSP_v0.1.0-by-luigi2498
        ICON_PATH ../pbpicons/ICON0.png
        BACKGROUND_PATH ../pbpicons/PIC1.png
        BUILD_PRX ON
    )
endif()