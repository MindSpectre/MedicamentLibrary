#pragma once
#include "data_property.hpp"

namespace drug_lib::data::objects::diseases
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr auto symptoms = "symptoms";
        static constexpr auto curative_drugs = "curative_drugs";
        static constexpr auto risk_factors = "risk_factors";
        static constexpr auto complications = "complications";
        static constexpr auto affected_age_groups = "affected_age_groups";
    };
}
