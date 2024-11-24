#pragma once
#include "data_property.hpp"

namespace drug_lib::data::objects::patients
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr auto current_diseases = "current_diseases";
        static constexpr auto current_medicaments = "current_medicaments";
        static constexpr auto allergies = "allergies";
        static constexpr auto blood_type = "blood_type";
        static constexpr auto insurance = "insurance";
        static constexpr auto medical_history = "medical_history";
        static constexpr auto vaccines = "vaccines";
    };
}
