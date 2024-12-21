#pragma once

#include <chrono>
#include <iomanip>
#include <memory>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <json/json.h>

namespace drug_lib::common::database
{
	template <typename T>
	class Field;
	/// @brief Base class representing a field in the database
	enum class SqlType
	{
		INT,
		UUID,
		BIGINT,
		DOUBLE_PRECISION,
		TEXT,
		JSONB,
		BOOLEAN,
		TIMESTAMP,
		UNSUPPORTED
	};

	class Uuid
	{
	public:
		static constexpr char default_value[] = "default";

		// Default constructor
		Uuid() = default;

		// Constructor from std::string
		explicit Uuid(std::string value)
		{
			uuid_ = std::move(value);
		}

		explicit operator std::string() const
		{
			return uuid_;
		}

		explicit operator std::string()
		{
			return uuid_;
		}

		// Equality operators
		bool operator==(const Uuid &other) const
		{
			return uuid_ == other.uuid_;
		}

		bool operator!=(const Uuid &other) const
		{
			return uuid_ != other.uuid_;
		}

		[[nodiscard]] const std::string &get_id() const
		{
			return uuid_;
		}

		void set_id(std::string uuid)
		{
			uuid_ = std::move(uuid);
		}

		Uuid(const Uuid &other) = default;

		Uuid(Uuid &&other) noexcept
			: uuid_(std::move(other.uuid_))
		{
		}

		Uuid &operator=(const Uuid &other)
		{
			if (this == &other)
				return *this;
			uuid_ = other.uuid_;
			return *this;
		}

		Uuid &operator=(std::string other)
		{
			uuid_ = std::move(other);
			return *this;
		}

		Uuid &operator=(Uuid &&other) noexcept
		{
			if (this == &other)
				return *this;
			uuid_ = std::move(other.uuid_);
			return *this;
		}

		friend bool operator<(const Uuid &lhs, const Uuid &rhs)
		{
			return lhs.uuid_ < rhs.uuid_;
		}

		friend bool operator<=(const Uuid &lhs, const Uuid &rhs)
		{
			return rhs >= lhs;
		}

		friend bool operator>(const Uuid &lhs, const Uuid &rhs)
		{
			return rhs < lhs;
		}

		friend bool operator>=(const Uuid &lhs, const Uuid &rhs)
		{
			return !(lhs < rhs);
		}

		friend std::ostream &operator<<(std::ostream &os, const Uuid &obj)
		{
			return os << obj.uuid_;
		}

	private:
		std::string uuid_ = default_value;

		// Validate UUID format (basic example, can be extended)
		static bool is_valid_uuid(const std::string &value)
		{
			const std::regex uuid_regex(
				R"([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12})");
			return std::regex_match(value, uuid_regex);
		}
	};

	class FieldBase
	{
	public:
		virtual ~FieldBase() = default;

		/// @return Column name of the field
		[[nodiscard]] virtual const std::string &get_name() const = 0;

		/// @brief Converts the field value to a string for SQL queries
		[[nodiscard]] virtual std::string to_string() const & = 0;

		/// @brief Converts the field value to a string for SQL queries
		/// @return moveable
		[[nodiscard]] virtual std::string to_string() && = 0;

		/// @brief Gets the SQL data type of the field
		[[nodiscard]] virtual SqlType get_sql_type() const = 0;

		/// @brief Gets the SQL data type of the field for creating
		[[nodiscard]] virtual std::string get_sql_type_initialization() const = 0;

		template <typename T>
		T as() const
		{
			// Ensure that T is a valid type, for example, int, std::string, etc.
			static_assert(std::is_default_constructible_v<T>, "T must be default constructible");

			// Attempt to dynamic cast this object to a Field<T> type

			if (const auto *derived_field = dynamic_cast<const Field<T> *>(this))
			{
				return derived_field->value();
			}
			throw std::runtime_error("FieldBase::as(): Incorrect type requested for field " + get_name());
		}

		[[nodiscard]] virtual std::unique_ptr<FieldBase> clone() const = 0;
	};

	/// @brief Represents a field of a specific type in the database
	/// @tparam T Type of the value in the database
	template <typename T>
	class Field final : public FieldBase
	{
	public:
		Field(std::string name, T value)
			: name_(std::move(name)),
			  value_(std::move(value))
		{
		}

		/// @return Column name of the field
		[[nodiscard]] const std::string &get_name() const override
		{
			return name_;
		}

		void set_name(const std::string &name)
		{
			name_ = name;
		}

		void set_name(std::string &&name)
		{
			name_ = std::move(name);
		}

		/// @return Value of the field
		const T &value() const
		{
			return value_;
		}

		void set_value(const T &value)
		{
			value_ = value;
		}

		void set_value(T &&value)
		{
			value_ = std::move(value);
		}

