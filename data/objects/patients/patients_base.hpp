#pragma once

#include "db_field.hpp"
#include "db_conditions.hpp"
#include "db_record.hpp"

namespace drug_lib::data::objects
{
    class PatientBase 
    {
    public:
        PatientBase() = default;

        // Конструктор с параметрами
        PatientBase(std::string name, std::string surname)
            : name_(std::move(name)), surname_(std::move(surname)) {}

        virtual ~PatientBase() = default;
    private:
        std::string name_;
        std::string surname_;
    };
}