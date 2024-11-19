#pragma once

#include <db_interface_pool.hpp>

#include "patients_handbook.hpp"
#include "medicaments_handbook.hpp"
#include "diseases_handbook.hpp"
#include "organizations_handbook.hpp"


namespace drug_lib::dao
{
    class SuperHandbook final
    {
    public:
        [[nodiscard]] PatientsHandbook& patients()
        {
            return patients_;
        }

        void set_patients(PatientsHandbook patients)
        {
            patients_ = std::move(patients);
        }

        [[nodiscard]] MedicamentsHandbook& medicaments()
        {
            return medicaments_;
        }

        void set_medicaments(MedicamentsHandbook medicaments)
        {
            medicaments_ = std::move(medicaments);
        }

        [[nodiscard]] OrganizationsHandbook& organizations()
        {
            return organizations_;
        }

        void set_organizations(OrganizationsHandbook organizations)
        {
            organizations_ = std::move(organizations);
        }

        [[nodiscard]] DiseaseHandbook& diseases()
        {
            return diseases_;
        }

        void set_diseases(DiseaseHandbook diseases)
        {
            diseases_ = std::move(diseases);
        }

        void direct_establish(const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            patients_.set_connection(connect);
            medicaments_.set_connection(connect);
            organizations_.set_connection(connect);
            diseases_.set_connection(connect);
        }

        void establish_from_pool(common::database::creational::DbInterfacePool& pool)
        {
            shared_connect = pool.acquire_db_interface();
            direct_establish(shared_connect);
            used_pool = pool;
        }

        ~SuperHandbook()
        {
            patients_.drop_connection();
            medicaments_.drop_connection();
            organizations_.drop_connection();
            diseases_.drop_connection();
            if (used_pool)
            {
                std::unique_ptr<common::database::interfaces::DbInterface> uniqueConn(std::move(shared_connect).get());
                shared_connect.reset();
                used_pool->get().release_db_interface(std::move(uniqueConn));
            }
        }

    private:
        PatientsHandbook patients_;
        MedicamentsHandbook medicaments_;
        OrganizationsHandbook organizations_;
        DiseaseHandbook diseases_;
        std::optional<std::reference_wrapper<common::database::creational::DbInterfacePool>> used_pool;
        std::shared_ptr<common::database::interfaces::DbInterface> shared_connect;
    };
}
