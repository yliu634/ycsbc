#include <iostream>
#include <memory>
#include <vector>
#include <cstring>

#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "spotkv_db.h"

using namespace std;

namespace ycsbc {
    
SpotKV::SpotKV(const string dbfilename) {
    
    spotkv::Options options;

    readop_ = new spotkv::ReadOptions();
    (*readop_).fill_cache = true;
    
    writeop_ = new spotkv::WriteOptions();
    (*writeop_).sync = false;
    
    // options config
    size_t bloom_bits = 8;
    bool compression_Open = false;
    size_t block_cache_size = 1000;
    size_t max_open_files = 100;
    options.create_if_missing = true;
    options.filter_policy = spotkv::NewBloomFilterPolicy(bloom_bits);
    options.compression = compression_Open? spotkv::kSnappyCompression:spotkv::kNoCompression; 
    options.block_cache = spotkv::NewLRUCache(block_cache_size); //block cache, 
    options.max_open_files = max_open_files; //sstable cache, even if you set a lower value, they will give it at least 64. 

    fprintf(stderr, "Spotkv configured finished.\n");
    
    spotkv::Status status = spotkv::DB::Open(options,dbfilename,&db_);

    if (!status.ok()) {
        fprintf(stderr, "can't open spotkv\n");
        cerr << status.ToString() << endl;
        exit(0);
    }
}

int SpotKV::Read(const string& table, 
                    const string& key, 
                    string &result) {
    spotkv::Status s = db_->Get(*readop_,key,&result);
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

int SpotKV::Insert(const string& table, 
                    const string& key, 
                    const string& value) {

    spotkv::Status s = db_->Put(*writeop_,key,value);
    if(!s.ok()){
        fprintf(stderr,"insert error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int SpotKV::Delete(const string& table, const string& key) {
    spotkv::Status s = db_->Delete(*writeop_,key);
    if(!s.ok()){
        fprintf(stderr,"delete error!\n");
        cout<<s.ToString()<<endl;
        exit(0);
    }
    return DB::kOK;
}

int SpotKV::Scan(const string& table, const string& key, 
                    int len, std::vector<std::string> &results) {
    std::vector<DB::KVPair> scanValue;
    std::string value;
    spotkv:: Iterator* iter = db_->NewIterator(*readop_);
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

int SpotKV::Update(const string& table, const string& key, const string &value) {
    return Insert(table,key,value);
}

void SpotKV::GetProperty(const std::string &property) {
    std::string value;
    if (db_->GetProperty("spotkv.disk-access", &value))
        std::cerr << value.c_str() << std::endl;
}

void SpotKV::Close() {};

SpotKV::~SpotKV() {
    delete db_;
}

}