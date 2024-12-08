#pragma once

#include <utility>

#include "super_handbook.hpp"

namespace drug_lib::services
{
	template <typename T> concept SearchableType = std::is_same_v<T, data::objects::Medicament> ||
	                                               std::is_same_v<T, data::objects::Disease> ||
	                                               std::is_same_v<T, data::objects::Organization> ||
	                                               	std::is_same_v<T, data::objects::Patient>;


	class LibrarianServiceInternal
	{
	public:
		// Medicament
		std::unique_ptr<data::objects::ObjectBase> get_medicament(common::database::Uuid id)
		{
			return std::make_unique<data::objects::Medicament>(handbook_.medicaments().get_by_id(std::move(id)));
		}

		void update_medicament(const data::objects::Medicament &element)
		{
			handbook_.medicaments().force_insert(element);
		}

		void add_medicament(data::objects::Medicament &element)
		{
			element.set_uuid( handbook_.medicaments().insert_without_id(element));
		}

		void remove_medicament(common::database::Uuid id)
		{
			handbook_.medicaments().remove_by_id(std::move(id));
		}

		// Disease
		std::unique_ptr<data::objects::ObjectBase> get_disease(common::database::Uuid id)
		{
			return std::make_unique<data::objects::Disease>(handbook_.diseases().get_by_id(std::move(id)));
		}

		void update_disease(const data::objects::Disease &element)
		{
			handbook_.diseases().force_insert(element);
		}

		void add_disease(data::objects::Disease &element)
		{
			element.set_uuid( handbook_.diseases().insert_without_id(element));
		}

		void remove_disease(common::database::Uuid id)
		{
			handbook_.diseases().remove_by_id(std::move(id));
		}

		// Organization
		std::unique_ptr<data::objects::ObjectBase> get_organization(common::database::Uuid id)
		{
			return std::make_unique<data::objects::Organization>(handbook_.organizations().get_by_id(std::move(id)));
		}

		void update_organization(const data::objects::Organization &element)
		{
			handbook_.organizations().force_insert(element);
		}

		void add_organization(data::objects::Organization &element)
		{
			element.set_uuid( handbook_.organizations().insert_without_id(element));
		}

		void remove_organization(common::database::Uuid id)
		{
			handbook_.organizations().remove_by_id(std::move(id));
		}

		// Patient
		std::unique_ptr<data::objects::ObjectBase> get_patient(common::database::Uuid id)
		{
			return std::make_unique<data::objects::Patient>(handbook_.patients().get_by_id(std::move(id)));
		}

		void update_patient(const data::objects::Patient &element)
		{
			handbook_.patients().force_insert(element);
		}

		void add_patient(data::objects::Patient &element)
		{
			element.set_uuid( handbook_.patients().insert_without_id(element));
		}

		void remove_patient(common::database::Uuid id)
		{
			handbook_.patients().remove_by_id(std::move(id));
		}

		void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			handbook_.direct_establish(connect);
		}

		void pool_setup(common::database::creational::DbInterfacePool &pool)
		{
			handbook_.establish_from_pool(pool);
		}

		explicit LibrarianServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			setup_from_one(connect);
		}

		LibrarianServiceInternal() = default;

	private:
		dao::SuperHandbook handbook_;
	};
} // namespace drug_lib::services
