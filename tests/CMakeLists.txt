add_executable(InstallmentCheck
    basic/installment_check.cpp
)

target_link_libraries(InstallmentCheck
    PRIVATE
    DrugLib.Common.Database.Base
    DrugLib.Dao.CrudBase
)