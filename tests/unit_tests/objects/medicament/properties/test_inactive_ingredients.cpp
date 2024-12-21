#include <gtest/gtest.h>
#include <json/json.h>
#include "medicament_properties/inactive_ingredients.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::medicaments;
TEST(InactiveIngredientsTest, DefaultConstructor)
{
	const InactiveIngredients inactive_ingredients;
	EXPECT_EQ(inactive_ingredients.get_data().size(), 0);
}

TEST(InactiveIngredientsTest, ParameterizedConstructor)
{
	const std::vector inactive_ingredients_list = {
		InactiveIngredient("Sugar", 3),
		InactiveIngredient("E675", 5)
	};
	const InactiveIngredients inactive_ingredients(inactive_ingredients_list);
	EXPECT_EQ(inactive_ingredients.get_data().size(), 2);
	EXPECT_EQ(inactive_ingredients.get_data()[0].get_name(), "Sugar");
	EXPECT_EQ(inactive_ingredients.get_data()[1].get_name(), "E675");
}

TEST(InactiveIngredientsTest, SetInactiveIngredients)
{
	InactiveIngredients inactive_ingredients;
	const std::vector inactive_ingredients_list = {
		InactiveIngredient("Sugar", 3),
	};
	inactive_ingredients.set_data(inactive_ingredients_list);
	EXPECT_EQ(inactive_ingredients.get_data().size(), 1);
	EXPECT_EQ(inactive_ingredients.get_data()[0].get_name(), "Sugar");
}

TEST(InactiveIngredientsTest, EmplaceBack)
{
	InactiveIngredients inactive_ingredients;
	inactive_ingredients.emplace_back("Sugar", 3);
	EXPECT_EQ(inactive_ingredients.get_data().size(), 1);
	EXPECT_EQ(inactive_ingredients.get_data()[0].get_name(), "Sugar");
}

TEST(InactiveIngredientsTest, GetInfo)
{
	const std::vector inactive_ingredients_list = {
		InactiveIngredient("Sugar", 3),
		InactiveIngredient("E675", 5)
	};
	const InactiveIngredients inactive_ingredients(inactive_ingredients_list);

	Json::Value info = inactive_ingredients.get_info();
	ASSERT_TRUE(info.isArray());
	EXPECT_EQ(info.size(), 2);
	EXPECT_EQ(info[0][InactiveIngredient::names_of_json_fields::name].asString(), "Sugar");
	EXPECT_EQ(info[1][InactiveIngredient::names_of_json_fields::name].asString(), "E675");
}

TEST(InactiveIngredientsTest, SetInfo)
{
	Json::Value inactive_ingredients_json(Json::arrayValue);
	Json::Value in_ingredient;
	in_ingredient[InactiveIngredient::names_of_json_fields::name] = "Sugar";
	in_ingredient[InactiveIngredient::names_of_json_fields::risk_level] = 2;
	inactive_ingredients_json.append(in_ingredient);

	InactiveIngredients inactive_ingredients;
	inactive_ingredients.set_info(inactive_ingredients_json);

	EXPECT_EQ(inactive_ingredients.get_data().size(), 1);
	EXPECT_EQ(inactive_ingredients.get_data()[0].get_name(), "Sugar");
	EXPECT_EQ(inactive_ingredients.get_data()[0].get_risk_level(), 2);
}

TEST(InactiveIngredientsTest, GetName)
{
	const InactiveIngredients strength;
	EXPECT_EQ(strength.get_name(), properties::inactive_ingredients);
}
