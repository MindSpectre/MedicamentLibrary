#include "db_interface_factory.hpp"
#include "super_handbook.hpp"
template <typename T>
concept Removable = requires(T table)
{
    { table.delete_table() };
};

template <Removable T>
void drop_obj(T& table_) { table_.delete_table(); }

template <typename... Args>
void drop(Args&... args)
{
    (drop_obj(args), ...);
}

int main()
{
    constexpr uint32_t port = 5432;
    constexpr auto host = "localhost";
    constexpr auto db_name = "test_db";
    constexpr auto username = "postgres";
    constexpr auto password = "postgres";
    const drug_lib::common::database::PqxxConnectParams connect_params{
        host, port, db_name, username, password
    };
    drug_lib::dao::MedicamentsHandbook medicaments_handbook{
        drug_lib::common::database::creational::DbInterfaceFactory::
        create_pqxx_client(connect_params)
    };
    drug_lib::dao::OrganizationsHandbook organizations_handbook{
        drug_lib::common::database::creational::DbInterfaceFactory::
        create_pqxx_client(connect_params)
    };
    drug_lib::dao::PatientsHandbook patients_handbook{
        drug_lib::common::database::creational::DbInterfaceFactory::
        create_pqxx_client(connect_params)
    };
    drug_lib::dao::DiseaseHandbook disease_handbook{
        drug_lib::common::database::creational::DbInterfaceFactory::
        create_pqxx_client(connect_params)
    };
    drop(medicaments_handbook, organizations_handbook, patients_handbook, disease_handbook);
}
