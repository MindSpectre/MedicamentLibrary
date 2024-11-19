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
        std::vector<std::unique_ptr<dao::objects::ObjectBase>> direct_search(const std::string& pattern);

        std::vector<std::string> suggest(const std::string& pattern);

    private:
        static uint32_t editor_distance(const std::string& suggest, const std::string& pattern);
        uint8_t page_limit_ = 10;
        dao::SuperHandbook handbook_;
    };
}
