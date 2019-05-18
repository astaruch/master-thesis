#include "spdlog/spdlog.h"

#include "config.h"
#include "database.h"

namespace phishapp {

Database::Database()
{
    auto conn_string = Config::conn_string();
    const auto conn = tao::pq::connection::create(conn_string);

    const auto tr = conn->transaction();

    const auto r1 = tr->execute("SELECT COUNT(*) FROM phishtank");
    tr->commit();
    for (const auto r: r1) {
        spdlog::info("row: '{}'", r.get(0));
    }
}

} // namespace phishapp
