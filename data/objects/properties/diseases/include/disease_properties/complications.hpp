#pragma once

#include <ostream>
#include "data_property.hpp"
#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
	class Complications final : public ArrayProperty<std::string>
	{
	public:
		Complications() = default;

		explicit Complications(const Json::Value &properties)
		{
			this->set_info(properties);
		}

		explicit Complications(std::vector<std::string> complications)
			: ArrayProperty(std::move(complications))
		{
		}

		~Complications() override = default;

		[[nodiscard]] std::string get_name() const override
		{
			return properties::complications;
		}

		[[nodiscard]] Json::Value get_info() const override
		{
			Json::Value result(Json::arrayValue);
			for (const auto &ids: data_)
			{
				result.append(ids);
			}
			return result;
		}

		void set_info(const Json::Value &property) override
		{
			if (!property.isArray())
			{
				throw std::invalid_argument("Property is not an array");
			}
			data_.reserve(property.size());
			for (const auto &it: property)
			{
				data_.push_back(it.asString());
			}
		}
	};
}
