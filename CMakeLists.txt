cmake_minimum_required(VERSION 2.8)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

FLEX_TARGET(Scanner src/uri.l  ${CMAKE_CURRENT_BINARY_DIR}/uri.yy.c)
BISON_TARGET(Parser src/uri.y ${CMAKE_CURRENT_BINARY_DIR}/uri.tab.c)

include_directories ("${PROJECT_SOURCE_DIR}/include")

set(LibHttp_SRC
  src/dns.c
  src/uri.c
  src/downloader.c
  ${FLEX_Scanner_OUTPUTS}
  ${BISON_Parser_OUTPUTS}
  )

add_library(http SHARED ${LibHttp_SRC})

set(HttpClient_SRC
  src/main.c
  )

add_executable(httpc ${HttpClient_SRC})
target_link_libraries(httpc http)



