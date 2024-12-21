#pragma once

#include <utility>

#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
	namespace medicament::field_name
	{
		constexpr char name[] = "name";
		constexpr char requires_prescription[] = "prescription";
		/// @typedef string
		constexpr char approval_number[] = "approval_number";
		/// @typedef string
		constexpr char approval_status[] = "approval_status";
		/// @typedef string
		constexpr char atc_code[] = "atc_code";
		constexpr char type[] = "type";
	};

	class Medicament final : public ObjectBase, public PropertiesHolder
	{
	public:
		// Конструктор по умолчанию
		Medicament() = default;

		// Конструктор с параметрами
		Medicament(
			common::database::Uuid id, std::string name, std::string type, const bool requires_prescription,
			std::string approval_number, std::string approval_status,
			std::string atc_code)
			: ObjectBase(std::move(id)),
			  name_(std::move(name)),
			  type_(std::move(type)),
			  requires_prescription_(requires_prescription),
			  approval_number_(std::move(approval_number)),
			  approval_status_(std::move(approval_status)),
			  atc_code_(std::move(atc_code))
		{
		}

		~Medicament() override = default;

		[[nodiscard]] const std::string &get_name() const
		{
			return name_;
		}

		void set_name(const std::string &name)
		{
			name_ = name;
		}

		[[nodiscard]] const std::string &get_type() const
		{
			return type_;
		}

		void set_type(const std::string &type)
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

		[[nodiscard]] const std::string &get_approval_number() const
		{
			return approval_number_;
		}

		void set_approval_number(const std::string &approval_number)
		{
			approval_number_ = approval_number;
		}

		[[nodiscard]] const std::string &get_approval_status() const
		{
			return approval_status_;
		}

		void set_approval_status(const std::string &approval_status)
		{
			approval_status_ = approval_status;
		}

		[[nodiscard]] const std::string &get_atc_code() const
		{
			return atc_code_;
		}

		void set_atc_code(const std::string &atc_code)
		{
			atc_code_ = atc_code;
		}

		// Метод для преобразования медикамента в запись (Record)
		[[nodiscard]] common::database::Record to_record() const override
		{
			common::database::Record record;
			record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(shared::field_name::id, id_));
			record.push_back(std::make_unique<common::database::Field<std::string>>(medicament::field_name::name, name_));
			record.push_back(std::make_unique<common::database::Field<std::string>>(medicament::field_name::type, type_));
			record.push_back(std::make_unique<common::database::Field<bool>>(
				medicament::field_name::requires_prescription,
				requires_prescription_));
			record.push_back(
				std::make_unique<common::database::Field<std::string>>(medicament::field_name::approval_status, approval_status_));
			record.push_back(
				std::make_unique<common::database::Field<std::string>>(medicament::field_name::approval_number, approval_number_));
			record.push_back(std::make_unique<common::database::Field<std::string>>(medicament::field_name::atc_code, atc_code_));
			record.push_back(collection_.make_properties_field());
			return record;
		}

		// Метод для обновления данных медикамента на основе записи
		void from_record(const common::database::Record &record) override
		{
			for (const auto &field: record.fields())
			{
				if (const auto &field_name = field->get_name();
					field_name == shared::field_name::id)
				{
					id_ = field->as<common::database::Uuid>();
				}
				else if (field_name == medicament::field_name::name)
				{
					name_ = field->as<std::string>();
				}
				else if (field_name == medicament::field_name::type)
				{
					type_ = field->as<std::string>();
				}
				else if (field_name == medicament::field_name::requires_prescription)
				{
					requires_prescription_ = field->as<bool>();
				}
				else if (field_name == medicament::field_name::approval_number)
				{
					approval_number_ = field->as<std::string>();
				}
				else if (field_name == medicament::field_name::approval_status)
				{
					approval_status_ = field->as<std::string>();
				}
				else if (field_name == medicament::field_name::atc_code)
				{
					atc_code_ = field->as<std::string>();
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
				else if (field_name == medicament::field_name::name)
				{
					name_ = viewed->extract(i);
				}
				else if (field_name == medicament::field_name::type)
				{
					type_ = viewed->extract(i);
				}
				else if (field_name == medicament::field_name::requires_prescription)
				{
					requires_prescription_ = viewed->view(i) == "t";
				}
				else if (field_name == medicament::field_name::approval_number)
				{
					approval_number_ = viewed->extract(i);
				}
				else if (field_name == medicament::field_name::approval_status)
				{
					approval_status_ = viewed->extract(i);
				}
				else if (field_name == medicament::field_name::atc_code)
				{
					atc_code_ = viewed->extract(i);
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
			result[medicament::field_name::name] = name_;
			result[medicament::field_name::type] = type_;
			result[medicament::field_name::requires_prescription] = requires_prescription_;
			result[medicament::field_name::approval_number] = approval_number_;
			result[medicament::field_name::approval_status] = approval_status_;
			result[medicament::field_name::atc_code] = atc_code_;
			result[shared::field_name::properties] = collection_.make_properties_field()->value();
			return result;
		}

		void from_json(const Json::Value &val) override
		{
			ObjectBase::from_json(val);
			name_ = val[medicament::field_name::name].asString();
			type_ = val[medicament::field_name::type].asString();
			try
			{
				requires_prescription_ = val[medicament::field_name::requires_prescription].asBool();
			}
			catch ([[maybe_unused]] const Json::LogicError &e)
			{
				std::string requires_prescription_string = val[medicament::field_name::requires_prescription].asString();
				std::transform(requires_prescription_string.begin(), requires_prescription_string.end(),
				               requires_prescription_string.begin(), ::tolower);
				if (requires_prescription_string != "true" && requires_prescription_string != "false")
				{
					throw;
				}
				requires_prescription_ = requires_prescription_string == "true";
			}
			approval_number_ = val[medicament::field_name::approval_number].asString();
			approval_status_ = val[medicament::field_name::approval_status].asString();
			atc_code_ = val[medicament::field_name::atc_code].asString();
			create_collection(val[shared::field_name::properties]);
		}

	private:
		std::string name_;
		std::string type_;
		bool requires_prescription_ = false;
		std::string approval_number_;
		std::string approval_status_;
		std::string atc_code_;
	};
}
