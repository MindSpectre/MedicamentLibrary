#pragma once
#include "data_property.hpp"

namespace drug_lib::data::objects::medicaments
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr auto prescription = "prescription";
        static constexpr auto dosage_form = "dosage_form";
        static constexpr auto strength = "strength";
        static constexpr auto active_ingredients = "active_ingredients";
        static constexpr auto inactive_ingredients = "inactive_ingredients";
        static constexpr auto side_effects = "side_effects";
    };
}
