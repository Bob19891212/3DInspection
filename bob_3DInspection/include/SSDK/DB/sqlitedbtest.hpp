#ifndef SQLITEDBTEST_H
#define SQLITEDBTEST_H

#include <string>

#include "../../include/boost_1_63_0/boost/timer.hpp"

#include "Archive/json.hpp"
#include "sqlitedb.hpp"

/**
 *  @brief A test class for SqliteDB
 *
 *  @author rime
 *  @version 1.00 2017-04-05 rime
 *                note:create it
*/
    namespace SSDK
    {
        namespace DB
        {
            class SqliteDBTest
            {
            public:
                SqliteDBTest();

                void testCreateTable();
                void testJsonHelper();
                void testJsonToSqlite(SqliteDB& db, const std::string& sqlinsert);
                void testPerformance();
            };
        }
    }

#endif // SQLITEDBTEST_H
