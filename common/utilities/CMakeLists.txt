##############################################################################
# Common utilities can be used outside
##############################################################################
add_library(DrugLib_Common_Utilities
        source/utilities.cpp
        include/compile_time_utils.hpp
        include/security_utils.hpp
)
target_include_directories(DrugLib_Common_Utilities
        PUBLIC
        include/
)
##############################################################################