cmake_minimum_required(VERSION 3.10)
project(moteustest1)

# set(MAIN_PROJECT OFF)


include(FetchContent)
FetchContent_Declare(
  moteus
  GIT_REPOSITORY https://github.com/mjbots/moteus.git
  GIT_TAG        2a44cf4e27902cb786d154b85b56bf5d9b567c22
)

FetchContent_MakeAvailable(moteus)

add_executable(test1 test1.cc)
target_link_libraries(test1 moteus::cpp)