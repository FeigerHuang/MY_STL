/*************************************************************************
	> File Name: my_algorithm.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Tue 17 Aug 2021 05:20:46 PM CST
 ************************************************************************/

#ifndef _MY_ALGORITHM_H
#define _MY_ALGORITHM_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

#ifdef BUG_ALG
#define LOG_ALGO(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_ALGO(msg)
#endif

_MY_NAMESPACE_BEGIN

//max & min BEGIN
template <class T>
inline
const T& max(const T& x, const T& y) {
    return y < x ? x : y;
}

template<class T, class Cmp>
inline
const T& max(const T& x, const T& y, Cmp cmp) {
    return cmp(y, x) ? x : y;
}

template<class T>
inline 
const T& min(const T& x, const T& y) {
    return y < x ? y : x;
}

template<class T, class Cmp>
inline 
const T& min(const T& x, const T& y, Cmp cmp) {
    return (y, x) ? y : x;
}

//max & min END

// lower_bound & upper_bound BEGIN
template <class ForwardIterator, class T, class Distance>
inline 
ForwardIterator __lower_bound(ForwardIterator first,ForwardIterator last, const T& val, Distance *)
{
    Distance len = 0;
    len = distance(first, last);
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        
        if (*middle < val) {
            first = middle;
            ++first;
            len = len - half - 1;
        } 
        else{
            len = half;
        }
    }
    return first;
}

template <class ForwardIterator, class T>
inline 
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,const T& val) {
    return __lower_bound(first, last, val, get_difference_type(first));
}


template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                             const T& val, Distance *)
{
    Distance len;
    len = distance(first, last);
    ForwardIterator middle;
    Distance half;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        
        if (val < *middle) {
            len = half;
        }
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template <class ForwardIterator, class T>
inline 
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val) {
    return __upper_bound(first, last, val, get_difference_type(first));
}

// lower_bound & upper_bound 的重载版本; 
template <class ForwardIterator, class T, class Distance, class Compare>
inline 
ForwardIterator __lower_bound(ForwardIterator first,ForwardIterator last,
                              const T& val, Distance *,const Compare& cmp)
{
    Distance len = 0;
    len = distance(first, last);
    Distance half;
    ForwardIterator middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        
        if (cmp(*middle ,val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        } 
        else{
            len = half;
        }
    }
    return first;
}

template <class ForwardIterator, class T, class Compare>
inline 
ForwardIterator lower_bound(ForwardIterator first, 
                            ForwardIterator last,
                            const T& val,const Compare& cmp) {
    return __lower_bound(first, last, val, get_difference_type(first), cmp);
}


template <class ForwardIterator, class T, class Distance, class Compare>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                             const T& val, Distance *,const Compare& cmp)
{
    Distance len;
    len = distance(first, last);
    ForwardIterator middle;
    Distance half;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        
        if (cmp(val, *middle)) {
            len = half;
        }
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template <class ForwardIterator, class T, class Compare>
inline 
ForwardIterator upper_bound(ForwardIterator first, 
                            ForwardIterator last, 
                            const T& val, const Compare& cmp) {
    return __upper_bound(first, last, val, get_difference_type(first), cmp);
}

// lower_bound & upper_bound END

// binary_search 算法 BEGIN
template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
    ForwardIterator i = lower_bound(first, last, value);
    return i != last && !(value < *i);
}

template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, const Compare& cmp) {
    ForwardIterator i = lower_bound(first, last, value, cmp);
    return i != last && !cmp(value, *i);
}

// binary_search 算法 END;

// sort 算法 BEGIN

// sort 要用到的一些辅助算法;
template<class T>
inline
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}


template<class T>
inline 
const T& __mid_of_tree(const T& a,const T& b,const T& c) {
    if (a < b) {
        if (b < c) return b;
        else { //  c <= b;
            if (c < a) return a;
            else return c; 
        }
    } 
    else { // b < a;
        if (a < c) return a;
        else { //    b < a  ; c <= a;
            if (c < b) return b;
            else return c;
        }
    }
}

template<class T, class Compare>
inline 
const T& __mid_of_tree(const T& a,const T& b,const T& c, Compare cmp) {
    if (cmp(a , b)) {
        if (cmp(b, c)) return b;
        else { //  c <= b;
            if (cmp(c, a)) return a;
            else return c; 
        }
    } 
    else { // b < a;
        if (cmp(a, c)) return a;
        else { //    b < a  ; c <= a;
            if (cmp(c, b)) return b;
            else return c;
        }
    }
}

//insertion_sort 算法
static const int __threshold = 16;

template<class RandomAccessIterator, class T>
inline 
void __unguarded_linear_insert(RandomAccessIterator last, T value) {
    RandomAccessIterator next = last;
    --next;
    while (value < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template<class RandomAccessIterator, class T>
inline
void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    T value = *last;
    if (value < *first) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else{
        __unguarded_linear_insert(last, value);
    }
}

template<class RandomAccessIterator>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
    if (first == last) return;
    for (RandomAccessIterator i = first + 1; i != last; ++i) {
        __linear_insert(first, i, get_value_type(first));
    }
}


