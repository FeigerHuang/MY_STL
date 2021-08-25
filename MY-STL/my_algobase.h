/*************************************************************************
	> File Name: my_algobase.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Fri 13 Aug 2021 09:09:41 PM CST
 ************************************************************************/

#ifndef _MY_ALGOBASE_H
#define _MY_ALGOBASE_H
#include <string.h>
#include "my_iterator.h"
#include "my_construct.h"
#ifdef BUG_ALG
#define LOG_ALG(msg) { std::cerr << msg << std::endl;}
#else
#define LOG_ALG(msg)
#endif
#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

//_M_fill & _M_fill_n BEGIN

template <class ForwardIterator, class T>
ForwardIterator _M_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for ( ;first != last; ++first) {
        *first = value;
    }
    return first;
}

template <class ForwardIterator,class Size, class T>
ForwardIterator _M_fill_n(ForwardIterator first, Size n, const T& value) {
    for ( ;n > 0; ++first, --n) {
        *first = value;
    }
    return first;
}

//fill & fill_n END 

// copy BEIGN

template <class RandomIterator , class OutputIterator, class Distance>
inline 
OutputIterator __copy_d(RandomIterator first, RandomIterator last, OutputIterator result, Distance *)
{
    LOG_ALG("access __copy_d RandomIterator & has_trivial_assignment_operator");
    for(Distance n = last - first; n > 0; --n, ++result, ++ first) {
        *result = *first; //调用赋值运算符; 
    }
    return result;
}

//当指针所指对象有trivial_assigiment
template <class T>
inline 
T* __copy_t(const T* first, const T* last, T* result, __true_type) {
    LOG_ALG("access __copy_t judge has_trivial_assignment_operator YES");
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template <class T>
inline 
T* __copy_t(const T* first, const T* last, T* result, __false_type) 
{
    LOG_ALG("access __copy_t judge has_trivial_assignment_operator NO");
    return __copy_d(first, last, result, (ptrdiff_t*)(0));
}

//__copy 根据 __type_traits获得的迭代器的标签进行重载;

template <class InputIterator, class OutputIterator>
inline 
OutputIterator __copy(InputIterator first, InputIterator last, 
                      OutputIterator result, input_iterator_tag)
{
    LOG_ALG("access __copy tag = input_iterator_tag");
    for (; first != last; ++result, ++first) 
    {
        *result = *first;
    }
    return result;
}

template <class RandomIterator, class OutputIterator>
inline 
OutputIterator __copy(RandomIterator first, RandomIterator last, 
                      OutputIterator result, random_access_iterator_tag)
{
    LOG_ALG("access __copy tag = random_access_iterator_tag");
    //继续套娃;
    return __copy_d(first, last, result, get_difference_type(first));
}


template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
    {
        LOG_ALG("access struct __copy_dispatch");
        return __copy(first, last, result, get_category(first));
    }
};

// 可调用对象的偏特化;
template <class T>
struct __copy_dispatch<T*, T*>
{
    T* operator()(T* first,T* last, T* result) {
        LOG_ALG("access structi __copy_dispatch<T*,T*>");
        typedef typename __type_traits<T>::has_trivial_assignment_operator tag;
        return __copy_t(first, last, result, tag());
    }
};

//copy的 对外接口;
template <class InputIterator, class OutputIterator>
inline 
OutputIterator copy(InputIterator first, InputIterator last, 
                   OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//针对 char* 实现的函数重载;
inline
char* copy(const char* first, const char* last, char* result)
{
    LOG_ALG("char* copy done!");
    memmove(result, first, (last - first));
    return result + (last - first);
}
// copy END

//copy_backward BEGIN

template <class RandomIterator , class OutputIterator, class Distance>
inline 
OutputIterator __copy_backward_d(RandomIterator first, RandomIterator last, OutputIterator result, Distance *)
{
    LOG_ALG("access __copy_backward_d RandomIterator & has_trivial_assignment_operator");
    for(Distance n = last - first; n > 0; --n) {
        *--result = *--last; //调用赋值运算符; 
    }
    return result;
}

template <class T>
inline 
T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
    const ptrdiff_t  NUM = last - first;
    LOG_ALG("access T* __copy_backward_t has_trivial_assignment_operator true");
    memmove(result - NUM, first, sizeof(T) * (last - first));
    return result - NUM;
}

template <class T>
inline 
T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
    LOG_ALG("access T* __copy_backward_t has_trivial_assignment_operator false");
    return __copy_backward_d(first, last, result, (ptrdiff_t*)(0));
}


template <class InputIterator, class OutputIterator>
inline 
OutputIterator __copy_backward(InputIterator first, InputIterator last, 
                      OutputIterator result, bidirectional_iterator_tag)
{
    LOG_ALG("access __copy_backward tag = bidirectional_iterator_tag");
    while (last != first) 
    {
        *--result = *--last;
    }
    return result;
}

template <class RandomIterator, class OutputIterator>
inline 
OutputIterator __copy_backward(RandomIterator first, RandomIterator last, 
                      OutputIterator result, random_access_iterator_tag)
{
    LOG_ALG("access __copy_backward tag = random_access_iterator_tag");
    //如果是randome_iterator_tag继续套娃;
    return __copy_backward_d(first, last, result, get_difference_type(first));
}

template <class InputIterator,class OutputIterator>
struct __copy_backward_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __copy_backward(first, last, result, get_category(first));
    }
};


template <class T>
struct __copy_backward_dispatch<T *, T*>
{
   T*  operator()(const T* first, const T* last, T* result) 
   {
        typedef typename __type_traits<T>::has_trivial_assignment_operator tag;
        return __copy_backward_t(first, last, result, tag());
   }
};

//copy_backward 的对外接口
template <class InputIterator, class OutputIterator>
inline
OutputIterator copy_backward(InputIterator first, InputIterator last,
                            OutputIterator result)
{
    return __copy_backward_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//针对char *实现函数重载
inline 
char *copy_backward(const char *first,const char *last, char *result)
{
    LOG_ALG("char * copy_backward done!");
    const ptrdiff_t NUM = last - first;
    memmove(result - NUM, first, (last - first));
    return result - NUM;
}

//copy_backward END

// uninitialized_fill_n 实现 BEGIN

template <class ForwardIterator, class Size, class T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
    ForwardIterator cur = first;
    LOG_ALG("uninitialized_fill_n judge is_POD = __false_type");
    for(; n > 0;--n, ++cur) {
        construct(&*cur, x);
    }
    LOG_ALG("construct " << n << " objects finish");
    return cur;
}

template <class ForwardIterator, class Size, class T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
    LOG_ALG("uninitialized_fill_n judge is_POD = __true_type");
    return _M_fill_n(first, n, x);
}

template <class ForwardIterator, class Size, class T, class T1>
inline
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) 
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIterator , class Size, class T>
inline 
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, get_value_type(first));
}

// uninitialized_fill_n 实现 END 

// uninitialized_fill 实现 BEGIN

template <class ForwardIterator, class T>
ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
    ForwardIterator cur = first;
    LOG_ALG("uninitialized_fill judge is_POD = __false_type");
    for(; cur != last; ++cur) {
        construct(&*cur, x);
    }
    return cur;
}

template <class ForwardIterator, class T>
ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
    LOG_ALG("uninitialized_fill judge is_POD = __true_type");
    return _M_fill(first, last, x);
}

template <class ForwardIterator, class T, class T1>
inline
ForwardIterator __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) 
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T>
inline 
ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    return __uninitialized_fill(first, last, x, get_value_type(first));
}

// uninitialized_fill 实现 END 

_MY_NAMESPACE_END

#undef LOG_ALG
#endif
