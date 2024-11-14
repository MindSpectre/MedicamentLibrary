#pragma once

#include "data_property.hpp"
#include "db_record.hpp"

namespace drug_lib::data
{
    namespace objects
    {
        class ObjectBase
        {
        public:
            virtual ~ObjectBase() = default;
            [[nodiscard]] virtual common::database::Record to_record() const = 0;
            virtual void from_record(const common::database::Record& selected) = 0;
            virtual void from_record(const std::unique_ptr<common::database::ViewRecord>& viewed) = 0;

            struct common_fields_names
            {
                static constexpr auto id = "id";
                static constexpr auto name = "name";
                static constexpr auto properties = "properties";
            };
        };
    }

    [[nodiscard]] inline std::unique_ptr<common::database::Field<Json::Value>>
    PropertyCollection::make_properties_field() const noexcept
    {
        Json::Value result;
        for (const auto& [name, property] : m_data)
        {
            result[name] = property->get_info();
        }
        return std::make_unique<common::database::Field<Json::Value>>(
            objects::ObjectBase::common_fields_names::properties, result);
    }
}
