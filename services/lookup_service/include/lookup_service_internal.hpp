#pragma once

#include "super_handbook.hpp"

namespace drug_lib::services
{
    class LookUpServiceInternal
    {
    public:
        std::vector<std::unique_ptr<dao::objects::ObjectBase>> search_through_all(const std::string& pattern);

        std::vector<std::unique_ptr<dao::objects::ObjectBase>> open_search(const std::string& pattern);

        template <typename T>
        std::vector<std::unique_ptr<data::objects::ObjectBase>> direct_search(
            const std::string& pattern)
        {
            if constexpr (std::is_same_v<T, data::objects::Medicament>)
            {
                std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
                if (std::vector<data::objects::Medicament> meds = handbook_.medicaments().search_paged(
                    pattern, this->page_limit_); meds.size() < 5)
                {
                    for (auto&& it : meds)
                    {
                        result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
                    }
                    for (auto&& it : handbook_.medicaments().fuzzy_search_paged(pattern, page_limit_ - meds.size()))
                    {
                        result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
                    }
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, data::objects::Disease>)
            {
                std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
                if (std::vector<data::objects::Disease> diseases = handbook_.diseases().search_paged(
                    pattern, this->page_limit_); diseases.size() < 5)
                {
                    for (auto&& it : diseases)
                    {
                        result.push_back(std::make_unique<data::objects::Disease>(std::move(std::move(it))));
                    }
                    for (auto&& it : handbook_.diseases().fuzzy_search_paged(pattern, page_limit_ - diseases.size()))
                    {
                        result.push_back(std::make_unique<data::objects::Disease>(std::move(std::move(it))));
                    }
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, data::objects::Organization>)
            {
                std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
                if (std::vector<data::objects::Organization> organizations = handbook_.organizations().search_paged(
                    pattern, this->page_limit_); organizations.size() < 5)
                {
                    for (auto&& it : organizations)
                    {
                        result.push_back(std::make_unique<data::objects::Organization>(std::move(it)));
                    }
                    for (auto&& it : handbook_.organizations().fuzzy_search_paged(
                             pattern, page_limit_ - organizations.size()))
                    {
                        result.push_back(std::make_unique<data::objects::Organization>(std::move(it)));
                    }
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, dao::objects::Patient>)
            {
                std::vector<std::unique_ptr<dao::objects::ObjectBase>> result;
                if (std::vector<data::objects::Patient> patients = handbook_.patients().search_paged(
                    pattern, this->page_limit_); patients.size() < 5)
                {
                    for (auto&& it : patients)
                    {
                        result.push_back(std::make_unique<data::objects::Patient>(std::move(it)));
                    }
                    for (auto&& it : handbook_.patients().fuzzy_search_paged(pattern, page_limit_ - patients.size()))
                    {
                        result.push_back(std::make_unique<data::objects::Patient>(std::move(it)));
                    }
                }
                return result;
            }
            else
            {
                throw std::invalid_argument("Unknown search type");
            }
        }

        std::vector<std::string> suggest(const std::string& pattern);

        void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            handbook_.direct_establish(connect);
        }

        void pool_setup(common::database::creational::DbInterfacePool& pool)
        {
            handbook_.establish_from_pool(pool);
        }

        explicit LookUpServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface>& connect)
        {
            setup_from_one(connect);
        }

        LookUpServiceInternal() = default;

    private:
        static uint32_t editor_distance(const std::string& suggest, const std::string& pattern);
        uint8_t page_limit_ = 10;
        uint8_t suggest_temperature_ = 12; // 0 - 100
        dao::SuperHandbook handbook_;
    };
}
