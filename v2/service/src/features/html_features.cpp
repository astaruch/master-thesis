#include "html_features.h"

#include "../help_functions.h"

#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

html_features_t::html_features_t(std::string_view url,
                                 uint64_t flags,
                                 std::string_view node_bin,
                                 std::string_view html_script)
    : url_(url)
    , flags_(flags)
    , node_bin_(node_bin)
    , html_script_(html_script)
{
    auto args = create_args();
    cmd_ = fmt::format("{} {} --output-lines --url '{}' {}", node_bin_, html_script_, url, args);
}

html_features_t::html_features_t(std::string_view url,
                                 uint64_t flags,
                                 std::string_view exe_path,
                                 bool extra_values = false)
    : url_(url)
    , flags_(flags)
    , exe_path_(exe_path)
{
    auto args = create_args();
    cmd_ = fmt::format("{} {} --output-values-string --url '{}' {}",
        exe_path_,
        args,
        url,
        extra_values ? "--include-values" : "");
    // fmt::print("{}\n", cmd_);
}

html_features_t::html_features_t(std::string_view url,
                                 uint64_t flags,
                                 std::string_view exe_path,
                                 uint16_t port)
    : url_(url)
    , flags_(flags)
    , exe_path_(exe_path)
    , port_(port)
{
    auto args = create_args();
    cmd_ = fmt::format("{} {} --output-json --url '{}'",
        exe_path_,
        args,
        url);
}

std::string html_features_t::get_header()
{
    auto cmd = fmt::format("{} --print-only-header", cmd_);
    return help_functions::get_output_from_program_in_string(cmd.c_str());
}

std::tuple<std::string, std::string> html_features_t::split_by_space(const std::string& str)
{
    auto found = str.find(' ');
    return {str.substr(0, found), str.substr(found + 1)};
}

std::vector<double> html_features_t::compute_values()
{
    auto lines = help_functions::get_output_from_program(cmd_.c_str());
    std::vector<double> f_vec;
    for (const auto& line: lines) {
        auto [column, value] = split_by_space(line);
        f_vec.push_back(std::stod(value));
    }
    return f_vec;
}

std::vector<double> html_features_t::get_values_from_external_script()
{
    // 0th line are values
    // 1th line is optional error message
    auto output = help_functions::get_line_from_program_if_exists(cmd_.c_str(), 0);
    std::vector<double> result;
    std::stringstream stream(output);
    std::string value_str;
    while(std::getline(stream, value_str, ',')) {
        result.push_back(std::stod(value_str));
    }
    return result;
}

json html_features_t::prepare_request() const
{
    // {
    //     "url":"http://google.com",
    //     "features": {
    //         "featSrcLink": true,
    //         "featAhrefLink": true,
    //         "featFaviconLink": true
    //     }
    // }
    json payload = json::object();
    payload["url"] = url_;
    json features = json::object();
    auto to_camel_case = [](std::string_view strview) -> std::string {
        std::string str = "feat";
        bool active = true;
        for (size_t i = 0; strview[i] != '\0'; i++) {
            if (std::isalpha(strview[i])) {
                if (active) {
                    str += static_cast<char>(std::toupper(strview[i]));
                    active = false;
                } else {
                    str += static_cast<char>(std::tolower(strview[i]));
                }
            } else if (strview[i] == '_') {
                active = true;
            }
        }
        return str;
    };
    for (const auto feature: feature_enum::html) {
        if (flags_ & feature) {
            auto col = feature_enum::column_names.at(feature);
            features[to_camel_case(col)] = true;
        }
    }
    payload["features"] = features;
    return payload;
}

std::string html_features_t::get_response_from_html_analysis(const std::string& request) const
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fmt::print(stderr, "can't create socket");
          close(sock);
          return "";
    }
    sockaddr_in html_analysis_server{};
    memset(&html_analysis_server, 0, sizeof(html_analysis_server));
    html_analysis_server.sin_family = AF_INET;
    html_analysis_server.sin_port = htons(port_);

    if (inet_pton(AF_INET, "127.0.0.1", &html_analysis_server.sin_addr) < 1) {
        fmt::print(stderr, "invalid server address");
        close(sock);
        return "";
    }

    if (connect(sock, reinterpret_cast<struct sockaddr*>(&html_analysis_server),
        sizeof(html_analysis_server)) == -1)
    {
        fmt::print(stderr, "can't connect to a server");
        close(sock);
        return "";
    }
    fmt::print("connected");

    if (send(sock, request.c_str(), request.size(), 0) == -1) {
        fmt::print(stderr, "send has failed");
        close(sock);
        return "";
    }

    // TODO: change to dynamic number
    char buffer[4096] = {0};
    ssize_t rc = read(sock, buffer, 4096);
    if (rc <= 0) {
        fmt::print("empty response");
    }

    close(sock);
    return std::string(buffer);
}

std::unordered_map<std::string_view, double> html_features_t::compute_values_map() const
{
    std::string output_json;
    fmt::print("port = {}\n", port_);
    if (port_ != 0) {
        json request = prepare_request();
        fmt::print("request: {}\n", request.dump());
        output_json = get_response_from_html_analysis(request.dump());
        fmt::print("response: {}\n", output_json);
    } else {
        output_json = help_functions::get_output_from_program_in_string(cmd_.c_str());
    }
    json parsed = json::parse(output_json);
    std::unordered_map<std::string_view, double> values;
    for (const auto feature: feature_enum::html) {
        if (flags_ & feature) {
            auto col = feature_enum::column_names.at(feature);
            values[col] = parsed[std::string(col)];
        }
    }
    return values;
}

std::string html_features_t::create_args()
{
    std::string args;
    for (const auto id: feature_enum::html) {
        if (flags_ & id) {
            args += fmt::format("{} ", html_feature_arg.at(id));
        }
    }
    return args;
}
