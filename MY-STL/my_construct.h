/*************************************************************************
	> File Name: my_construct.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
 ************************************************************************/

#ifdef BUG
#define LOG_CON(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_CON(msg)
#endif

#ifndef _MY_CONSTRUCT_H
#define _MY_CONSTRUCT_H

#include "my_iterator.h"

//负责对象的构造与析构
template <class T1, class T2>
inline 
void construct(T1* p,const T2& value) {
    new(p) T1(value);
    LOG_CON("in address = "<< p <<" :construct done!");
}


template <class T>
inline
void destroy(T* pointer) {
    pointer->~T();
}

//destroy 各类型重载 BEGIN
template <class IT_TYPE>
inline 
void __destroy_aux(IT_TYPE first, IT_TYPE last, __false_type) {
    LOG_CON("__destroy_aux(false_type)");
    for (; first < last; ++first) {
        destroy(&*first);
    }
}

template <class IT_TYPE>
inline 
void __destroy_aux(IT_TYPE first, IT_TYPE last, __true_type) {
    LOG_CON("__destroy_aux(true_type)");
}

template <class IT_TYPE, class T>
inline 
void __destroy(IT_TYPE first, IT_TYPE last, T*){
    typedef typename __type_traits<T>::has_trivial_destructor tag;
    __destroy_aux(first, last, tag());
}

template <class IT_TYPE>
inline void destroy(IT_TYPE first, IT_TYPE last) {
    __destroy(first, last, get_value_type(first));
}

//destroy 各类型重载 END 


#undef LOG_CON

#endif
