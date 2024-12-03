#include <drogon/drogon.h>

int main() {
    // Load configuration and run the Drogon application
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            //LOG_DEBUG << "postHandling1";
            resp->addHeader("Access-Control-Allow-Origin", "*");
        }).loadConfigFile("./config/drogon_config.json").run();
    return 0;
}
