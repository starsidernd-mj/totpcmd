cmake_minimum_required(VERSION 3.0)
project(NcursesMainMenu)

# Find ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add your source files
set(SOURCES src/totpcmd.cpp)

# Create executable
add_executable(totpcmd ${SOURCES})

# Link ncurses
target_link_libraries(totpcmd ${CURSES_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)

