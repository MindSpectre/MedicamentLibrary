#pragma once

#include "common_obj.hpp"
#include "data_property.hpp"


namespace drug_lib::data::objects
{
    class Medicament final : public ObjectBase, public PropertiesHolder
    {
    public:
        // Конструктор по умолчанию
        Medicament() = default;

        // Конструктор с параметрами
        Medicament(const int32_t id, std::string name)
            : id_(id), name_(std::move(name))
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

        // Метод для преобразования медикамента в запись (Record)
        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.add_field(std::make_shared<common::database::Field<int32_t>>("id", id_));
            record.add_field(std::make_shared<common::database::Field<std::string>>("name", name_));
            record.add_field(collection_.make_properties_field());
            return record;
        }

        // Метод для обновления данных медикамента на основе записи
        void from_record(const common::database::Record& record) override
        {
            for (const auto& [field_name, field] : record.fields())
            {
                if (field_name == "id")
                {
                    id_ = field->as<int32_t>();
                }
                if (field_name == "name")
                {
                    name_ = field->as<std::string>();
                }
                if (field_name == properties::properties)
                {
                    create_collection(field);
                }
            }
        }

    private:
        int32_t id_ = -1;
        std::string name_;
    };
}
