#include "model_checker.h"

#include "help_functions.h"

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace phishscore {

auto escape_json = [](std::string str) -> std::string {
    for (size_t i = 0, end = str.size(); i < end; ++i) {
        if (str[i] == '"') {
            str.replace(i, 1, "\\\"");
            end++;
            i++;
        }
    }
    return str;
};

model_checker_t::model_checker_t(const options& opts)
    : opts_(opts)
{
}

json model_checker_t::use_program(const std::string& escaped_json)
{
    auto cmd = fmt::format("{} --data-json \"{}\"", opts_.model_checker.path, escaped_json);
    if (opts_.verbose) fmt::print("--> executing command: {}\n", cmd);
    auto str = help_functions::get_output_from_program_in_string(cmd.c_str());
    return json::parse(str);
}

json model_checker_t::use_service(const std::string& request)
{
    if (opts_.verbose) fmt::print("--> connecting to model checking {}:{}\n",
        opts_.model_checker.host, opts_.model_checker.port);
    json response = json::object();


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        auto msg = "can't create socket";
        fmt::print(stderr, "{}\n", msg);
        response["error"] = "RESPONSE_MODEL_CHECKER";
        response["message"] = msg;
        close(sock);
        return response;
    }
    sockaddr_in model_checker_server{};
    memset(&model_checker_server, 0, sizeof(model_checker_server));
    model_checker_server.sin_family = AF_INET;
    model_checker_server.sin_port = htons(opts_.model_checker.port);

    const char* host = opts_.model_checker.host.c_str();

    if (inet_pton(AF_INET, host, &model_checker_server.sin_addr) < 1) {
        auto msg = "invalid server address";
        fmt::print(stderr, "{}\n", msg);
        response["error"] = "RESPONSE_MODEL_CHECKER";
        response["message"] = msg;
        close(sock);
        return response;
    }

    if (connect(sock, reinterpret_cast<struct sockaddr*>(&model_checker_server),
        sizeof(model_checker_server)) == -1)
    {
        auto msg = "can't connect to a server. is server running?";
        fmt::print(stderr, "{}\n", msg);
        response["error"] = "RESPONSE_MODEL_CHECKER";
        response["message"] = msg;
        close(sock);
        return response;
    }
    if (opts_.verbose) fmt::print("--> connected");
    if (opts_.verbose) fmt::print("--> sending request: {}\n", request);

    if (send(sock, request.c_str(), request.size(), 0) == -1) {
        auto msg = "send has failed";
        fmt::print(stderr, "{}\n", msg);
        response["error"] = "RESPONSE_MODEL_CHECKER";
        response["message"] = msg;
        close(sock);
        return response;
    }

    // TODO: change to dynamic number
    char buffer[4096] = {0};
    ssize_t rc = read(sock, buffer, 4096);
    if (rc <= 0) {
        auto msg = "empty response";
        response["error"] = "RESPONSE_MODEL_CHECKER";
        response["message"] = msg;
        close(sock);
        return response;
    }

    close(sock);
    return json::parse(buffer);
}

json model_checker_t::predict(const json& features_json)
{
    if (opts_.model_checker.port == 0) {
        return use_program(escape_json(features_json.dump()));
    } else {
        return use_service(features_json.dump());
    }
}

} // namespace phishscore
