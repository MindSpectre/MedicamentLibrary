#pragma once

#include "super_handbook.hpp"

namespace drug_lib::services
{
	template <typename T>
	concept SearchableType =
			std::is_same_v<T, data::objects::Medicament> || std::is_same_v<T, data::objects::Disease> ||
			std::is_same_v<T, data::objects::Organization> || std::is_same_v<T, data::objects::Patient>;

	class SearchResponse
	{
	public:
		enum MatchStatus
		{
			PERFECT_MATCH, PARTIAL_MATCH
		};

		struct Match
		{
			std::unique_ptr<data::objects::ObjectBase> object;
			MatchStatus status;
		};

		void add(std::unique_ptr<data::objects::ObjectBase> &&object, MatchStatus status = PARTIAL_MATCH)
		{
			results_.emplace_back(std::move(object), status);
		}

		template <SearchableType T>
		void add(T &&object, MatchStatus status = PARTIAL_MATCH)
		{
			results_.emplace_back(std::make_unique<T>(std::move(object)), status);
		}

		void pop()
		{
			results_.pop_back();
		}

		[[nodiscard]] const std::vector<Match> &get() const
		{
			return results_;
		}

		void add(std::vector<std::unique_ptr<data::objects::ObjectBase>> &&arr, MatchStatus status = PARTIAL_MATCH)
		{
			for (auto & i : arr)
			{
				results_.emplace_back(std::move(i), status);
			}
		}

		template <SearchableType T>
		void add(std::vector<T> &&arr, MatchStatus status = PARTIAL_MATCH)
		{
			for (std::size_t i = 0; i < arr.size(); ++i)
			{
				results_.emplace_back(std::make_unique<T>(std::move(arr[i])), status);
			}
		}

		[[nodiscard]] Json::Value to_json() const
		{
			Json::Value response(Json::arrayValue);
			for (const auto &object: results_)
			{
				const auto &[obj, match] = object;
				Json::Value match_object = obj->to_json();;
				switch (match)
				{
					case MatchStatus::PERFECT_MATCH:
						match_object["match"] = "PERFECT_MATCH";
					case MatchStatus::PARTIAL_MATCH:
						match_object["match"] = "PARTIAL_MATCH";
				}

				response.append(match_object);
			}
			return response;
		}

		[[nodiscard]] std::vector<Match>::const_iterator begin() const
		{
			return results_.cbegin();
		}

		[[nodiscard]] std::vector<Match>::const_iterator end() const
		{
			return results_.cend();
		}

	private:
		std::vector<Match> results_;
	};

	class SearchServiceInternal
	{
	public:
		SearchResponse search_through_all(const std::string &pattern);

		SearchResponse open_search(const std::string &pattern);

		template <SearchableType T>
		std::size_t get_last_page_number()
		{
			if constexpr (std::is_same_v<T, data::objects::Medicament>)
			{
				return handbook_.medicaments().count_all() / page_limit_;
			}
			else if (std::is_same_v<T, data::objects::Disease>)
			{
				return handbook_.diseases().count_all() / page_limit_;
			}
			else if (std::is_same_v<T, data::objects::Organization>)
			{
				return handbook_.organizations().count_all() / page_limit_;
			}
			else if (std::is_same_v<T, data::objects::Patient>)
			{
				return handbook_.patients().count_all() / page_limit_;
			}
			else
			{
				throw std::invalid_argument("Invalid search type");
			}
		}


		SearchResponse direct_search_medicaments(
			const std::string &pattern,
			std::size_t page_number = 1);

		SearchResponse direct_search_diseases(
			const std::string &pattern,
			std::size_t page_number = 1);

		SearchResponse direct_search_organizations(
			const std::string &pattern,
			std::size_t page_number = 1);

		SearchResponse direct_search_patients(
			const std::string &pattern,
			std::size_t page_number = 1);

		std::vector<std::string> suggest(const std::string &pattern);

		void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			handbook_.direct_establish(connect);
		}

		void pool_setup(common::database::creational::DbInterfacePool &pool)
		{
			handbook_.establish_from_pool(pool);
		}

		explicit SearchServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			setup_from_one(connect);
		}

		SearchServiceInternal() = default;

	private:
		static uint32_t editor_distance(const std::string &suggest, const std::string &pattern);

		uint8_t page_limit_ = 10;
		uint8_t suggest_temperature_ = 12; // 0 - 100
		dao::SuperHandbook handbook_;
	};
} // namespace drug_lib::services
