#pragma once


#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
    class Medicament final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            /// @typedef string
            static constexpr auto type = "type";
            /// @typedef bool
            static constexpr auto requires_prescription = "prescription";
            /// @typedef int32_t
            static constexpr auto manufacturer_id = "manufacturer_id";
        };

        // Конструктор по умолчанию
        Medicament() = default;

        // Конструктор с параметрами
        Medicament(const int32_t id, std::string name, std::string type, const bool requires_prescription,
                   const int32_t manufacturer_id)
            : id_(id), name_(std::move(name)), type_(std::move(type)), requires_prescription_(requires_prescription),
              manufacturer_id_(manufacturer_id)
        {
        }

        ~Medicament() override = default;

        // Методы для получения и установки данных медикамента
        [[nodiscard]] const std::string& get_name() const
        {
            return name_;
        }

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        [[nodiscard]] int32_t id() const
        {
            return id_;
        }

        void set_id(const int32_t id)
        {
            id_ = id;
        }

        [[nodiscard]] const std::string& get_type() const
        {
            return type_;
        }

        void set_type(const std::string& type)
        {
            type_ = type;
        }

        [[nodiscard]] bool is_requires_prescription() const
        {
            return requires_prescription_;
        }

        void set_requires_prescription(const bool requires_prescription)
        {
            requires_prescription_ = requires_prescription;
        }

        [[nodiscard]] int32_t get_manufacturer_id() const
        {
            return manufacturer_id_;
        }

        void set_manufacturer_id(const int32_t manufacturer_id)
        {
            manufacturer_id_ = manufacturer_id;
        }

        // Метод для преобразования медикамента в запись (Record)
        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<int32_t>>(field_name::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::type, type_));
            record.push_back(
                std::make_unique<common::database::Field<bool>>(field_name::requires_prescription,
                                                                requires_prescription_));
            record.push_back(
                std::make_unique<common::database::Field<int32_t>>(field_name::manufacturer_id, manufacturer_id_));
            record.push_back(collection_.make_properties_field());
            return record;
        }

        // Метод для обновления данных медикамента на основе записи
        void from_record(const common::database::Record& record) override
        {
            for (const auto& field : record.fields())
            {
                if (const auto& field_name = field->get_name();
                    field_name == field_name::id)
                {
                    id_ = field->as<int32_t>();
                }
                else if (field_name == field_name::name)
                {
                    name_ = field->as<std::string>();
                }
                else if (field_name == field_name::type)
                {
                    type_ = field->as<std::string>();
                }
                else if (field_name == field_name::requires_prescription)
                {
                    requires_prescription_ = field->as<bool>();
                }
                else if (field_name == field_name::manufacturer_id)
                {
                    manufacturer_id_ = field->as<int32_t>();
                }
                else if (field_name == field_name::properties)
                {
                    create_collection(field);
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        void from_record(const std::unique_ptr<common::database::ViewRecord>& viewed) override
        {
            for (std::size_t i = 0; i < viewed->size(); i++)
            {
                if (const auto& field_name = viewed->name(i); field_name == field_name::id)
                {
                    id_ = std::stoi(viewed->extract(i));
                }
                else if (field_name == field_name::name)
                {
                    name_ = viewed->extract(i);
                }
                else if (field_name == field_name::type)
                {
                    type_ = viewed->extract(i);
                }
                else if (field_name == field_name::requires_prescription)
                {
                    requires_prescription_ = viewed->view(i) == "true";
                }
                else if (field_name == field_name::manufacturer_id)
                {
                    manufacturer_id_ = std::stoi(viewed->extract(i));
                }
                else if (field_name == field_name::properties)
                {
                    create_collection(viewed->extract(i));
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        Json::Value to_json() override
        {
            Json::Value result;
            result[field_name::id] = id_;
            result[field_name::name] = name_;
            result[field_name::type] = type_;
            result[field_name::requires_prescription] = requires_prescription_;
            result[field_name::manufacturer_id] = manufacturer_id_;
            result[field_name::properties] = collection_.make_properties_field()->value();
            return result;
        }

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string type_;
        bool requires_prescription_ = false;
        int32_t manufacturer_id_ = -1;
    };
}
