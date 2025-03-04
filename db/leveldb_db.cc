#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "leveldb_db.h"

using namespace std;

namespace ycsbc {
    
LevelDB::LevelDB(const string dbfilename) {
    
    leveldb::Options options;

    readop_ = new leveldb::ReadOptions();
    (*readop_).fill_cache = true;
    
    writeop_ = new leveldb::WriteOptions();
    (*writeop_).sync = false;
    
    // options config
    size_t bloom_bits = 8;
    bool compression_Open = false;
    size_t block_cache_size = 1000;
    size_t max_open_files = 100;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(bloom_bits);
    options.compression = compression_Open? leveldb::kSnappyCompression:leveldb::kNoCompression; 
    options.block_cache = leveldb::NewLRUCache(block_cache_size); //block cache, 
    options.max_open_files = max_open_files; //sstable cache, even if you set a lower value, they will give it at least 64. 

    fprintf(stderr, "Leveldb configured finished.\n");
    
    leveldb::Status status = leveldb::DB::Open(options,dbfilename,&db_);

    if (!status.ok()) {
        fprintf(stderr, "can't open leveldb\n");
        cerr << status.ToString() << endl;
        exit(0);
    }
}

int LevelDB::Read(const string& table, 
                    const string& key, 
                    string &result) {
    leveldb::Status s = db_->Get(*readop_,key,&result);
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

int LevelDB::Insert(const string& table, 
                    const string& key, 
                    const string& value) {

    leveldb::Status s = db_->Put(*writeop_,key,value);
    if(!s.ok()){
        fprintf(stderr,"insert error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int LevelDB::Delete(const string& table, const string& key) {
    leveldb::Status s = db_->Delete(*writeop_,key);
    if(!s.ok()){
        fprintf(stderr,"delete error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int LevelDB::Scan(const string& table, const string& key, 
                    int len, std::vector<std::string> &results) {
    std::vector<DB::KVPair> scanValue;
    std::string value;
    leveldb:: Iterator* iter = db_->NewIterator(*readop_);
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

int LevelDB::Update(const string& table, const string& key, const string &value) {
    return Insert(table,key,value);
}

void LevelDB::GetProperty(const std::string &property) {
    std::string value;
    if (db_->GetProperty("leveldb.disk-access", &value))
        std::cerr << value.c_str() << std::endl;
}

void LevelDB::Close() {};

LevelDB::~LevelDB() {
    delete db_;
}

}