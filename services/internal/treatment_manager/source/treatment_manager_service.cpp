#include "treatment_manager_service.hpp"


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
