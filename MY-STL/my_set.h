/*************************************************************************
	> File Name: my_set.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 01 Sep 2021 09:13:50 PM CST
 ************************************************************************/

#ifndef _MY_SET_H
#define _MY_SET_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }
#include "my_function.h"
#include "my_rbtree.h"

_MY_NAMESPACE_BEGIN

// unordered_set 类的封装;
template <class Value, 
         class EqualKey = equal_to<Value> >
class set  {
private:
    typedef RB_Tree<Value, Value, identify<Value>, EqualKey> tree;
    tree rep;
public:           
    typedef typename tree::key_type   key_type;
    typedef typename tree::value_type value_type;
    typedef typename tree::key_equal  key_equal;
    
    typedef typename tree::size_type          size_type;
    typedef typename tree::difference_type    difference_type;
    typedef typename tree::const_pointer      const_pointer;
    typedef typename tree::pointer            pointer;
    typedef typename tree::reference          reference;
    typedef typename tree::const_reference    const_reference;
    typedef typename tree::iterator           iterator;
public:
    set() : rep(key_equal()) {}
    explicit set(const key_equal& cmp) : rep(cmp) {}
    
    iterator begin() {return rep.begin();}
    iterator end() {return rep.end();}
    bool empty() const {return rep.empty();}
    void clear() {rep.clear();}
    void insert(const value_type& x) {rep.insert_unique(x);}
    void range() {rep.inorder();}
    bool find(const value_type& x) {return rep.find(x);}
    size_type size() {return rep.size();}
    void erase(const value_type& x) {rep.erase_unique(x);}
};


_MY_NAMESPACE_END  
#endif
