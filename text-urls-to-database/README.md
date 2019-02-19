# Prerequisites

1. Poco library (instructions in benchmark project)
2. sqlite3 
    1. `sudo apt-get install libsqlite3-dev`
    2. `#include <sqlite3.>` in `C++` code
    3. link `sqlite3` in `CMakelLists.txt`
    
    
Execution:

        ./text_urls_to_database --input urls.txt --output urls.db
        