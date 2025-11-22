cmake_minimum_required(VERSION 3.20)
project(pocker_2206)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/HandEvaluator.cpp
    src/HandComparator.cpp
    src/Player.cpp
    src/Game.cpp
)

# Header files (for IDEs)
set(HEADERS
    include/Card.h
    include/Deck.h
    include/Hand.h
    include/HandEvaluator.h
    include/HandComparator.h
    include/Player.h
    include/Game.h
)

add_executable(poker_2206 ${SOURCES} ${HEADERS})
