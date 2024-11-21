#pragma once

#include "super_handbook.hpp"


namespace drug_lib::services
{
    class PatientProfileServiceInternal
    {
    public:
        struct MedicamentSuggestion
        {
            enum MedicamentSuggestionType
            {
                remove,
                add
            };

            MedicamentSuggestionType suggest;
            data::objects::Medicament medicament;
        };

        enum DrugCompatibility
        {
            NoCompatibility = 0,
            CauseProblems = 1,
            CauseProblem = 2,
            Ok = 3
        };

        void diagnose(int32_t patient_id, int32_t disease_id);
        void assign_drug(int32_t patient_id, int32_t drug_id);
        std::vector<data::objects::Medicament> current_drugs(int32_t patient_id);
        std::vector<data::objects::Disease> current_diseases(int32_t patient_id);
        data::objects::Patient patient_profile(int32_t patient_id);
        bool is_dangerous(int32_t patient_id);
        DrugCompatibility medicament_compatibility(int32_t patient_id);
        std::vector<MedicamentSuggestion> recommend(int32_t patient_id);

        void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            handbook_.direct_establish(connect);
        }

        void pool_setup(common::database::creational::DbInterfacePool& pool)
        {
            handbook_.establish_from_pool(pool);
        }

        explicit PatientProfileServiceInternal(
            const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            setup_from_one(connect);
        }

        PatientProfileServiceInternal() = default;

    private:
        dao::SuperHandbook handbook_;
    };
}
