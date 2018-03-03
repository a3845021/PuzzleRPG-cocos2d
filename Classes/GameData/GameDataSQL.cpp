#include "GameDataSQL.h"

#define dbName "sample.db"

USING_NS_CC;

//データベースを開く
int GameDataSQL::sqliteOpen(sqlite3 **database)
{
    std::string filePath = FileUtils::getInstance()->getWritablePath();
    filePath += dbName;
    return sqlite3_open(filePath.c_str(), database);
}

//データベースのテーブルの作成
void GameDataSQL::sqliteCreateTable()
{
    // データベースファイルを作成する
    sqlite3 *db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK)
    {
        const char *sql_createtable = "CREATE TABLE test1(key TEXT,value TEXT)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_createtable, -1, &stmt, NULL) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_DONE)
            {
                CCLOG("create table done");
            }
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
}

//keyから値を取り出す。
const char *GameDataSQL::sqliteGetValueForKey(const char *key)
{
    // キー文字列に対応する文字列を取得する
    __String *strValue = NULL;
    sqlite3 *db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK)
    {
        const char *sql_select = "SELECT value FROM test1 WHERE key=?";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            int ret = sqlite3_step(stmt);
            if( ret == SQLITE_DONE || ret == SQLITE_ROW)
            {
                const char *val = (const char *)sqlite3_column_text(stmt, 0);
                strValue = __String::create(val);
            }
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    
    if (strValue != NULL)
    {
        return strValue->getCString();
    }
    return NULL;
}

//keyとvalueを設定する(初期)
void GameDataSQL::sqliteSetValueForKey(const char *key, const char *value)
{
    // キー文字列に対応する文字列を設定する
    std::string fullpath = FileUtils::getInstance()->getWritablePath();
    fullpath += dbName;
    sqlite3 *db = NULL;
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK)
    {
        const char *sql_select = "REPLACE INTO test1 (key, value) VALUES (?,?)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, value, -1, SQLITE_TRANSIENT);
            if( sqlite3_step(stmt) == SQLITE_DONE)
            {
                CCLOG("replace key:%s value:%s", key, value);
            }
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
}