/*************************************************************************
	> File Name: my_map.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Fri 03 Sep 2021 04:39:24 PM CST
 ************************************************************************/

#ifndef _MY_MAP_H
#define _MY_MAP_H

#include "my_function.h"
#include "my_rbtree.h"

_MY_NAMESPACE_BEGIN

// unordered_set 类的封装;
template <class Key, class T,
         class EqualKey = equal_to<Key> >
class map  {
public:           
    typedef Key                        key_type;
    typedef T                          data_type;
    typedef pair<const Key, T>         value_type;
    typedef EqualKey                   key_equal;
    
private:
    typedef RB_Tree<Key, pair<const Key, T>, select1st<value_type>, EqualKey> tree;
    tree rep;
public:
    typedef typename tree::size_type          size_type;
    typedef typename tree::difference_type    difference_type;
    typedef typename tree::const_pointer      const_pointer;
    typedef typename tree::pointer            pointer;
    typedef typename tree::reference          reference;
    typedef typename tree::const_reference    const_reference;
    typedef typename tree::iterator           iterator;
public:
    map() : rep(key_equal()) {}
    explicit map(const key_equal& cmp) : rep(cmp) {}
    iterator begin() {return rep.begin();}
    iterator end() {return rep.end();} 
    bool empty() const {return rep.empty();}
    void clear() {rep.clear();}
    void insert(const value_type& x) {rep.insert_unique(x);}
    void range() {rep.inorder();}
    bool find(const value_type& x) {return rep.find(x);}
    size_type size() {return rep.size();}
    void erase(const value_type& x) {rep.erase_unique(x);}
    value_type& operator[](const key_type& x) {
        return rep.find_or_insert(x);
    }
};

_MY_NAMESPACE_END 
#endif
