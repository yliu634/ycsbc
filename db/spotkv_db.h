#ifndef YCSB_C_SPOTKV_H
#define YCSB_C_SPOTKV_H

#include "core/db.h"
#include "core/properties.h"

#include <iostream>
#include <string>
#include <fstream>

#include <spotkv/db.h>
#include <spotkv/env.h>
#include <spotkv/cache.h>
#include <spotkv/slice.h>
#include <spotkv/filter_policy.h>

using std::cout;
using std::endl;

namespace ycsbc {
class SpotKV : public DB{
  public :
    SpotKV(const std::string filename);
    int Read(const std::string &table, const std::string &key,
            std::string &result);
    int Insert(const std::string &table, const std::string &key,
            const std::string &value);
    int Scan(const std::string &table, const std::string &key, int len,
            std::vector<std::string> &results);
    int Update(const std::string &table, const std::string &key,
            const std::string &value);
    int Delete(const std::string &table, const std::string &key);
    void GetProperty(const std::string &property);
    virtual ~SpotKV();
    void Close();

  private:
    spotkv::DB *db_;
    spotkv::ReadOptions *readop_;
    spotkv::WriteOptions *writeop_;

};

}
#endif