#ifndef MASTER_THESIS_CONFIG_H
#define MASTER_THESIS_CONFIG_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>

#include <yaml-cpp/yaml.h>

namespace phishapp {

/**
 * Singleton config class for configuration
 */
class Config {
public:
    Config(Config const&) = delete;

    Config
    operator=(Config const&) = delete;

    static void
    set_path(const std::string& path);

    static void
    save();

    static int
    weight_length()
    {
        return m_config->m_root_node["tests"]["length"]["weight"].as<int>();
    }

    static int
    weight_depth()
    {
        return m_config->m_root_node["tests"]["depth"]["weight"].as<int>();
    }

    static int
    weight_special_chars()
    {
        return m_config->m_root_node["tests"]["special_chars"]["weight"].as<int>();
    }

    static int
    weight_special_keywords()
    {
        return m_config->m_root_node["tests"]["special_keywords"]["weight"].as<int>();
    }

    static int
    weight_hostname_ip()
    {
        return m_config->m_root_node["tests"]["hostname_ip"]["weight"].as<int>();
    }

    static int
    weight_non_std_port()
    {
        return m_config->m_root_node["tests"]["non_std_port"]["weight"].as<int>();
    }

    static int
    weight_non_std_tld()
    {
        return m_config->m_root_node["tests"]["non_std_tld"]["weight"].as<int>();
    }

    static int
    weight_query_script()
    {
        return m_config->m_root_node["tests"]["query_script"]["weight"].as<int>();
    }

    static int
    weight_query_html()
    {
        return m_config->m_root_node["tests"]["query_html"]["weight"].as<int>();
    }

    static int
    weight_chars_freq()
    {
        return m_config->m_root_node["tests"]["chars_freq"]["weight"].as<int>();
    }

private:
    Config() = default;
    explicit Config(const std::string& path);

    static Config&
    get_instance();

    static YAML::Node&
    get_config();

    void
    load();

    YAML::Node
    load_defaults();

    static std::unique_ptr<Config> m_config;
    static std::mutex m_mtx;

    std::string m_path;
    YAML::Node m_root_node;
};

} // phishapp

#endif //MASTER_THESIS_CONFIG_H
