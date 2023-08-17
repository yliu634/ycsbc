//
//  basic_db.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_BASIC_DB_H_
#define YCSB_C_BASIC_DB_H_

#include "core/db.h"

#include <iostream>
#include <string>
#include <mutex>
#include "core/properties.h"

using std::cout;
using std::endl;

namespace ycsbc {

class BasicDB : public DB {
 public:
  void Init() {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "A new thread begins working." << endl;
  }

  int Read(const std::string &table, const std::string &key,
           std::string &result) {
    std::lock_guard<std::mutex> lock(mutex_);
    return 0;
  }

  int Scan(const std::string &table, const std::string &key,
           int len, std::vector<std::string> &results) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "SCAN " << table << ' ' << key << " " << len;

    return 0;
  }

  int Update(const std::string &table, const std::string &key,
             const std::string &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "UPDATE " << table << ' ' << key << " [ ";
    cout << ']' << endl;
    return 0;
  }

  int Insert(const std::string &table, const std::string &key,
             const std::string &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "INSERT " << table << ' ' << key << " [ ";
    return 0;
  }

  int Delete(const std::string &table, const std::string &key) {
    std::lock_guard<std::mutex> lock(mutex_);
    cout << "DELETE " << table << ' ' << key << endl;
    return 0; 
  }

  void GetProperty(const std::string &property) {
    return;
  }

 private:
  std::mutex mutex_;
};

} // ycsbc

#endif // YCSB_C_BASIC_DB_H_

