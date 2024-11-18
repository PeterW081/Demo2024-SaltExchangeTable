set(VAR0_PROJECT_NAME "Project20240827_SaltExchangeTable")

###
cmake_minimum_required(VERSION "3.12")
project("${VAR0_PROJECT_NAME}")
##X
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###
# find_package(GTest)
# > brew install googletest
# > sudo apt-get install -y libgtest-dev libgmock-dev
###
find_package(GTest CONFIG REQUIRED COMPONENTS GTest GMock)

###
FILE(GLOB ARR0_PROJECT_CXX_SOURCE "${PROJECT_SOURCE_DIR}/cxx_src/*.cxx" "${PROJECT_SOURCE_DIR}/cxx_src_gtest/*.cxx")
add_executable(
  "${VAR0_PROJECT_NAME}"
)
target_sources(
  "${VAR0_PROJECT_NAME}" PUBLIC
  PRIVATE "${ARR0_PROJECT_CXX_SOURCE}"
)
target_include_directories(
  "${VAR0_PROJECT_NAME}" PUBLIC
  "${PROJECT_SOURCE_DIR}/cxx_include/"
)
target_link_libraries(
  "${VAR0_PROJECT_NAME}" PUBLIC
  "GTest::gtest"
)

