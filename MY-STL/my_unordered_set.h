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

template <class Value, 
         class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value> >
class unordered_set  {
private:
public:
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

public:
    unordered_set() 
        : rep(10, hasher(), key_equal()) {}

};






_MY_NAMESPACE_END 

#endif
