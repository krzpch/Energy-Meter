add_library(csv src/csv.cpp)
target_include_directories(csv PUBLIC inc)
target_link_libraries(
    csv
    PUBLIC bsp
)