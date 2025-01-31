cmake_minimum_required(VERSION 3.10)
project(johma_bot C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ソースファイル
file(GLOB_RECURSE SOURCES 
    "*.c"
    "libs/*.c"
    "commands/*.c"
)

# 実行ファイル
add_executable(johma_bot ${SOURCES})

# インクルードディレクトリ
target_include_directories(johma_bot PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/include
)

# コンパイラフラグ
target_compile_options(johma_bot PRIVATE
    -Wall
    -Wextra
    -pthread
    -no-pie
)

# リンクライブラリ
target_link_libraries(johma_bot
    discord
    curl
    pthread
)