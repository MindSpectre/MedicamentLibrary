##############################################################################
# Common utilities can be used outside
##############################################################################
add_library(DrugLib_Common_Utilities
        source/utilities.cpp
)
target_include_directories(DrugLib_Common_Utilities
        PUBLIC
        include/
)
##############################################################################