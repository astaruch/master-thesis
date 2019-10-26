#include "training_data.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>

#include "features/feature_base.h"



void training_data::set_feature_flags(uint64_t flags)
{
    _feature_flags = flags;
}

void training_data::set_input_data(std::vector<std::string> urls)
{
    _urls.swap(urls);
}

void training_data::set_label(double label)
{
    _label = label;
}

void training_data::set_output_name(std::string name)
{
    _output_name = name;
}

bool training_data::create_training_data()
{
    std::string csv_header = create_csv_header();
    spdlog::info("CSV header = {}", csv_header);
    return false;
}

std::string training_data::create_csv_header()
{
    std::vector<std::string> columns;

    if (_feature_flags & feature_name::ip_address == feature_name::ip_address) {
        columns.push_back("ip_address");
    }

    return std::accumulate(columns.begin(), columns.end(), std::string(),
        [](const std::string& a, const std::string& b) -> std::string {
            return a + (a.length() > 0 ? "," : "") + b;
        }
    );
}