		/// @brief Converts the field value to a string for SQL queries
		[[nodiscard]] std::string to_string() const & override
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				return value_;
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				return value_ ? "TRUE" : "FALSE";
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return std::to_string(value_);
			}
			else if constexpr (std::is_same_v<T, Uuid>)
			{
				return value_.get_id();
			}
			else if constexpr (std::is_same_v<T, Json::Value>)
			{
				return value_.toStyledString();
			}
			else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
			{
				// Convert time_point to string in ISO 8601 format
				const std::time_t time = std::chrono::system_clock::to_time_t(value_);
				const std::tm *tm_ptr = std::localtime(&time);
				std::ostringstream oss;
				oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
				return oss.str();
			}
			else
			{
				static_assert(sizeof(T) == 0, "Unsupported field type for to_string()");
			}
			return {};
		}

		/// @brief Converts the field value to a string for SQL queries
		[[nodiscard]] std::string to_string() && override
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				return std::move(value_); // Move the string instead of copying
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				return value_ ? "TRUE" : "FALSE";
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return std::to_string(value_); // No need to move arithmetic types
			}
			else if constexpr (std::is_same_v<T, Uuid>)
			{
				return value_.get_id();
			}
			else if constexpr (std::is_same_v<T, Json::Value>)
			{
				return value_.toStyledString(); // Move the JSON string representation
			}
			else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
			{
				// Convert time_point to string in ISO 8601 format
				const std::time_t time = std::chrono::system_clock::to_time_t(value_);
				const std::tm *tm_ptr = std::localtime(&time);
				std::ostringstream oss;
				oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
				return oss.str(); // No need to move, it's a local object
			}
			else
			{
				static_assert(sizeof(T) == 0, "Unsupported field type for to_string()");
			}
			return {};
		}

		/// @brief Gets the SQL data type of the field
		[[nodiscard]] SqlType get_sql_type() const override
		{
			if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>)
			{
				return SqlType::INT;
			}
			else if constexpr (std::is_same_v<T, Uuid>)
			{
				return SqlType::UUID;
			}
			else if constexpr (std::is_same_v<T, int64_t>)
			{
				return SqlType::BIGINT;
			}
			else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
			{
				return SqlType::DOUBLE_PRECISION;
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				return SqlType::TEXT;
			}
			else if constexpr (std::is_same_v<T, Json::Value>)
			{
				return SqlType::JSONB;
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				return SqlType::BOOLEAN;
			}
			else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
			{
				return SqlType::TIMESTAMP;
			}
			else
			{
				static_assert(sizeof(T) == 0, "Unsupported field type for get_sql_type()");
				return SqlType::UNSUPPORTED; // Fallback for unsupported types
			}
		}

		/// @brief Gets the SQL data type of the field
		[[nodiscard]] std::string get_sql_type_initialization() const override
		{
			if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>)
			{
				return "INT";
			}
			else if constexpr (std::is_same_v<T, Uuid>)
			{
				return "UUID DEFAULT gen_random_uuid() PRIMARY KEY";
			}
			else if constexpr (std::is_same_v<T, int64_t>)
			{
				return "BIGINT";
			}
			else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
			{
				return "DOUBLE PRECISION";
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				return "TEXT";
			}
			else if constexpr (std::is_same_v<T, Json::Value>)
			{
				return "JSONB";
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				return "BOOLEAN";
			}
			else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
			{
				return "TIMESTAMP";
			}
			else
			{
				static_assert(sizeof(T) == 0, "Unsupported field type for get_sql_type()");
			}
			return {};
		}

		[[nodiscard]] std::unique_ptr<FieldBase> clone() const override
		{
			return std::make_unique<Field>(*this);
		}

	private:
		std::string name_;
		T value_;
	};

	template <typename T>
		requires std::default_initializable<T>
	std::shared_ptr<Field<T>> make_field_shared_by_default(std::string name)
	{
		return std::make_shared<Field<T>>(std::move(name), T());
	}

	template <typename T>
		requires std::default_initializable<T>
	std::unique_ptr<Field<T>> make_field_unique_by_default(std::string name)
	{
		return std::make_unique<Field<T>>(std::move(name), T());
	}

	class ViewingField final : public FieldBase
	{
	public:
		ViewingField(std::string &&name, std::string_view &&value)
			: name_(std::move(name)),
			  value_(value)
		{
		}

		/// @return Column name of the field
		[[nodiscard]] const std::string &get_name() const override
		{
			return name_;
		}

		void set_name(const std::string &name)
		{
			name_ = name;
		}

		void set_name(std::string &&name)
		{
			name_ = std::move(name);
		}

		/// @return Value of the field
		[[nodiscard]] std::string_view value() const
		{
			return value_;
		}

		void set_value(const std::string_view &value)
		{
			value_ = value;
		}

		void set_value(std::string_view &&value)
		{
			value_ = value;
		}

		/// @brief Converts the field value to a string for SQL queries
		[[nodiscard]] std::string to_string() const & override
		{
			return std::string(value_);
		}

		/// @brief Converts the field value to a string for SQL queries
		[[nodiscard]] std::string to_string() && override
		{
			return std::string(value_);
		}

		/// @brief Gets the SQL data type of the field
		[[nodiscard]] SqlType get_sql_type() const override
		{
			throw std::runtime_error("get_sql_type() called in VIEWING field");
		}

		[[nodiscard]] std::string get_sql_type_initialization() const override
		{
			throw std::runtime_error("get_sql_type_initialization() called in VIEWING field");
		}

		[[nodiscard]] std::unique_ptr<FieldBase> clone() const override
		{
			return std::make_unique<ViewingField>(*this);
		}

	private:
		std::string name_;
		std::string_view value_;
	};
}
