cmake_minimum_required(VERSION 3.10)
project(GraphicEngine3D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# 1. Tenta encontrar a libpng no sistema
find_package(PNG REQUIRED)

include_directories(libs ${PNG_INCLUDE_DIRS})

add_executable(engine main.cpp)

# 2. Linka a libpng junto com as outras
if(MSVC)
    target_link_libraries(engine user32.lib gdi32.lib opengl32.lib gdiplus.lib)
else()
    # Adicionamos ${PNG_LIBRARIES} aqui
    target_link_libraries(engine X11 GL pthread ${PNG_LIBRARIES})
endif()
