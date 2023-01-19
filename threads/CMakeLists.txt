add_library(thread src/display_thread.cpp src/meter_thread.cpp src/csv_thread.cpp)
target_include_directories(thread PUBLIC inc)
target_link_libraries(thread PUBLIC display ina219 csv bsp)