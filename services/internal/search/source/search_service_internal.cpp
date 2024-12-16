#include "search_service_internal.hpp"

namespace drug_lib::services
{
    SearchResponse
    SearchServiceInternal::search_through_all(const std::string& pattern)
    {
        SearchResponse result;
        std::vector<data::objects::Medicament> meds = handbook_.medicaments().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Disease> diseases = handbook_.diseases().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Organization> organizations =
            handbook_.organizations().search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Patient> patients = handbook_.patients().search_paged(pattern, this->page_limit_);
        result.add(std::move(patients), SearchResponse::PERFECT_MATCH);
        result.add(std::move(meds), SearchResponse::PERFECT_MATCH);
        result.add(std::move(diseases), SearchResponse::PERFECT_MATCH);
        result.add(std::move(organizations), SearchResponse::PERFECT_MATCH);
        return result;
    }

    SearchResponse
    SearchServiceInternal::open_search(const std::string& pattern)
    {
        SearchResponse result;
        std::vector<data::objects::Medicament> meds =
            handbook_.medicaments().fuzzy_search_paged(pattern, this->page_limit_);
        std::vector<data::objects::Disease> diseases =
            handbook_.diseases().fuzzy_search_paged(pattern, this->page_limit_);
        result.add(std::move(meds), SearchResponse::PARTIAL_MATCH);
        result.add(std::move(diseases), SearchResponse::PARTIAL_MATCH);
        return result;
    }

    SearchResponse SearchServiceInternal::direct_search_medicaments(const std::string &pattern, const std::size_t page_number)
    {
        std::cout << "SearchServiceInternal::direct_search_medicaments" << std::endl;
        SearchResponse result;
        std::vector<data::objects::Medicament> perfect_medicaments = handbook_.medicaments().search_paged(pattern, this->page_limit_, page_number);
        result.add(std::move(perfect_medicaments), SearchResponse::PERFECT_MATCH);
        if (result.get().size() < page_limit_)
        {
            std::vector<data::objects::Medicament> partial_medicaments =
                    handbook_.medicaments().fuzzy_search_paged(pattern, page_limit_, page_number);
            while (partial_medicaments.size() > result.get().size())
            {
                result.add(std::move(partial_medicaments.back()), SearchResponse::PARTIAL_MATCH);
                partial_medicaments.pop_back();
            }
            result.add(std::move(partial_medicaments), SearchResponse::PARTIAL_MATCH);
        }
        std::cerr << result.get().size() << std::endl;
        return result;
    }
    SearchResponse
    SearchServiceInternal::direct_search_diseases(const std::string& pattern, const std::size_t page_number)
    {
        SearchResponse result;
        std::vector<data::objects::Disease> perfect_diseases = handbook_.diseases().search_paged(pattern, this->page_limit_, page_number);
        result.add(std::move(perfect_diseases), SearchResponse::PERFECT_MATCH);
        if (result.get().size() < page_limit_)
        {
            std::vector<data::objects::Disease> partial_diseases =
                    handbook_.diseases().fuzzy_search_paged(pattern, page_limit_, page_number);
            while (partial_diseases.size() > result.get().size())
            {
                result.add(std::move(partial_diseases.back()), SearchResponse::PARTIAL_MATCH);
                partial_diseases.pop_back();
            }
            result.add(std::move(partial_diseases), SearchResponse::PARTIAL_MATCH);
        }
        return result;
    }
    SearchResponse
    SearchServiceInternal::direct_search_organizations(const std::string& pattern, const std::size_t page_number)
    {
        SearchResponse result;
        std::vector<data::objects::Organization> perfect_organizations = handbook_.organizations().search_paged(pattern, this->page_limit_, page_number);
        result.add(std::move(perfect_organizations), SearchResponse::PERFECT_MATCH);
        if (result.get().size() < page_limit_)
        {
            std::vector<data::objects::Organization> partial_organizations =
                    handbook_.organizations().fuzzy_search_paged(pattern, page_limit_, page_number);
            while (partial_organizations.size() > result.get().size())
            {
                result.add(std::move(partial_organizations.back()), SearchResponse::PARTIAL_MATCH);
                partial_organizations.pop_back();
            }
            result.add(std::move(partial_organizations), SearchResponse::PARTIAL_MATCH);
        }
        return result;
    }

    SearchResponse
    SearchServiceInternal::direct_search_patients(const std::string& pattern, const std::size_t page_number)
    {
        SearchResponse result;
        std::vector<data::objects::Patient> perfect_patients = handbook_.patients().search_paged(pattern, this->page_limit_, page_number);
        result.add(std::move(perfect_patients), SearchResponse::PERFECT_MATCH);
        if (result.get().size() < page_limit_)
        {
            std::vector<data::objects::Patient> partial_patients =
                    handbook_.patients().fuzzy_search_paged(pattern, page_limit_, page_number);
            while (partial_patients.size() > result.get().size())
            {
                result.add(std::move(partial_patients.back()), SearchResponse::PARTIAL_MATCH);
                partial_patients.pop_back();
            }
            result.add(std::move(partial_patients), SearchResponse::PARTIAL_MATCH);
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
