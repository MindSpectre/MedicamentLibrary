add_executable(InstallmentCheck
        basic/installment_check.cpp
)

target_link_libraries(InstallmentCheck
        PRIVATE
        DrugLib.Dao.CrudInterface
        DrugLib.Dao.Handbook.Medicaments
)
