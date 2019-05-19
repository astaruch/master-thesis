#ifndef PHISHING_DETECTION_DATABASE_H
#define PHISHING_DETECTION_DATABASE_H

#include "tao/pq.hpp"

namespace phishapp {

class Database {
public:
    Database();

    bool
    table_exists(const std::string& table_name, const std::string& schema_name);

    void
    create_table(const char* table_name, bool force);

    void
    drop_table(const char* table_name);

    std::shared_ptr<tao::pq::connection>
    get_conn()
    {
        return m_conn;
    }
private:
    std::shared_ptr<tao::pq::connection> m_conn;
};

} // namespace phishapp

#endif //PHISHING_DETECTION_DATABASE_H
