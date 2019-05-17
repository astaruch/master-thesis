#include "database.h"

namespace phishapp {

Database::Database()
{
    const auto conn = tao::pq::connection::create("dbname=phishing-detection");
}

} // namespace phishapp
