/*************************************************************************
	> File Name: my_numeric.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Sun 22 Aug 2021 05:09:59 PM CST
 ************************************************************************/

#ifndef _MY_NUMERIC_H
#define _MY_NUMERIC_H

#include "my_iterator.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// accumulate 算法 BEGIN;
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init)
{
    for (; first != last; ++first) {
        init += *first;
    }
    return init;
}

template <class InputIterator, class T, class Option>
T accumulate(InputIterator first, InputIterator last, T init, Option binary_op)
{
    for (; first != last; ++first) {
        init = binary_op(init, *first);
    }
    return init;
}

// accumulate 算法 END;

// adjacent_different 算法 BEGIN
template <class InputIterator, class OutputIterator>
OutputIterator adjacent_different(InputIterator first, InputIterator last,
                                  OutputIterator result)
{
    if (first == last) return result;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type T;
    T value  = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return ++result;
}

template <class InputIterator, class OutputIterator, class Option>
OutputIterator adjacent_different(InputIterator first, InputIterator last,
                                 OutputIterator result, Option binary_op)
{
    if (first == last) return result;
    * result = *first;
    typedef typename iterator_traits<InputIterator>::value_type T;
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = binary_op(tmp, value);
        value = tmp;
    }
    return ++result;
}

// adjacent_different 算法 END

// inner_product 算法 BEGIN
template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2, T init)
{
    for (; first1 != last1; ++first1, ++first2) {
        init += (*first1) * (*first2);
    }
    return init;
}

template<class InputIterator1, class InputIterator2,
        class T, class Option1, class Option2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init, Option1 binary_op1, Option2 binary_op2)
{
    for (; first1 != last1; ++first1, ++first2) {
        init = binary_op1(init, binary_op2(*first1, *first2));
    }
    return init;
}

// inner_product 算法 END 

// partial_sum 算法 BEGIN
template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) 
{
    if (first == last) return result;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type T;
    T value = *first;
    while (++first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

template <class InputIterator, class OutputIterator, class Option>
OutputIterator partial_sum(InputIterator first, InputIterator last, 
                           OutputIterator result, Option binary_op) 
{
    if (first == last) return result;
    *result = *first;
    typedef typename iterator_traits<InputIterator>::value_type T;
    T value = *first;
    while (++first != last) {
        value = binary_op(value, *first);
        *++result = value;
    }
    return ++result;
}

// partial_sum 算法 END

// adjacent_find 算法 BEGIN , 找到第一个相邻相同元素的位置;
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) 
{
    if (first == last) return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (*first == *next) return first;
        first = next;
    }
    return last;
}

template <class ForwardIterator, class Option>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, Option binary_op) 
{
    if (first == last) return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (binary_op(*first, *next) ) return first;
        first = next;
    }
    return last;
}

// adjacent_find 算法 END

// 相关 查找算法 实现
// count && count_if 算法 BEGIN
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type 
count(InputIterator first, InputIterator last, const T& value) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value) ++n;
    }
    return n;
}

template <class InputIterator, class Option>
typename iterator_traits<InputIterator>::difference_type 
count_if(InputIterator first, InputIterator last, const Option& unarry_op) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (unarry_op(*first)) ++n;
    }
    return n;
}

// count && count_if 算法 END

// find && find_if && find_end  算法 BEGIN 

template <class InputIterator , class T>
InputIterator find(InputIterator first, InputIterator last, const T& value)
{
    for (; first != last; ++first) {
        if (*first == value) return first;
    }
    return first;
}

template <class InputIterator, class Option>
InputIterator find_if(InputIterator first, InputIterator last, const Option unarry_op)
{
    for (; first != last; ++first) {
        if (unarry_op(*first)) return first;
    }
    return first;
}

// find && find_if && find_end  算法 END

_MY_NAMESPACE_END

#endif
