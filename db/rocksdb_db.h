#ifndef YCSB_C_ROCKSDB_H
#define YCSB_C_ROCKSDB_H

#include "core/db.h"
#include "core/properties.h"

#include <iostream>
#include <string>
#include <fstream>

#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>

using std::cout;
using std::endl;

namespace ycsbc {
class RocksDB : public DB{
  public :
    RocksDB(const std::string filename);
    int Read(const std::string &table, const std::string &key,
            std::string &result);
    int Insert(const std::string &table, const std::string &key,
            const std::string &value);
    int Scan(const std::string &table, const std::string &key, int len,
            std::vector<std::string> &results);
    int Update(const std::string &table, const std::string &key,
            const std::string &value);
    int Delete(const std::string &table, const std::string &key);
    virtual ~RocksDB();
    void Close();

  private:
    rocksdb::DB *db_;
    rocksdb::ReadOptions *readop_;
    rocksdb::WriteOptions *writeop_;

};

}
#endif