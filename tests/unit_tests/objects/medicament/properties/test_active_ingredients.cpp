#pragma once
#include <gtest/gtest.h>

#include "medicament_properties/active_ingredients.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::medicaments;

TEST(ActiveIngredientsTest, DefaultConstructor)
{
    const ActiveIngredients active_ingredients;
    EXPECT_EQ(active_ingredients.get_data().size(), 0);
}

TEST(ActiveIngredientsTest, ParameterizedConstructor)
{
    const std::vector active_ingredients_list = {
        ActiveIngredient("Paracetamol", 50),
        ActiveIngredient("Caffeine", 5)
    };
    const ActiveIngredients active_ingredients(active_ingredients_list);
    EXPECT_EQ(active_ingredients.get_data().size(), 2);
    EXPECT_EQ(active_ingredients.get_data()[0].get_name(), "Paracetamol");
    EXPECT_EQ(active_ingredients.get_data()[0].get_risk_level(), 50);
    EXPECT_EQ(active_ingredients.get_data()[1].get_name(), "Caffeine");
    EXPECT_EQ(active_ingredients.get_data()[1].get_risk_level(), 5);
}

TEST(ActiveIngredientsTest, SetActiveIngredients)
{
    ActiveIngredients active_ingredients;
    const std::vector active_ingredients_list = {
        ActiveIngredient("Ibuprofen", 40),
    };
    active_ingredients.set_data(active_ingredients_list);
    EXPECT_EQ(active_ingredients.get_data().size(), 1);
    EXPECT_EQ(active_ingredients.get_data()[0].get_name(), "Ibuprofen");
    EXPECT_EQ(active_ingredients.get_data()[0].get_risk_level(), 40);
}

TEST(ActiveIngredientsTest, EmplaceBack)
{
    ActiveIngredients active_ingredients;
    active_ingredients.emplace_back("Amoxicillin", 25);
    EXPECT_EQ(active_ingredients.get_data().size(), 1);
    EXPECT_EQ(active_ingredients.get_data()[0].get_name(), "Amoxicillin");
    EXPECT_EQ(active_ingredients.get_data()[0].get_risk_level(), 25);
}

TEST(ActiveIngredientsTest, GetInfo)
{
    const std::vector active_ingredients_list = {
        ActiveIngredient("Metformin", 50),
        ActiveIngredient("Sitagliptin", 5)
    };
    const ActiveIngredients active_ingredients(active_ingredients_list);

    Json::Value info = active_ingredients.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0][ActiveIngredient::names_of_json_fields::name].asString(), "Metformin");
    EXPECT_EQ(info[0][ActiveIngredient::names_of_json_fields::risk_level].asInt(), 50);
    EXPECT_EQ(info[1][ActiveIngredient::names_of_json_fields::name].asString(), "Sitagliptin");
    EXPECT_EQ(info[1][ActiveIngredient::names_of_json_fields::risk_level].asInt(), 5);
}

TEST(ActiveIngredientsTest, SetInfo)
{
    Json::Value active_ingredients_json(Json::arrayValue);
    Json::Value in_ingredient;
    in_ingredient[ActiveIngredient::names_of_json_fields::name] = "Metoprolol";
    in_ingredient[ActiveIngredient::names_of_json_fields::risk_level] = 10;
    active_ingredients_json.append(in_ingredient);

    ActiveIngredients active_ingredients;
    active_ingredients.set_info(active_ingredients_json);

    EXPECT_EQ(active_ingredients.get_data().size(), 1);
    EXPECT_EQ(active_ingredients.get_data()[0].get_name(), "Metoprolol");
    EXPECT_EQ(active_ingredients.get_data()[0].get_risk_level(), 10);
}

TEST(ActiveIngredientsTest, GetName)
{
    const ActiveIngredients strength;
    EXPECT_EQ(strength.get_name(), properties::active_ingredients);
}
