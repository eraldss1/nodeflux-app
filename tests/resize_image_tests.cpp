#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"
#include "crow.h"
#include "main.cpp"

TEST_CASE("Resize Image Route Test", "[resize_image]")
{
    crow::SimpleApp app;

    crow::request request;
    request.method = crow::HTTPMethod::POST;
    request.body = R"({
        "input_jpeg": "base64_encoded_data_here",
        "desired_width": 100,
        "desired_height": 100
    })";

    crow::response response;

    app.handle(request, response);

    REQUIRE(response.code == 200);

    crow::json::rvalue response_json = crow::json::load(response.body);
    REQUIRE(response_json);

    int response_code = response_json["code"].i();
    REQUIRE(response_code == 200);

    std::string message = response_json["message"].s();
    REQUIRE(message == "success");

    std::string output_jpeg = response_json["output_jpeg"].s();
    REQUIRE(output_jpeg.size() > 0);
}
