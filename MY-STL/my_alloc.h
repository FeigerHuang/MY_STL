/*************************************************************************
	> File Name: my_alloc.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 11 Aug 2021 09:23:16 PM CST
 ************************************************************************/

#ifndef _MY_ALLOC_H
#define _MY_ALLOC_H

#include <iostream>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef BUG_ALL
#define LOG(msg) {std::cerr << msg << std::endl;}
#else
#define LOG(msg)
#endif

//一级配置器的实现 BEGIN
template <class T>
inline 
T* allocate(ptrdiff_t size, T*) {
    //set_new_handler(0);
    T *tmp = (T *)(::operator new((size_t)(size *sizeof(T))));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    //LOG("allocate done");
    return tmp;
}

template <class T>
inline 
void deallocate(T *buffer) {
    ::operator delete(buffer);
    //LOG("deallocate done");
}

//一级配置器的实现 END

template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    static pointer allocate(size_type n) {
        pointer result = static_cast<pointer>(malloc(n));
        if (0 == result) {
            std::cerr << "malloc out of memory" << std::endl;
        }
        LOG("malloc address = " << result );
        return result;
    }

    static void deallocate(pointer p, size_t) {
        free(p);
    }

    static pointer address(reference x) {return (pointer)&x;}

    static const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    static size_type init_page_size() {
        return std::max(size_type(1), size_type(4096 / sizeof(T)) );
    }

    static size_type max_size() {
        return std::max(size_type(1), size_type(UINT_MAX / sizeof(T)) );
    }
};

template<>
class allocator<void> {
public:
    typedef void *pointer;
};

//一级配置器的实现 END 

// 封装空间配置器的对外接口 BEGIN
// 以便日后实现二级配置器进行封装

template<class T, class Alloc = allocator<T> >
class simple_alloc{
public:
    static T *allocate(size_t n) {
        return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
    }
    
    static T *allocate(void ){
        return (T*)Alloc::allocate(sizeof(T));
    }
    
    static void deallocate(T* p, size_t n) {
        if (0 == n) return ;
        Alloc::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T* p) {
        Alloc::deallocate(p, sizeof(T));
    }
};
// 封装空间配置器的对外接口 END

#undef LOG

#endif
