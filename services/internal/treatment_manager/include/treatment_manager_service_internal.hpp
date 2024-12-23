#pragma once

#include "super_handbook.hpp"


namespace drug_lib::services
{
    class TreatmentManagerServiceInternal
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

        void assign_disease(common::database::Uuid patient_id, common::database::Uuid disease_id);
        void remove_disease(common::database::Uuid patient_id, const common::database::Uuid &disease_id);
        void cure_disease(common::database::Uuid patient_id, const common::database::Uuid &disease_id);
        void assign_medicament(common::database::Uuid patient_id, common::database::Uuid drug_id);
        void remove_medicament(common::database::Uuid patient_id, const common::database::Uuid &drug_id);
        std::vector<data::objects::Medicament> current_medicaments(common::database::Uuid patient_id);
        std::vector<data::objects::Disease> current_diseases(common::database::Uuid patient_id);
        data::objects::Patient patient_profile(common::database::Uuid patient_id);
        MedicamentSuggestion suggest_medicament(const common::database::Uuid &patient_id);
        bool is_dangerous(common::database::Uuid patient_id);

        void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            handbook_.direct_establish(connect);
        }

        void pool_setup(common::database::creational::DbInterfacePool& pool)
        {
            handbook_.establish_from_pool(pool);
        }

        explicit TreatmentManagerServiceInternal(
            const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            setup_from_one(connect);
        }

        TreatmentManagerServiceInternal() = default;

    private:
        dao::SuperHandbook handbook_;
    };
}
