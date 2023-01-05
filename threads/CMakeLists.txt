add_library(thread src/display_thread.cpp)
target_include_directories(thread PUBLIC inc)
target_link_libraries(thread PUBLIC display bsp)