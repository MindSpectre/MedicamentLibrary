#pragma once

#include <ostream>

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class HealthRecord final
    {
    public:
        HealthRecord(common::database::Uuid disease_id, const std::chrono::year_month& start_date,
                     const std::chrono::year_month& end_date) :
            disease_id_(std::move(disease_id)),
            start_date_(start_date),
            end_date_(end_date)
        {
            current = false;
        }

        HealthRecord(common::database::Uuid disease_id, const std::chrono::year_month& start_date) :
            disease_id_(std::move(disease_id)),
            start_date_(start_date)
        {
            current = true;
        }

        HealthRecord() = default;
        ~HealthRecord() = default;

        [[nodiscard]] const common::database::Uuid& get_disease_id() const
        {
            return disease_id_;
        }

        void set_disease_id(common::database::Uuid disease_id)
        {
            disease_id_ = std::move(disease_id);
        }
        void set_disease_id(std::string disease_id)
        {
            disease_id_.set_id(std::move(disease_id));
        }
        [[nodiscard]] const std::chrono::year_month& get_start_date() const
        {
            return start_date_;
        }

        [[nodiscard]] std::string get_string_start_date() const
        {
            std::ostringstream oss;
            oss << static_cast<int32_t>(start_date_.year()) << '-' << std::setfill('0') << std::setw(2)
                << static_cast<uint32_t>(start_date_.month());
            return oss.str();;
        }

        void set_start_date(const std::chrono::year_month start_date)
        {
            start_date_ = start_date;
        }

        void set_start_date(const std::string& start_date)
        {
            std::istringstream iss(start_date);
            uint32_t month;
            char dash;

            if (int32_t year; iss >> year >> dash >> month && dash == '-' && month >= 1 && month <= 12)
            {
                start_date_ = std::chrono::year{year} / std::chrono::month{month};
            }
            else
                throw std::invalid_argument("Invalid date format. Expected 'YYYY-MM'.");
        }

        [[nodiscard]] const std::chrono::year_month& get_end_date() const
        {
            return end_date_;
        }

        [[nodiscard]] std::string get_string_end_date() const
        {
            std::ostringstream oss;
            oss << static_cast<int32_t>(end_date_.year()) << '-' << std::setfill('0') << std::setw(2)
                << static_cast<uint32_t>(end_date_.month());
            return oss.str();;
        }

        void set_end_date(const std::chrono::year_month end_date)
        {
            end_date_ = end_date;
        }

        void set_end_date(const std::string& end_date)
        {
            std::istringstream iss(end_date);
            uint32_t month;
            char dash;

            if (int32_t year; iss >> year >> dash >> month && dash == '-' && month >= 1 && month <= 12)
            {
                end_date_ = std::chrono::year{year} / std::chrono::month{month};
            }
            else
                throw std::invalid_argument("Invalid date format. Expected 'YYYY-MM'.");
        }

        [[nodiscard]] bool is_current() const
        {
            return current;
        }

        void set_current(const bool current)
        {
            this->current = current;
        }

        struct names_of_json_fields
        {
            static constexpr auto disease_id = "disease_id";
            static constexpr auto start_date = "start_date";
            static constexpr auto end_date = "end_date";
        };

    private:
        common::database::Uuid disease_id_;
        std::chrono::year_month start_date_{};
        std::chrono::year_month end_date_{};
        bool current = true;
    };

    class MedicalHistory final : public ArrayProperty<HealthRecord>
    {
    public:
        MedicalHistory() = default;

        explicit MedicalHistory(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit MedicalHistory(std::vector<HealthRecord> allergies) :
            ArrayProperty(std::move(allergies))
        {
        }

        ~MedicalHistory() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::medical_history;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : data_)
            {
                Json::Value med_record;
                med_record[HealthRecord::names_of_json_fields::disease_id] = ids.get_disease_id().get_id();
                med_record[HealthRecord::names_of_json_fields::start_date] = ids.get_string_start_date();
                if (ids.is_current())
                    med_record[HealthRecord::names_of_json_fields::end_date] = "N/A";
                else
                    med_record[HealthRecord::names_of_json_fields::end_date] = ids.get_string_end_date();
                result.append(med_record);
            }
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            if (!property.isArray())
            {
                throw std::invalid_argument("Property is not an array");
            }
            data_.reserve(property.size());
            for (const auto& it : property)
            {
                if (it.isObject())
                {
                    HealthRecord med_record;
                    med_record.set_disease_id(
                        it[HealthRecord::names_of_json_fields::disease_id].asString()
                    );
                    med_record.set_start_date(it[HealthRecord::names_of_json_fields::start_date].asString());
                    try
                    {
                        med_record.set_end_date(it[HealthRecord::names_of_json_fields::end_date].asString());
                    }
                    catch (const std::invalid_argument&)
                    {
                        med_record.set_current(true);
                    }

                    data_.push_back(med_record);
                }
            }
        }
    };
}
