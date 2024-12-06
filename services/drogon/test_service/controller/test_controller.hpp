#pragma once

#include <drogon/HttpController.h>

namespace TestService
{
    class TestController final : public drogon::HttpController<TestController>
    {
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(TestController::helloWorld, "/hello", drogon::Get); // Endpoint: /hello
        ADD_METHOD_TO(TestController::sumNumbers, "/sum?num1={val1}&num2={val2}",
                      drogon::Get); // Endpoint: /sum/{num1}/{num2}
        ADD_METHOD_TO(TestController::handlePreflight, "/sum?num1={val1}&num2={val2}", drogon::Options);

        METHOD_LIST_END

        // Declare functions for endpoints
        void helloWorld(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback);
        void sumNumbers(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    private:
        static void handlePreflight(const drogon::HttpRequestPtr& req,
                             std::function<void(const drogon::HttpResponsePtr&)>&& callback)
        {
            const auto resp = drogon::HttpResponse::newHttpResponse();

            // Add CORS headers for preflight response
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }

        int32_t cnt_op{0};
    };
} // namespace TestService
