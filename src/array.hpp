#pragma once

#ifndef ARRAY_H
#define ARRAY_H
#include <cstdint>
#include <cstdlib>
#include <strings.h>
#include <random>
#include <vector>
#include <string>

#include "run.hpp"
using namespace std;

template<class K,class V>
class Array : public Run<K,V> {
private:
    int _capacity;

public:
    K min;
    K max;
    vector<KVPair<K,V>> data;
    Array(const K minKey,const K maxKey){
        _capacity = 100;
        set_size(_capacity);
        min = minKey;
        max = maxKey;
    }

    ~Array(){

    }

    int get_capacity() {
        return _capacity;
    }

    int set_capacity(int capacity) {
        _capacity = capacity;
    }

    K get_min() {
        return min;
    }

    K get_max() {
        return max;
    }

    void insert_key(const K &key, const V &value) {
        //cout << "data size " << data.size() << "_capacity:" << _capacity << endl;
        // 这段代码没发挥作用
        // if (data.size() == _capacity) {
        //     _capacity = _capacity * 2;
        //     set_size(_capacity);
        // }
        bool found = false;
        int index = binary_search(key, found); // 在找到的地方后面插入
        //cout << "binary_search - key:" << key << " - value:" << value << " - found:" << found << " - index:" << index << endl;
        if (found) {
            index ++;
        }
        if(key > max) {
            max = key;
        }
        if (key < min) {
            min = key;
        }
        // 在下标index前进行插入
        KVPair<K,V> p;
        p.key = key;
        p.value = value;
        data.insert(data.begin()+index, p);
    }

    void delete_key(const K &key) {
        bool found = false;
        int sidx = binary_search(key, found);
        
        if (found) {
            data.erase(data.begin()+sidx);
        }
        
    }

    // 插入返回数据的后面
    int binary_search(const K &key, bool &found) {
        found = false;
        int left = 0;
        int right = data.size() - 1;
        int  middle = (left + right) >> 1;
        while (left <= right) {
            if (key > data[middle].key) {
                left = middle + 1;
            }
            else if (key == data[middle].key) {
                found = true;
                // left = middle;
                return middle;
            } else {
                right = middle - 1;
            }
            middle = (left + right) >> 1;
        }
         
        return left;
    }

    int binary_search2(const K &key, bool &found, bool is_last = true) {
        if (is_last) {
            return binary_search_last(key, found);
        } else {
            return binary_search_first(key, found);
        }
    }

    int binary_search_first(const K &key, bool &found) {
        found = false;
        int left = 0;
        int right = data.size() - 1;
        int  middle = (left + right) >> 1;
        while (left <= right) {
            if (key > data[middle].key) {
                left = middle + 1;
            }
            else if (key == data[middle].key) {
                found = true;
                // return middle;
                right = middle;
                if (middle == 0 || data[middle-1].key < key) {
                    return middle;
                }
            } else {
                right = middle - 1;
            }
            middle = (left + right) >> 1;
        }
         
        return left;
    }

    int binary_search_last(const K &key, bool &found) {
        found = false;
        int left = 0;
        int right = data.size() - 1;
        int  middle = (left + right) >> 1;
        while (left <= right) {
            if (key > data[middle].key) {
                left = middle + 1;
            }
            else if (key == data[middle].key) {
                found = true;
                // return middle;
                left = middle;
                if (middle == data.size() -1 || data[middle+1].key > key) {
                    return middle;
                }
            } else {
                right = middle - 1;
            }
            middle = (left + right) >> 1;
        }
         
        return left;
    }


   

    // 二分查找
    // todo顺序查找
    V lookup(const K &key, bool &found) {
        int index = binary_search(key, found);
        return found ? data[index].value: (V) NULL;
    }

    unsigned long long num_elements() {
        return data.size();
    }

    // todoreverse
    void set_size(const unsigned long size) {
        data.reserve(size);
    }

    // todo遍历一遍得到所有的数据
    vector<KVPair<K,V>> get_all() {
        return data;
    }

    // todo遍历一遍得到所有的数据
    vector<KVPair<K,V>> get_all_in_range(const K &key1, const K &key2) {
        bool found = false;
        int start = binary_search(key1, found);
        int end = binary_search(key2, found);
        vector<KVPair<K,V>> arr;
        arr.assign(data.begin() + start, data.begin() + end);
        return arr;
    }
};



#endif /* array_h */