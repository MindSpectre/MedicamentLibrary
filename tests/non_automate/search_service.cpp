#include <db_interface_factory.hpp>

#include "search_service_internal.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "super_handbook.hpp"

void print_menu()
{
    std::cout << "\n--- Search Service Interactive Tester ---\n";
    std::cout << "1. Search Through All\n";
    std::cout << "2. Open Search\n";
    std::cout << "3. Direct Search (by Type)\n";
    std::cout << "4. Suggest\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

void print_results(const std::vector<std::unique_ptr<drug_lib::dao::objects::ObjectBase> > &results)
{
    if (results.empty())
    {
        std::cout << "No results found.\n";
        return;
    }
    for (const auto &obj: results)
    {
        std::cout << obj->to_json().toStyledString() << "\n"; // Assuming ObjectBase has a `to_string` method
    }
}

void print_suggestions(const std::vector<std::string> &suggestions)
{
    if (suggestions.empty())
    {
        std::cout << "No suggestions available.\n";
        return;
    }
    for (const auto &suggestion: suggestions)
    {
        std::cout << suggestion << "\n";
    }
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
    auto n_connect = drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client(connect_params);
    drug_lib::services::SearchServiceInternal search_service(std::move(n_connect));

    std::cout << "Initializing SearchServiceInternal...\n";

    bool running = true;
    while (running)
    {
        print_menu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        std::string pattern;
        switch (choice)
        {
            case 1: // Search Through All
                std::cout << "Enter search pattern: ";
                std::getline(std::cin, pattern); {
                    auto results = search_service.search_through_all(pattern);
                    print_results(results);
                }
                break;
            case 2: // Open Search
                std::cout << "Enter search pattern: ";
                std::getline(std::cin, pattern); {
                    auto results = search_service.open_search(pattern);
                    print_results(results);
                }
                break;
            case 3: // Direct Search (by Type)
                std::cout << "Enter search pattern: ";
                std::getline(std::cin, pattern); {
                    // Replace ` YourType` with the appropriate type you want to test
                    auto results = search_service.direct_search_diseases(pattern);
                    print_results(results);
                }
                break;
            case 4: // Suggest
                std::cout << "Enter search pattern for suggestions: ";
                std::getline(std::cin, pattern); {
                    auto suggestions = search_service.suggest(pattern);
                    print_suggestions(suggestions);
                }
                break;
            case 5: // Exit
                running = false;
                std::cout << "Exiting tester. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}
