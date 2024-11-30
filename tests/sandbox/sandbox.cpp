#include <iostream>

#include "db_field.hpp"
#include "stopwatch.hpp"
#include <json/json.h>
#include <chrono>
#include <drogon/HttpController.h>

class MyController : public drogon::HttpController<MyController>
{
public:
    METHOD_LIST_BEGIN
        // Bind your endpoint to a handler function
        ADD_METHOD_TO(MyController::helloWorld, "/hello", drogon::Get);
    METHOD_LIST_END

    // Handler function
    void helloWorld(const drogon::HttpRequestPtr& req,
                    std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        Json::Value message = "Hello, World!";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(message);
        callback(resp);
    }

};

int main()
{
    drogon::app().addListener("0.0.0.0", 8080); // Host on localhost:8080
    drogon::app().run();
    return 0;
}
