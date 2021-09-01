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
#include "my_rbtree.h.h"

_MY_NAMESPACE_BEGIN

// unordered_set 类的封装;
template <class Key, 
         class EqualKey = equal_to<Key> >
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


};



_MY_NAMESPACE_END  
#endif
