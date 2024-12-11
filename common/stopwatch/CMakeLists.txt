##############################################################################
# Stopwatch class with measure purposes
##############################################################################
add_library(DrugLib_Common_Stopwatch
        source/stopwatch.cpp
        include/stopwatch.hpp
)
target_include_directories(DrugLib_Common_Stopwatch
        PUBLIC
        include/
)
##############################################################################