cmake_minimum_required(VERSION 3.15)
project(RedisAssistant VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Find hiredis
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARY NAMES hiredis)

# Source files
set(SOURCES
    src/main.cpp
    src/RedisFrame.cpp
    src/RedisClient.cpp
)

# Create executable
add_executable(RedisAssistant ${SOURCES})

# Link libraries
target_include_directories(RedisAssistant PRIVATE 
    ${wxWidgets_INCLUDE_DIRS}
    ${HIREDIS_INCLUDE_DIR}
)

target_link_libraries(RedisAssistant 
    ${wxWidgets_LIBRARIES}
    ${HIREDIS_LIBRARY}
)

# Windows specific settings
if(WIN32)
    set_target_properties(RedisAssistant PROPERTIES
        WIN32_EXECUTABLE YES
    )
endif()

# Install rules
install(TARGETS RedisAssistant
    RUNTIME DESTINATION bin
)
