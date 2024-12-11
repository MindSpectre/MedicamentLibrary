#pragma once

#include "super_handbook.hpp"

namespace drug_lib::services
{
    template<typename T>
    concept SearchableType =
            std::is_same_v<T, data::objects::Medicament> || std::is_same_v<T, data::objects::Disease> ||
            std::is_same_v<T, data::objects::Organization> || std::is_same_v<T, data::objects::Patient>;


    class SearchServiceInternal
    {
    public:
        std::vector<std::unique_ptr<data::objects::ObjectBase> > search_through_all(const std::string &pattern);

        std::vector<std::unique_ptr<data::objects::ObjectBase> > open_search(const std::string &pattern);

        template<SearchableType T>
        std::size_t get_last_page_number()
        {
            if constexpr (std::is_same_v<T, data::objects::Medicament>)
            {
                return handbook_.medicaments().count_all() / page_limit_;
            } else if (std::is_same_v<T, data::objects::Disease>)
            {
                return handbook_.diseases().count_all() / page_limit_;
            } else if (std::is_same_v<T, data::objects::Organization>)
            {
                return handbook_.organizations().count_all() / page_limit_;
            } else if (std::is_same_v<T, data::objects::Patient>)
            {
                return handbook_.patients().count_all() / page_limit_;
            } else
            {
                throw std::invalid_argument("Invalid search type");
            }
        }


        std::vector<std::unique_ptr<data::objects::ObjectBase> > direct_search_medicaments(const std::string &pattern,
            std::size_t page_number = 1);

        std::vector<std::unique_ptr<data::objects::ObjectBase> > direct_search_diseases(const std::string &pattern,
            std::size_t page_number = 1);

        std::vector<std::unique_ptr<data::objects::ObjectBase> >
        direct_search_organizations(const std::string &pattern, std::size_t page_number = 1);

        std::vector<std::unique_ptr<data::objects::ObjectBase> > direct_search_patients(const std::string &pattern,
            std::size_t page_number = 1);

        std::vector<std::string> suggest(const std::string &pattern);

        void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
        {
            handbook_.direct_establish(connect);
        }

        void pool_setup(common::database::creational::DbInterfacePool &pool)
        {
            handbook_.establish_from_pool(pool);
        }

        explicit SearchServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
        {
            setup_from_one(connect);
        }

        SearchServiceInternal() = default;

    private:
        static uint32_t editor_distance(const std::string &suggest, const std::string &pattern);

        uint8_t page_limit_ = 100;
        uint8_t suggest_temperature_ = 12; // 0 - 100
        dao::SuperHandbook handbook_;
    };
} // namespace drug_lib::services
