/*************************************************************************
	> File Name: my_iterator.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
 ************************************************************************/

#ifndef _MY_ITERATOR_H
#define _MY_ITERATOR_H
//迭代器标签, 方便重载;
struct input_iterator_tag {};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category, class T, class Distance = ptrdiff_t,
    class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};

//萃取姬, 萃取iterator中的类型;
template <class iterator>
struct iterator_traits {
    typedef typename iterator::iterator_category  iterator_category;  
    typedef typename iterator::value_type         value_type;
    typedef typename iterator::difference_type    difference_type;
    typedef typename iterator::pointer            pointer;
    typedef typename iterator::reference          reference;
};

//针对指针类型设计偏特化版本;
template<class T>
struct iterator_traits<T*>{
    typedef random_access_iterator_tag  iterator_category;  
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

template<class T>
struct iterator_traits<const T*>{
    typedef random_access_iterator_tag  iterator_category;  
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
};


//利用萃取技术,搞事情的几个函数 BEGIN;

template<class IT_TYPE>
inline 
typename iterator_traits<IT_TYPE>::iterator_category 
get_category(const IT_TYPE&) {
    typedef typename iterator_traits<IT_TYPE>::iterator_category iterator_category;
    return iterator_category();
}

template<class IT_TYPE>
inline 
typename iterator_traits<IT_TYPE>::difference_type*
get_difference_type(const IT_TYPE&) {
    typedef typename iterator_traits<IT_TYPE>::difference_type* distance_p;
    return static_cast<distance_p>(0);
}

template<class IT_TYPE>
inline
typename iterator_traits<IT_TYPE>::value_type*
get_value_type(const IT_TYPE&) {
    typedef typename iterator_traits<IT_TYPE>::value_type* value_type_p;
    return static_cast<value_type_p>(0);
}

//萃取技术搞事情函数 END

//获取迭代器的相差距离 BEGIN

//利用 萃取到的iterator_tag标签进行重载
template<class IT_TYPE>
inline
typename iterator_traits<IT_TYPE>::difference_type
__distance(IT_TYPE first, IT_TYPE last, input_iterator_tag){
    typename iterator_traits<IT_TYPE>::difference_type n = 0;
    while (first != last) {
        ++n;
        ++first;
    }
    return n;
}

template<class IT_TYPE>
inline
typename iterator_traits<IT_TYPE>::difference_type
__distance(IT_TYPE first, IT_TYPE last, random_access_iterator_tag){
    typename iterator_traits<IT_TYPE>::difference_type n = last - first;
    return n;
}

template <class IT_TYPE>
inline 
typename iterator_traits<IT_TYPE>::difference_type
distance(IT_TYPE first, IT_TYPE last) {
    typedef typename iterator_traits<IT_TYPE>::iterator_category tag;
    return __distance(first, last, tag());
}

//获取迭代器的相差距离 END 


//迭代器相关的前进函数 BEGIN

template <class IT_TYPE, class DIS>
inline
void __advance(IT_TYPE& first, DIS distance, random_access_iterator_tag) {
    first += distance;
}


template <class IT_TYPE, class DIS>
inline
void __advance(IT_TYPE& first, DIS n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n){
            ++first;
            --n;
        }
    } else{
        while (n){
            --first;
            ++n;
        }
    }
}

template <class IT_TYPE, class DIS>
inline
void __advance(IT_TYPE& first, DIS n, input_iterator_tag) {
    while (n){
        ++first;
        --n;
    }
}


template <class IT_TYPE, class DIS>
inline 
void advance(IT_TYPE& first,DIS distance) {
    __advance(first, distance, get_category(first));
}

//迭代器相关的前进函数 END 


//先将__type_traits补充在该文件 BEGIN
struct __true_type {};
struct __false_type {};

template <class type>
struct __type_traits{
    //为了防范编译器;
    typedef __true_type this_dummy_member_must_be_first;
        
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};

//开始实现特化版, 模板参数无视CV限定;
template<>
struct __type_traits<char>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<signed char>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<short>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned short>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<int>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned int>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned long>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<float>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<double>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long double>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<class T>
struct __type_traits<T*>{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};
//先将__type_traits补充在该文件 END


//后面到时再补充
#endif
