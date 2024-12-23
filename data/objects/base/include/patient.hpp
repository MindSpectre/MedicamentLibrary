#pragma once

#include <utility>

#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
	namespace patient::field_name
	{
		static constexpr char name[] = "name";
		static constexpr char gender[] = "gender";
		static constexpr char birth_date[] = "birth_date";
		static constexpr char contact_information[] = "contact_information";
	};

	class Patient final : public ObjectBase, public PropertiesHolder
	{
	public:
		// ReSharper disable once CppInconsistentNaming


		Patient() = default;


		Patient(
			common::database::Uuid id, std::string name, std::string gender, std::chrono::year_month_day const birth_date,
			std::string contact_information)
			: ObjectBase(std::move(id)),
			  name_(std::move(name)),
			  gender_(std::move(gender)),
			  birth_date_(birth_date),
			  contact_information_(std::move(contact_information))
		{
		}

		~Patient() override = default;

		[[nodiscard]] common::database::Record to_record() const override
		{
			common::database::Record record;
			record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(shared::field_name::id, id_));
			record.push_back(std::make_unique<common::database::Field<std::string>>(patient::field_name::name, name_));
			record.push_back(std::make_unique<common::database::Field<std::string>>(patient::field_name::gender, gender_));
			record.push_back(
				std::make_unique<common::database::Field<std::chrono::system_clock::time_point>>(
					patient::field_name::birth_date, std::chrono::sys_days{birth_date_}));
			record.push_back(
				std::make_unique<common::database::Field<std::string>>(patient::field_name::contact_information,
				                                                       contact_information_));
			record.push_back(collection_.make_properties_field());
			return record;
		}

		void from_record(const common::database::Record &record) override
		{
			for (const auto &field: record.fields())
			{
				if (const auto &field_name = field->get_name();
					field_name == shared::field_name::id)
				{
					id_ = field->as<common::database::Uuid>();
				}
				else if (field_name == patient::field_name::name)
				{
					name_ = field->as<std::string>();
				}

				else if (field_name == patient::field_name::gender)
				{
					gender_ = field->as<std::string>();
				}
				else if (field_name == patient::field_name::birth_date)
				{
					auto time_point = field->as<std::chrono::system_clock::time_point>();
					birth_date_ = std::chrono::floor<std::chrono::days>(time_point);
				}
				else if (field_name == patient::field_name::contact_information)
				{
					contact_information_ = field->as<std::string>();
				}
				else if (field_name == shared::field_name::properties)
				{
					create_collection(field);
				}
				else
				{
					throw std::invalid_argument("Unknown field name: " + field_name);
				}
			}
		}

		void from_record(const std::unique_ptr<common::database::ViewRecord> &viewed) override
		{
			for (std::size_t i = 0; i < viewed->size(); i++)
			{
				if (const auto &field_name = viewed->name(i);
					field_name == shared::field_name::id)
				{
					id_ = viewed->extract(i);
				}
				else if (field_name == patient::field_name::name)
				{
					name_ = viewed->extract(i);
				}
				else if (field_name == patient::field_name::gender)
				{
					gender_ = viewed->extract(i);
				}
				else if (field_name == patient::field_name::birth_date)
				{
					std::istringstream iss(viewed->extract(i));
					std::tm tm = {};

					if (iss.fail())
					{
						throw std::runtime_error("Failed to parse date: " + viewed->extract(i));
					}
					auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
					birth_date_ = std::chrono::floor<std::chrono::days>(tp);
				}
				else if (field_name == patient::field_name::contact_information)
				{
					contact_information_ = viewed->extract(i);
				}
				else if (field_name == shared::field_name::properties)
				{
					create_collection(viewed->extract(i));
				}
				else
				{
					throw std::invalid_argument("Unknown field name: " + field_name);
				}
			}
		}

		[[nodiscard]] Json::Value to_json() const override
		{
			Json::Value result = ObjectBase::to_json();
			result[patient::field_name::name] = name_;
			result[patient::field_name::gender] = gender_;
			result[patient::field_name::birth_date]["year"] = static_cast<int>(birth_date_.year());
			result[patient::field_name::birth_date]["month"] = static_cast<uint>(birth_date_.month());
			result[patient::field_name::birth_date]["day"] = static_cast<uint>(birth_date_.day());
			result[shared::field_name::properties] = collection_.make_properties_field()->value();

			return result;
		}

		void from_json(const Json::Value &val) override
		{
			ObjectBase::from_json(val);
			name_ = val[patient::field_name::name].asString();
			gender_ = val[patient::field_name::gender].asString();

			if (const Json::Value &birth_date = val[patient::field_name::birth_date];
				birth_date.isMember("year") && birth_date.
				isMember("month") && birth_date.isMember("day"))
			{
				const int32_t year = birth_date["year"].asInt();
				const uint32_t month = birth_date["month"].asUInt();
				const uint32_t day = birth_date["day"].asUInt();
				birth_date_ = std::chrono::year_month_day{
					std::chrono::year{year},
					std::chrono::month{month},
					std::chrono::day{day}
				};
			}
			create_collection(val[shared::field_name::properties]);
		}

		[[nodiscard]] const std::string &get_name() const
		{
			return name_;
		}

		void set_name(const std::string &name)
		{
			name_ = name;
		}

		[[nodiscard]] const std::string &get_gender() const
		{
			return gender_;
		}

		void set_gender(const std::string &gender)
		{
			gender_ = gender;
		}

		[[nodiscard]] const std::chrono::year_month_day &get_birth_date() const
		{
			return birth_date_;
		}

		void set_birth_date(const std::chrono::year_month_day &birth_date)
		{
			birth_date_ = birth_date;
		}

		[[nodiscard]] const std::string &get_contact_information() const
		{
			return contact_information_;
		}

		void set_contact_information(const std::string &contact_information)
		{
			contact_information_ = contact_information;
		}

	private:
		std::string name_;
		std::string gender_;
		std::chrono::year_month_day birth_date_{};
		std::string contact_information_;
	};
}
