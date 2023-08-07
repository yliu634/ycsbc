#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "rocksdb_db.h"

using namespace std;

namespace ycsbc {
    
RocksDB::RocksDB(const string dbfilename) {
    
  rocksdb::Options options;
  readop_ = new rocksdb::ReadOptions();
    writeop_ = new rocksdb::WriteOptions();
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  rocksdb::Status s = rocksdb::DB::Open(options,dbfilename,&db_);
  if (!s.ok()) {
        fprintf(stderr, "can't open rocksdb\n");
        cerr << s.ToString() << endl;
        exit(0);
    }

}

int RocksDB::Read(const string& table, 
                    const string& key, 
                    string &result) {
    rocksdb::Status s = db_->Get(*readop_,key,&result);
    if(s.IsNotFound()){
        return DB::kErrorNoData;
    }
    if(!s.ok()){
        cerr<<s.ToString()<<endl;
        fprintf(stderr,"read error\n");
        exit(0);
    }
    return DB::kOK;
}

int RocksDB::Insert(const string& table, 
                    const string& key, 
                    const string& value) {

    rocksdb::Status s = db_->Put(*writeop_,key,value);
    if(!s.ok()){
        fprintf(stderr,"insert error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int RocksDB::Delete(const string& table, const string& key) {
    rocksdb::Status s = db_->Delete(*writeop_,key);
    if(!s.ok()){
        fprintf(stderr,"delete error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int RocksDB::Scan(const string& table, const string& key, 
                    int len, std::vector<std::string> &results) {
    std::vector<DB::KVPair> scanValue;
    std::string value;
    rocksdb:: Iterator* iter = db_->NewIterator(*readop_);
    iter->Seek(key);
    int num_next = len;
    if (iter->Valid() && iter->key() == key) {   
        for (int j = 0; iter->Valid() && j < num_next; j ++) {
            //result.push_back(iter->value());
            iter->Next();
        }
    } else {    	  
        return DB::kErrorNoData;
    }
    delete iter;
    return DB::kOK;
}

int RocksDB::Update(const string& table, const string& key, const string &value) {
    return Insert(table,key,value);
}

void RocksDB::Close() {};

RocksDB::~RocksDB() {
    delete db_;
}

}