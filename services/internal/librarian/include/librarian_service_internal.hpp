#pragma once

#include "super_handbook.hpp"

namespace drug_lib::services
{
    template<typename T>
    concept SearchableType =
            std::is_same_v<T, data::objects::Medicament> || std::is_same_v<T, data::objects::Disease> ||
            std::is_same_v<T, data::objects::Organization> || std::is_same_v<T, data::objects::Patient>;


    class LibrarianServiceInternal
    {
    public:
        // Medicament
        std::unique_ptr<data::objects::ObjectBase> get_medicament(const int32_t id)
        {
            return std::make_unique<data::objects::Medicament>
                (handbook_.medicaments().get_by_id(id));
        }

        void update_medicament(const data::objects::Medicament &element)
        {
            handbook_.medicaments().force_insert(element);
        }

        void add_medicament(const data::objects::Medicament &element)
        {
            handbook_.medicaments().insert(element);
        }

        void remove_medicament(const int32_t id)
        {
            handbook_.medicaments().remove_by_id(id);
        }

        // Disease
        std::unique_ptr<data::objects::ObjectBase> get_disease(const int32_t id)
        {
            return std::make_unique<data::objects::Disease>(handbook_.diseases().get_by_id(id));
        }

        void update_disease(const data::objects::Disease &element)
        {
            handbook_.diseases().force_insert(element);
        }

        void add_disease(const data::objects::Disease &element)
        {
            handbook_.diseases().insert(element);
        }

        void remove_disease(const int32_t id)
        {
            handbook_.diseases().remove_by_id(id);
        }

        // Organization
        std::unique_ptr<data::objects::ObjectBase> get_organization(const int32_t id)
        {
            return std::make_unique<data::objects::Organization>(handbook_.organizations().get_by_id(id));
        }

        void update_organization(const data::objects::Organization &element)
        {
            handbook_.organizations().force_insert(element);
        }

        void add_organization(const data::objects::Organization &element)
        {
            handbook_.organizations().insert(element);
        }

        void remove_organization(const int32_t id)
        {
            handbook_.organizations().remove_by_id(id);
        }

        // Patient
        std::unique_ptr<data::objects::ObjectBase> get_patient(const int32_t id)
        {
            return std::make_unique<data::objects::Patient>(handbook_.patients().get_by_id(id));
        }

        void update_patient(const data::objects::Patient &element)
        {
            handbook_.patients().force_insert(element);
        }

        void add_patient(const data::objects::Patient &element)
        {
            handbook_.patients().insert(element);
        }

        void remove_patient(const int32_t id)
        {
            handbook_.patients().remove_by_id(id);
        }
        void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
        {
            handbook_.direct_establish(connect);
        }

        void pool_setup(common::database::creational::DbInterfacePool &pool)
        {
            handbook_.establish_from_pool(pool);
        }
        explicit LibrarianServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
        {
            setup_from_one(connect);
        }

        LibrarianServiceInternal() = default;
    private:
        dao::SuperHandbook handbook_;
    };
} // namespace drug_lib::services
