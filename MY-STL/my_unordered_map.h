/*************************************************************************
	> File Name: my_map.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Fri 20 Aug 2021 08:49:47 PM CST
 ************************************************************************/

#ifndef _MY_UNORDERED_MAP_H
#define _MY_UNORDERED_MAP_H
#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }
#include "my_function.h"
#include "my_hash_fun.h"
#include "my_hashtable.h"

_MY_NAMESPACE_BEGIN

// unordered_set 类的封装;
template <class Key, class Value,
         class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value> >
class unordered_map {
private:    // 这里以 pair<key, val> 作为hashtable节点的值插入;
    typedef hashtable<pair<const Key, Value>, Key, HashFcn, select1st<pair<const Key, Value> >, EqualKey>   ht;
    ht rep;
public:
    typedef typename ht::key_type           key_type;
    typedef typename ht::value_type         value_type;
    typedef typename ht::hasher             hasher;
    typedef typename ht::key_equal          key_equal;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;
    typedef typename ht::pointer            pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::iterator           iterator;

    typedef  Value      data_type;
    typedef  Value      mapped_type;

    hasher hash_funct() const {return rep.hash_funct();}
    key_equal key_eq() const {return rep.key_eq();}
public:
    unordered_map() : rep(100, hasher(), key_equal()) {}
    explicit unordered_map(size_type n)
        : rep(n, hasher(), key_equal()) {}
    unordered_map(size_type n, const hasher& hf)
        : rep(n, hf, key_equal()) {}
    unordered_map(size_type n, const hasher& hf, const key_equal& eql)
        : rep(n, hf, eql) {}
public:
    size_type size() const {return rep.elems_count();}
    size_type max_size() const {return rep.max_size();}
    bool empty() const {return rep.empty();}
    
    void swap(unordered_map& rhs) {
        rep.swap(rhs.rep);
    }
    
    iterator begin() {return rep.begin();}
    iterator end() {return rep.end();}

public:
    pair<iterator, bool> insert(const value_type& obj) 
    {
        return rep.insert_unique(obj);
    }

    pair<iterator, bool> insert_noresize(const value_type& obj)
    {
        return rep.insert_unique_noresize(obj);
    }
    Value& operator[](const key_type& key) {
        return rep.find_or_insert(value_type(key, Value())).second;
    }

    iterator find(const key_type& key) {
        return rep.find(key);
    }
    
    pair<iterator, iterator> equal_range(const key_type& key) {
        return rep.equal_range(key);
    }

    size_type count(const key_type& key) {
        return rep.count(key);
    }
    
    size_type erase(const key_type& key) {
        return rep.erase(key);
    }

    void erase(iterator& it) {
        rep.erase(it);
        return ;
    }
    void clear() {
        return rep.clear();
    }
public:
    void resize(size_type n) {rep.resize(n);}
    size_type bucket_count() {
        return rep.bucket_count();
    }
    size_type max_bucket_count() {
        return rep.max_bucket_count();
    }
    
    size_type elems_in_bucket(size_type n){
        return rep.elems_in_bucket(n);
    }
};

//unordered_mulitmap 类的封装
template <class Key, class Value,
         class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value> >
class unordered_mulitmap {
private:    // 这里以 pair<key, val> 作为hashtable节点的值插入;
    typedef hashtable<pair<const Key, Value>, Key, HashFcn, select1st<pair<const Key, Value> >, EqualKey>   ht;
    ht rep;
public:
    typedef typename ht::key_type           key_type;
    typedef typename ht::value_type         value_type;
    typedef typename ht::hasher             hasher;
    typedef typename ht::key_equal          key_equal;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;
    typedef typename ht::pointer            pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::iterator           iterator;

    typedef  Value      data_type;
    typedef  Value      mapped_type;

    hasher hash_funct() const {return rep.hash_funct();}
    key_equal key_eq() const {return rep.key_eq();}
public:
    unordered_mulitmap() : rep(100, hasher(), key_equal()) {}
    explicit unordered_mulitmap(size_type n)
        : rep(n, hasher(), key_equal()) {}
    unordered_mulitmap(size_type n, const hasher& hf)
        : rep(n, hf, key_equal()) {}
    unordered_mulitmap(size_type n, const hasher& hf, const key_equal& eql)
        : rep(n, hf, eql) {}
public:
    size_type size() const {return rep.elems_count();}
    size_type max_size() const {return rep.max_size();}
    bool empty() const {return rep.empty();}
    
    void swap(unordered_mulitmap& rhs) {
        rep.swap(rhs.rep);
    }
    
    iterator begin() {return rep.begin();}
    iterator end() {return rep.end();}

public:
    iterator insert(const value_type& obj) 
    {
        return rep.insert_equal(obj);
    }

    iterator insert_noresize(const value_type& obj)
    {
        return rep.insert_equal_noresize(obj);
    }

    iterator find(const key_type& key) {
        return rep.find(key);
    }
    
    pair<iterator, iterator> equal_range(const key_type& key) {
        return rep.equal_range(key);
    }

    size_type count(const key_type& key) {
        return rep.count(key);
    }
    
    size_type erase(const key_type& key) {
        return rep.erase(key);
    }

    void erase(iterator& it) {
        rep.erase(it);
        return ;
    }
    void clear() {
        return rep.clear();
    }
public:
    void resize(size_type n) {rep.resize(n);}
    size_type bucket_count() {
        return rep.bucket_count();
    }
    size_type max_bucket_count() {
        return rep.max_bucket_count();
    }
    
    size_type elems_in_bucket(size_type n){
        return rep.elems_in_bucket(n);
    }
};


_MY_NAMESPACE_END

#endif
