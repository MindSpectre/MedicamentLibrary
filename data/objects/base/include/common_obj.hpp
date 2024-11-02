#pragma once

#include <data_property.hpp>

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

            struct _cm_fields
            {
                static constexpr std::string id = "id";
                static constexpr std::string name = "name";
                static constexpr std::string properties = "properties";
            };
        };
    }

    [[nodiscard]] inline std::unique_ptr<common::database::Field<Json::Value>>
    PropertyCollection::make_properties_field() const noexcept
    {
        Json::Value result;
        for (const auto& [name, property] : m_data)
        {
            if (property->enabled())
            {
                result[name] = property->get_info();
            }
        }
        return std::make_unique<common::database::Field<Json::Value>>(
            objects::ObjectBase::_cm_fields::properties, result);
    }
}
