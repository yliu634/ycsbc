#ifndef YCSB_C_LEVELDB_H
#define YCSB_C_LEVELDB_H

#include "core/db.h"
#include "core/properties.h"

#include <iostream>
#include <string>
#include <fstream>

#include <leveldb/db.h>
#include <leveldb/env.h>
#include <leveldb/filter_policy.h>

using std::cout;
using std::endl;

namespace ycsbc {
class LevelDB : public DB{
  public :
    LevelDB(const std::string filename);
    int Read(const std::string &table, const std::string &key,
            std::string &result);
    int Insert(const std::string &table, const std::string &key,
            const std::string &value);
    int Scan(const std::string &table, const std::string &key, int len,
            std::vector<std::string> &results);
    int Update(const std::string &table, const std::string &key,
            const std::string &value);
    int Delete(const std::string &table, const std::string &key);
    virtual ~LevelDB();
    void Close();

  private:
    leveldb::DB *db_;
    leveldb::ReadOptions *readop_;
    leveldb::WriteOptions *writeop_;

};

}
#endif