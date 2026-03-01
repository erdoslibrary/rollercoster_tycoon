cmake_minimum_required(VERSION 3.10)
project(RCTNative)

# C++11 표준 강제
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SDL2 검색 및 설정
find_package(SDL2 REQUIRED)

# 소스 파일 정의
set(SOURCES
    src/main.cpp
    src/render/Renderer.cpp
    src/render/UI.cpp
    src/game/World.cpp
    src/game/Guest.cpp
    src/game/Pathfinder.cpp
    src/game/Track.cpp
    src/game/Train.cpp
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# 헤더 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    src
)

# 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${SDL2_LIBRARIES}
)
