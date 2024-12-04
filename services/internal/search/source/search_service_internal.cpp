#include "search_service_internal.hpp"

namespace drug_lib::services
{
    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::search_through_all(const std::string& pattern)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        std::vector<data::objects::Medicament> meds = handbook_.medicaments().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Disease> diseases = handbook_.diseases().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Organization> organizations =
            handbook_.organizations().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Patient> patients = handbook_.patients().search_paged(pattern, this->page_limit_);
        result.reserve(4 * this->page_limit_);
        for (auto&& it : meds)
        {
            result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
        }
        for (auto&& it : diseases)
        {
            result.push_back(std::make_unique<data::objects::Disease>(std::move(it)));
        }
        for (auto&& it : organizations)
        {
            result.push_back(std::make_unique<data::objects::Organization>(std::move(it)));
        }
        for (auto&& it : patients)
        {
            result.push_back(std::make_unique<data::objects::Patient>(std::move(it)));
        }
        return result;
    }

    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::open_search(const std::string& pattern)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        std::vector<data::objects::Medicament> meds =
            handbook_.medicaments().fuzzy_search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Disease> diseases =
            handbook_.diseases().fuzzy_search_paged(pattern, this->page_limit_);
        result.reserve(4 * this->page_limit_);
        for (auto&& it : meds)
        {
            result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
        }
        for (auto&& it : diseases)
        {
            result.push_back(std::make_unique<data::objects::Disease>(std::move(it)));
        }
        return result;
    }
    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::direct_search_medicaments(const std::string& pattern, const std::size_t page_number)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        if (std::vector<data::objects::Medicament> meds =
                handbook_.medicaments().search_paged(pattern, this->page_limit_);
            meds.size() < 5)
        {
            for (auto&& it : meds)
            {
                result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
            }
            for (auto&& it :
                 handbook_.medicaments().fuzzy_search_paged(pattern, page_limit_ - meds.size(), page_number))
            {
                result.push_back(std::make_unique<data::objects::Medicament>(std::move(it)));
            }
        }
        return result;
    }
    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::direct_search_diseases(const std::string& pattern, const std::size_t page_number)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        if (std::vector<data::objects::Disease> diseases =
                handbook_.diseases().search_paged(pattern, this->page_limit_);
            diseases.size() < 5)
        {
            for (auto&& it : diseases)
            {
                result.push_back(std::make_unique<data::objects::Disease>(std::move(it)));
            }
            for (auto&& it :
                 handbook_.diseases().fuzzy_search_paged(pattern, page_limit_ - diseases.size(), page_number))
            {
                result.push_back(std::make_unique<data::objects::Disease>(std::move(it)));
            }
        }
        else
        {
            for (auto&& it : diseases)
            {
                result.push_back(std::make_unique<data::objects::Disease>(std::move(it)));
            }
        }
        return result;
    }
    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::direct_search_organizations(const std::string& pattern, const std::size_t page_number)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        if (std::vector<data::objects::Organization> organizations =
                handbook_.organizations().search_paged(pattern, this->page_limit_);
            organizations.size() < 5)
        {
            for (auto&& it : organizations)
            {
                result.push_back(std::make_unique<data::objects::Organization>(std::move(it)));
            }
            for (auto&& it :
                 handbook_.organizations().fuzzy_search_paged(pattern, page_limit_ - organizations.size(), page_number))
            {
                result.push_back(std::make_unique<data::objects::Organization>(std::move(it)));
            }
        }
        return result;
    }

    std::vector<std::unique_ptr<data::objects::ObjectBase>>
    SearchServiceInternal::direct_search_patients(const std::string& pattern, const std::size_t page_number)
    {
        std::vector<std::unique_ptr<data::objects::ObjectBase>> result;
        if (std::vector<data::objects::Patient> patients =
                handbook_.patients().search_paged(pattern, this->page_limit_);
            patients.size() < 5)
        {
            for (auto&& it : patients)
            {
                result.push_back(std::make_unique<data::objects::Patient>(std::move(it)));
            }
            for (auto&& it :
                 handbook_.patients().fuzzy_search_paged(pattern, page_limit_ - patients.size(), page_number))
            {
                result.push_back(std::make_unique<data::objects::Patient>(std::move(it)));
            }
        }
        return result;
    }
    std::vector<std::string> SearchServiceInternal::suggest(const std::string& pattern)
    {
        this->suggest_temperature_ = 0;
        throw std::logic_error("not implemented");
    }


    uint32_t SearchServiceInternal::editor_distance(const std::string& suggest, const std::string& pattern)
    {
        // Get lengths of both strings
        const std::size_t m = suggest.size();
        const std::size_t n = pattern.size();

        // Create a 2D DP table
        std::vector dp(m + 1, std::vector<uint32_t>(n + 1));
        for (size_t i = 0; i <= m; ++i)
        {
            for (size_t j = 0; j <= n; ++j)
            {
                if (i == 0)
                {
                    dp[i][j] = j;
                }
                else if (j == 0)
                {
                    dp[i][j] = i;
                }
                else if (suggest[i - 1] == pattern[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = 1 + std::min(std::min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
                }
            }
        }
        return dp[m][n];
    }

} // namespace drug_lib::services