template<class RandomAccessIterator, class T>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, 
                                   RandomAccessIterator last, T*)
{
    for (RandomAccessIterator i = first; i != last; ++i) {
        __unguarded_linear_insert(i, T(*i));
    }
}

template<class RandomAccessIterator>
inline 
void __unguarded_insertion_sort(RandomAccessIterator first,
                                 RandomAccessIterator last) 
{
    __unguarded_insertion_sort_aux(first, last, get_value_type(first));
}

template<class RandomAccessIterator>
void __finial_insertion_sort(RandomAccessIterator first,
                             RandomAccessIterator last) 
{   // 待排序元素大于16时, 对第一区间正常插排;
    if (last - first > __threshold) {
        insertion_sort(first, first + __threshold);
        //后面的区间可以使用无监督的插排;
        __unguarded_insertion_sort(first + __threshold, last);
    } else{
        insertion_sort(first, last);
    }
}

//无监督快排 算法
template<class RandomAccessIterator>
void quick_sort(RandomAccessIterator first,RandomAccessIterator last, size_t segment = 1)
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    while (last - first > segment) {
        RandomAccessIterator low = first, high = last - 1;
        T  pivot = __mid_of_tree(*low, *high, *(low + (high - low)/2));
        do {
            while(*low < pivot) ++low;
            while(pivot < *high) --high;
            if (low <= high) {
                swap(*low, *high);
                ++low, --high;
            }
        } while (low <= high);
        quick_sort(low, last, segment);
        last = high + 1;
    }
}

template <class RandomAccessIterator>
inline  // sort 对外的接口
void sort(RandomAccessIterator first, RandomAccessIterator last) {
    if (first != last) {
        quick_sort(first, last , __threshold);
        __finial_insertion_sort(first, last);
    } 
}

// sort & quick_sort & insertion_sort 算法的重载版本, 支持自定义比较

//insertion_sort 算法

template<class RandomAccessIterator, class T, class Compare>
inline 
void __unguarded_linear_insert(RandomAccessIterator last, T value, Compare cmp) {
    RandomAccessIterator next = last;
    --next; // 没有监督项, 就是这么优秀;
    while (cmp(value,*next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template<class RandomAccessIterator, class T, class Compare>
inline
void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*, Compare cmp)
{
    T value = *last;
    if (cmp(value,*first)) {
        //待比较元素小于首位, 直接 copy_backward
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else{
        // 否说明第一位小于当前, 直接 无监督插排;
        __unguarded_linear_insert(last, value, cmp);
    }
}

template<class RandomAccessIterator, class Compare>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
    if (first == last) return;
    for (RandomAccessIterator i = first + 1; i != last; ++i) {
        __linear_insert(first, i, get_value_type(first), cmp);
    }
}


template<class RandomAccessIterator, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, 
                                   RandomAccessIterator last, T*, Compare cmp)
{
    for (RandomAccessIterator i = first; i != last; ++i) {
        __unguarded_linear_insert(i, T(*i), cmp);
    }
}

template<class RandomAccessIterator, class Compare>
inline 
void __unguarded_insertion_sort(RandomAccessIterator first,
                                 RandomAccessIterator last, Compare cmp) 
{    // 再封装一层, 利用萃取加模板 获得迭代器所指元素类型;
    __unguarded_insertion_sort_aux(first, last, get_value_type(first), cmp);
}

template<class RandomAccessIterator, class Compare>
void __finial_insertion_sort(RandomAccessIterator first,
                             RandomAccessIterator last,Compare cmp) 
{   // 待排序元素大于16时, 对第一区间正常插排;
    if (last - first > __threshold) {
        insertion_sort(first, first + __threshold, cmp);
        //后面的区间可以使用无监督的插排;
        __unguarded_insertion_sort(first + __threshold, last, cmp);
    } else{
        insertion_sort(first, last, cmp);
    }
}

//无监督快排 算法
template<class RandomAccessIterator, class Compare>
void quick_sort(RandomAccessIterator first,RandomAccessIterator last, Compare cmp,  size_t segment = 1)
{
    //利用榨汁姬萃取 迭代器所指类型;
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    //partialtion保证, pivot前面的小于pivot后面的;
    while (last - first > segment) {
        RandomAccessIterator low = first, high = last - 1;
        T  pivot = __mid_of_tree(*low, *high, *(low + (high - low)/2), cmp);
        do {
            while(cmp(*low ,pivot)) ++low;
            while(cmp(pivot ,*high)) --high;
            if (low <= high) {
                // 找到 违反顺序的元素进行交换;
                swap(*low, *high);
                ++low, --high;
            }
        } while (low <= high);
        //单边递归,提速;
        quick_sort(low, last,cmp, segment);
        last = high + 1;
    }
}

template <class RandomAccessIterator, class Compare>
inline  // sort 对外的接口
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
    if (first != last) {
        // 首先进行partialtion操作,当区间长度小于16进行, 插排
        quick_sort(first, last , cmp,  __threshold);
        __finial_insertion_sort(first, last, cmp);
    } 
}


// sort 算法 END


_MY_NAMESPACE_END
#undef LOG_ALGO
#endif
