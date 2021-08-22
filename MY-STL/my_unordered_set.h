/*************************************************************************
	> File Name: my_unordered_set.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 18 Aug 2021 08:07:33 PM CST
 ************************************************************************/

#ifndef _MY_UNORDERED_SET_H
#define _MY_UNORDERED_SET_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }
#include "my_function.h"
#include "my_hash_fun.h"
#include "my_hashtable.h"

_MY_NAMESPACE_BEGIN

// unordered_set 类的封装;
template <class Value, 
         class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value> >
class unordered_set  {
private:
    typedef hashtable<Value, Value, HashFcn, identify<Value>, EqualKey> ht;
    ht rep;
public:           
    typedef typename ht::key_type   key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher     hasher;
    typedef typename ht::key_equal  key_equal;
    
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::pointer            pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;

    typedef typename ht::iterator           iterator;

    hasher hash_funct() const {return rep.hash_funct();}
    key_equal key_eq() const {return rep.key_eq();}
public:
    unordered_set() 
        : rep(10, hasher(), key_equal()) {}
    explicit unordered_set(size_type n) 
        : rep(n, hasher(), key_equal()) {}
    unordered_set(size_type n,const HashFcn& hf)
        : rep(n, hf, key_equal()) {}
    unordered_set(size_type n, const HashFcn& hf, const EqualKey& eql)
        : rep(n, hf, eql) {}
public:
    size_type size() const {return rep.elems_count();}
    size_type max_size() const {return rep.max_size();}
    bool empty() const {return rep.empty();}
    
    iterator begin() {return rep.begin();}
    iterator end()  {return rep.end();}
    void swap(unordered_set& rhs) {rep.swap(rhs.rep);}
public:
    pair<iterator, bool> insert(const value_type& obj) {
        return rep.insert_unique(obj);
    }
    
    iterator insert_noresize(const value_type& obj) {
        return rep.insert_unique_noresize(obj);
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

    void erase(iterator it) {
        rep.erase(it);
        return;
    }
    void clear() { rep.clear();}

    size_type bucket_count() {
        return rep.bucket_count();
    }
    
    size_type max_bucket_count() {
        return rep.max_bucket_count();
    }
    
    size_type elems_in_bucket(size_type n){
        return rep.elems_in_bucket(n);
    }
    
    void resize(size_type n) {
        rep.resize(n);
    }
};


// unordered_mulitset 类的封装;
template <class Value, 
         class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value> >
class unordered_mulitset  {
private:
    typedef hashtable<Value, Value, HashFcn, identify<Value>, EqualKey> ht;
    ht rep;
public:           
    typedef typename ht::key_type   key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher     hasher;
    typedef typename ht::key_equal  key_equal;
    
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::pointer            pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;

    typedef typename ht::iterator           iterator;

    hasher hash_funct() const {return rep.hash_funct();}
    key_equal key_eq() const {return rep.key_eq();}
public:
    unordered_mulitset() : rep(100, hasher(), key_equal()) {}
    explicit unordered_mulitset(size_type n) 
        : rep(n, hasher(), key_equal()) {}
    unordered_mulitset(size_type n,const HashFcn& hf)
        : rep(n, hf, key_equal()) {}
    unordered_mulitset(size_type n, const HashFcn& hf, const EqualKey& eql)
        : rep(n, hf, eql) {}

public:
    size_type size() const {return rep.elems_count();}
    size_type max_size() const {return rep.max_size();}
    bool empty() const {return rep.empty();}
    
    iterator begin() {return rep.begin();}
    iterator end()  {return rep.end();}
    void swap(unordered_mulitset& rhs) {rep.swap(rhs.rep);}
public:
    iterator insert(const value_type& obj) {
        return rep.insert_equal(obj);
    }
    
    iterator insert_noresize(const value_type& obj) {
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

    void erase(iterator it) {
        rep.erase(it);
        return;
    }
    void clear() { rep.clear();}
    
    void resize(size_type n) {
        rep.resize(n);
    }

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
