#include <spdlog/spdlog.h>

#include "config.h"

namespace fs = std::filesystem;

namespace phishapp {

namespace defaults {

namespace weights {

static const int length = 0;
static const int depth = 0;
static const int special_chars = 0;
static const int special_keywords = 0;
static const int ip_hostname = 0;
static const int non_std_port = 0;
static const int non_std_tld = 0;
static const int query_script = 0;
static const int query_html = 0;
static const int chars_freq = 0;

}; // namespace weights

namespace params {

static const int max_length = 53;
static const int max_depth = 5;
static const char special_chars[] = "['@', '/', '?', '.', '=', '-', '_']";
static const char special_keywords[] = "['webscr', 'secure', 'banking', 'ebayisapi', 'account', 'confirm', 'login', 'signin']";

} // namespace params

} // namespace defaults

std::unique_ptr<Config> Config::m_config;
std::mutex Config::m_mtx;

Config::Config(const std::string& path)
        :
        m_path(path)
{
}

Config&
Config::get_instance()
{
    if (!m_config) {
        throw std::logic_error("Requested config without a path");
    }
    return *m_config;
}

YAML::Node&
Config::get_config()
{
    if (!m_config) {
        throw std::logic_error("Requested config without a path");
    }
    return m_config->m_root_node;
}

void
Config::set_path(const std::string& path)
{
    spdlog::info("Setting a path for config to '{}'", path);
    if (m_config && !m_config->m_path.empty()) {
        spdlog::error("Path for config was already set. Path: {}", m_config->m_path);
        return;
    }
    m_config.reset(new Config(path));
    m_config->load();
}

void
Config::load()
{
    std::lock_guard<std::mutex> lck(m_mtx);
    auto& cfg = m_config->m_root_node;
    if (!fs::exists(m_config->m_path)) {
        spdlog::warn("the config file at location '{}' doesn't exist", m_config->m_path);
        cfg = load_defaults();
    }
    else {
        spdlog::info("loading config file from '{}'", m_config->m_path);
        cfg = YAML::LoadFile(m_config->m_path);
        if (m_config->m_root_node.IsNull()) {
            spdlog::error("the config file is corrupted");
            cfg = load_defaults();
        }
    }
    spdlog::info("config file loaded");
    print();
}

void
Config::print()
{
    spdlog::info("test weight - length: '{}'", weight_length());
    spdlog::info("test weight - max_depth: '{}'", weight_depth());
    spdlog::info("test weight - special_chars: '{}'", weight_special_chars());
    spdlog::info("test weight - special_keywords: '{}'", weight_special_keywords());
    spdlog::info("test weight - hostname_ip: '{}'", weight_hostname_ip());
    spdlog::info("test weight - non_std_port: '{}'", weight_non_std_port());
    spdlog::info("test weight - non_std_tld: '{}'", weight_non_std_tld());
    spdlog::info("test weight - query_script: '{}'", weight_query_script());
    spdlog::info("test weight - query_html: '{}'", weight_query_html());
    spdlog::info("test weight - chars_freq: '{}'", weight_chars_freq());
    spdlog::info("database connection: '{}'", conn_string());
}

void
Config::save()
{
    std::lock_guard<std::mutex> lck(m_mtx);
    spdlog::info("Saving the configuration into file '{}'", m_config->m_path);
    std::ofstream outputFile(m_config->m_path);
    outputFile << m_config->m_root_node;
}

void
Config::destroy()
{
    m_config.reset(nullptr);
}

YAML::Node
Config::load_defaults()
{
    spdlog::info("Loading default config values");
    YAML::Node cfg;

    cfg["tests"]["length"]["weight"] = defaults::weights::length;
    cfg["tests"]["length"]["max_length"] = defaults::params::max_length;
    cfg["tests"]["length"]["active"] = true;

    cfg["tests"]["depth"]["weight"] = defaults::weights::depth;
    cfg["tests"]["depth"]["max_depth"] = defaults::params::max_depth;
    cfg["tests"]["depth"]["active"] = true;

    cfg["tests"]["special_chars"]["weight"] = defaults::weights::special_chars;
    cfg["tests"]["special_chars"]["chars"] = YAML::Load(defaults::params::special_chars);
    cfg["tests"]["special_chars"]["active"] = true;

    cfg["tests"]["special_keywords"]["weight"] = defaults::weights::special_keywords;
    cfg["tests"]["special_keywords"]["keywords"] = YAML::Load(defaults::params::special_keywords);
    cfg["tests"]["special_keywords"]["active"] = true;

    cfg["tests"]["hostname_ip"]["weight"] = defaults::weights::ip_hostname;
    cfg["tests"]["hostname_ip"]["active"] = true;

    cfg["tests"]["non_std_port"]["weight"] = defaults::weights::non_std_port;
    cfg["tests"]["non_std_port"]["active"] = true;

    cfg["tests"]["non_std_tld"]["weight"] = defaults::weights::non_std_tld;
    cfg["tests"]["non_std_tld"]["active"] = true;

    cfg["tests"]["query_script"]["weight"] = defaults::weights::query_script;
    cfg["tests"]["query_script"]["active"] = true;

    cfg["tests"]["query_html"]["weight"] = defaults::weights::query_html;
    cfg["tests"]["query_html"]["active"] = true;

    cfg["tests"]["chars_freq"]["weight"] = defaults::weights::chars_freq;
    cfg["tests"]["chars_freq"]["active"] = true;

    cfg["conn_string"] = "postgres://postgres@localhost:5432/phishing-detection";

    return cfg;
}

} // namespace phishapp