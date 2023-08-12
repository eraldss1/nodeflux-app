#include "crow.h"
#include "crow/utility.h"
#include "crow/json.h"
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>

std::string resizeImage(const std::string &input_jpeg, int desired_width, int desired_height);

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
     { return "root"; });

    CROW_ROUTE(app, "/resize_image")
        .methods(crow::HTTPMethod::POST)([](const crow::request &req)
                                         {
        try {
            crow::json::rvalue json_payload = crow::json::load(req.body);
            if (!json_payload)
                throw crow::response(400, "Invalid JSON payload");

            std::string input_jpeg = json_payload["input_jpeg"].s();
            int desired_width = json_payload["desired_width"].i();
            int desired_height = json_payload["desired_height"].i();

            std::string resized_image = resizeImage(input_jpeg, desired_width, desired_height);

            crow::json::wvalue response;
            response["code"] = 200;
            response["message"] = "success";
            response["output_jpeg"] = resized_image;

            return crow::response(response);
        } catch (const std::exception& e) {
            crow::json::wvalue error_response;
            error_response["code"] = 500;
            error_response["message"] = e.what();

            return crow::response(500, error_response);
        } });

    app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}

std::string resizeImage(const std::string &input_jpeg, int desired_width, int desired_height)
{
    try
    {
        // Decode base64
        std::string decoded_data = crow::utility::base64decode(input_jpeg);

        // Convert decoded_data to cv
        std::vector<uchar> buffer(decoded_data.begin(), decoded_data.end());
        cv::Mat image = cv::imdecode(buffer, cv::IMREAD_COLOR);

        // Resize image
        cv::resize(image, image, cv::Size(desired_width, desired_height));

        // Encode the resized image to base64
        std::vector<uchar> resized_buffer;
        cv::imencode(".jpg", image, resized_buffer);
        std::string resized_encoded = crow::utility::base64encode(resized_buffer.data(), resized_buffer.size());

        return resized_encoded;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error resizing image: " + std::string(e.what()));
    }
}
