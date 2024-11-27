#include "treatment_manager_service_internal.hpp"

#include <algorithm>


void drug_lib::services::TreatmentManagerServiceInternal::assign_disease(
    const int32_t patient_id, const int32_t disease_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::shared_ptr<data::objects::patients::CurrentDiseases> cur_diseases = std::dynamic_pointer_cast<
        data::objects::patients::CurrentDiseases>(
        persona.get_property(data::objects::patients::properties::current_diseases));
    cur_diseases->push_back(disease_id);
    handbook_.patients().force_insert(persona);
}

void drug_lib::services::TreatmentManagerServiceInternal::assign_drug(const int32_t patient_id, const int32_t drug_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::shared_ptr<data::objects::patients::CurrentMedicaments> cur_drugs = std::dynamic_pointer_cast<
        data::objects::patients::CurrentMedicaments>(
        persona.get_property(data::objects::patients::properties::current_medicaments));
    cur_drugs->push_back(drug_id);
    handbook_.patients().force_insert(persona);
}

void drug_lib::services::TreatmentManagerServiceInternal::remove_disease(
    const int32_t patient_id, const int32_t disease_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::shared_ptr<data::objects::patients::CurrentDiseases> cur_diseases = std::dynamic_pointer_cast<
        data::objects::patients::CurrentDiseases>(
        persona.get_property(data::objects::patients::properties::current_diseases));
    std::vector<int32_t>& diseases = cur_diseases->get_data();
    std::erase_if(diseases, [&](const int32_t it) { return it == disease_id; });
    handbook_.patients().force_insert(persona);
}

void drug_lib::services::TreatmentManagerServiceInternal::remove_drug(const int32_t patient_id, const int32_t drug_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::shared_ptr<data::objects::patients::CurrentMedicaments> cur_drugs = std::dynamic_pointer_cast<
        data::objects::patients::CurrentMedicaments>(
        persona.get_property(data::objects::patients::properties::current_medicaments));
    std::vector<int32_t>& meds = cur_drugs->get_data();
    std::erase_if(meds, [&](const int32_t it) { return it == drug_id; });
    handbook_.patients().force_insert(persona);
}

void drug_lib::services::TreatmentManagerServiceInternal::cure_disease(const int32_t patient_id,
                                                                       const int32_t disease_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::shared_ptr<data::objects::patients::CurrentDiseases> cur_diseases = std::dynamic_pointer_cast<
        data::objects::patients::CurrentDiseases>(
        persona.get_property(data::objects::patients::properties::current_diseases));
    const std::shared_ptr<data::objects::patients::MedicalHistory> medical_history = std::dynamic_pointer_cast<
        data::objects::patients::MedicalHistory>(
        persona.get_property(data::objects::patients::properties::medical_history));
    std::vector<int32_t>& diseases = cur_diseases->get_data();
    std::erase_if(diseases, [&](const int32_t it) { return it == disease_id; });

    std::vector<data::objects::patients::HealthRecord>& health_records = medical_history->get_data();
    const auto it = std::ranges::find_if(health_records,
                                         [&](const data::objects::patients::HealthRecord& rec)
                                         {
                                             return rec.get_disease_id() == disease_id && rec.is_current();
                                         });
    if (it != health_records.end())
    {
        // Modify the found element in place
        const auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm{};
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&local_tm, &time); // Windows-specific
#else
        localtime_r(&time, &local_tm); // POSIX-specific
#endif

        const int32_t year = local_tm.tm_year + 1900; // tm_year is years since 1900
        const uint32_t month = local_tm.tm_mon + 1; // tm_mon is 0-based

        // Set the end date of the found record
        const std::chrono::year_month current_date = std::chrono::year{year} / std::chrono::month{month};
        it->set_end_date(current_date);
        it->set_current(false);
    }
    handbook_.patients().force_insert(persona);
}

std::vector<drug_lib::data::objects::Medicament> drug_lib::services::TreatmentManagerServiceInternal::current_drugs(
    const int32_t patient_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::vector medicament_indexes = std::dynamic_pointer_cast<
        data::objects::patients::CurrentMedicaments>(
        persona.get_property(data::objects::patients::properties::current_medicaments))->get_data();
    std::vector<data::objects::Medicament> result;
    result.reserve(medicament_indexes.size());
    for (const int32_t ids : medicament_indexes)
    {
        result.emplace_back(std::move(handbook_.medicaments().get_by_id(ids)));
    }
    return result;
}

std::vector<drug_lib::data::objects::Disease> drug_lib::services::TreatmentManagerServiceInternal::current_diseases(
    const int32_t patient_id)
{
    const data::objects::Patient persona = handbook_.patients().get_by_id(patient_id);
    const std::vector<int32_t>& disease_indexes = std::dynamic_pointer_cast<
        data::objects::patients::CurrentDiseases>(
        persona.get_property(data::objects::patients::properties::current_diseases))->get_data();
    std::vector<data::objects::Disease> result;
    result.reserve(disease_indexes.size());
    for (const int32_t ids : disease_indexes)
    {
        result.emplace_back(std::move(handbook_.diseases().get_by_id(ids)));
    }
    return result;
}

drug_lib::data::objects::Patient drug_lib::services::TreatmentManagerServiceInternal::patient_profile(
    const int32_t patient_id)
{
    return handbook_.patients().get_by_id(patient_id);
}

bool drug_lib::services::TreatmentManagerServiceInternal::is_dangerous(const int32_t patient_id)
{
    return handbook_.patients().get_by_id(patient_id).get_gender() == "Female";
}
