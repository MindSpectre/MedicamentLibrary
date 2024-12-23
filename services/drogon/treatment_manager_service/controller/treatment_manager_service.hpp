
#pragma once

#include <drogon/HttpController.h>

namespace drug_lib::services::drogon {
	class TreatmentManager final : public ::drogon::HttpController<TreatmentManager> {
	public:
		METHOD_LIST_BEGIN
		METHOD_LIST_END
		static constexpr bool isAutoCreation = false;
		TreatmentManager() { LOG_INFO << "TreatmentManager service has been created"; }
		~TreatmentManager() override { LOG_INFO << "TreatmentManager service has been destroyed"; }
	};
} // namespace drug_lib::services::drogon
