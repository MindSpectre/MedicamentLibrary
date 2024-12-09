#pragma once

#include <utility>

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

            explicit ObjectBase(common::database::Uuid id) :
                id_(std::move(id))
            {
            }

            ObjectBase() = default;

            explicit ObjectBase(const std::string & id): id_(id){}

            [[nodiscard]] virtual common::database::Record to_record() const = 0;
            virtual void from_record(const common::database::Record& selected) = 0;
            virtual void from_record(const std::unique_ptr<common::database::ViewRecord>& viewed) = 0;

            [[nodiscard]] virtual Json::Value to_json() const
            {
                Json::Value result;
                result[common_fields_names::id] = id_.get_id();
                return result;
            }
            virtual void from_json(const Json::Value& val)
            {
                if (val.isMember(common_fields_names::id))
                {
                    id_.set_id(val[common_fields_names::id].asString()) ;
                } else
                {
                    id_.set_id(common::database::Uuid::default_value);
                }
            }

            struct common_fields_names
            {
                static constexpr auto id = "id";
                static constexpr auto properties = "properties";
            };

            [[nodiscard]] const std::string& get_id() const
            {
                return id_.get_id();
            }
            [[nodiscard]] const common::database::Uuid& get_uuid() const
            {
                return id_;
            }
            void set_uuid(const common::database::Uuid& id)
            {
                id_ = id;
            }
            void set_id(const std::string& id)
            {
                id_ = id;
            }
        protected:
            common::database::Uuid id_;
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
