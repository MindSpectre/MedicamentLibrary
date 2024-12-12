#include "test_controller.hpp"

using namespace drogon;
using namespace TestService;

void TestController::helloWorld(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    // Create a simple JSON response
    Json::Value response;
    response["message"] = "Hello, World!";
    response["method"] = req->getMethodString();

    // Send the response using the callback
    const auto resp = HttpResponse::newHttpJsonResponse(response);
    cnt_op++;
    callback(resp);
}

void TestController::sumNumbers(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    const auto num1Str = req->getParameter("num1");
    const auto num2Str = req->getParameter("num2");
    LOG_INFO << "num1: " << num1Str << " num2: " << num2Str;
    try
    {
        // Convert to integers and calculate the sum
        const int num1 = std::stoi(num1Str);
        const int num2 = std::stoi(num2Str);
        const int sum = num1 + num2;

        // Create JSON response
        Json::Value response;
        response["num1"] = num1;
        response["num2"] = num2;
        response["sum"] = sum;

        const auto resp = HttpResponse::newHttpJsonResponse(response);
        cnt_op++;
        callback(resp);
    }
    catch (const std::exception&)
    {
        // Handle invalid input
        const auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid input: Please provide valid integers.");
        callback(resp);
    }
}
