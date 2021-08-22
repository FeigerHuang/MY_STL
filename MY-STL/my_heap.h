/*************************************************************************
	> File Name: my_heap.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Sun 22 Aug 2021 07:25:56 PM CST
 ************************************************************************/

#ifndef _MY_HEAP_H
#define _MY_HEAP_H

#include "my_iterator.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// heap 算法 具体实现
// 入堆操作, 默认大顶堆, 要求末尾元素为入堆元素;
template <class RandomIterator, class Distance, class T>
void __push_heap(RandomIterator first, Distance holeIndex, 
                    Distance topIndex, T value)
{
    Distance father = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + father) < value) {
        *(first + holeIndex) = *(first + father);
        holeIndex = father;
        father = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
    return ;
}

template <class RandomIterator, class Distance, class T>
inline 
void __push_heap_aux(RandomIterator first, RandomIterator last, 
                     Distance* ,  T* ) {   

    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));    
}

template <class RandomIterator>
inline 
void push_heap(RandomIterator first, RandomIterator last) {
    __push_heap_aux(first, last, get_difference_type(first) , get_value_type(first));
}

//pop_heap操作, 弹出堆顶元素;
//向下调整;
template <class RandomIterator , class Distance, class T>
void __adjust_heap(RandomIterator first, 
        Distance holeIndex, Distance len, T value ) { 
    Distance father = holeIndex;
    Distance child = 2 * holeIndex + 1;
    while (child < len) {
        if (*(first + child) < *(first + child + 1) && child + 1 < len) {
            ++child;
        }
        if (value < *(first + child)) {
            *(first + father) = *(first + child);
            father = child;
            child = 2 *father + 1;
            continue;
        } 
        break;
    }
    *(first + father) = value;
    return ;
}

template <class RandomIterator , class T, class Distance>
inline 
void __pop_heap(RandomIterator first, RandomIterator last, 
                RandomIterator result, T value, Distance *) 
{
    *result = *first;
    // 将堆顶元素丢末尾, 向下调整; 从0号位开始; value 为最后填坑的值;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomIterator, class T>
inline 
void __pop_heap_aux(RandomIterator first, RandomIterator last,  T* ) { 
    __pop_heap(first, last - 1, last -1, T(*(last - 1)), get_difference_type(first));
}

template <class RandomIterator>
inline 
void pop_heap(RandomIterator first, RandomIterator last) {
    __pop_heap_aux(first, last, get_value_type(first));
}

// sort_heap 算法 实现
template <class RandomIterator>
void sort_heap(RandomIterator first, RandomIterator last) {
    while (last - first > 1) {
        pop_heap(first, last--);
    }
}

// make_heap 算法实现
template<class RandomIterator, class T, class Distance>
void __make_heap(RandomIterator first, RandomIterator last , T*, Distance *)
{
    if (last - first < 2) return;
    Distance len = last - first;
    // 找出最后一个非叶子节点, 进行线性建堆;
    Distance father = (len - 2) / 2;

    for (Distance cur = father; cur >= 0; --cur) {
        __adjust_heap(first, cur, len, T(*(first + cur)));
    }
}

template <class RandomIterator>
inline
void make_heap(RandomIterator first, RandomIterator last) {
    __make_heap(first, last, get_value_type(first), get_difference_type(first));
}

// heap 算法的重载实现, 可自定义比较函数;
// 入堆操作
template <class RandomIterator, class Distance, class T, class Compare>
void __push_heap(RandomIterator first, Distance holeIndex, 
                    Distance topIndex, T value, Compare cmp)
{
    Distance father = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && cmp(*(first + father), value) ) {
        *(first + holeIndex) = *(first + father);
        holeIndex = father;
        father = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
    return ;
}

template <class RandomIterator, class Compare, class Distance, class T>
inline 
void __push_heap_aux(RandomIterator first, RandomIterator last, 
                     Compare cmp, Distance* ,  T* ) {   

    __push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)), cmp);    
}

template <class RandomIterator, class Compare>
inline 
void push_heap(RandomIterator first, RandomIterator last, Compare cmp) {
    __push_heap_aux(first, last, cmp , get_difference_type(first) , get_value_type(first));
}

//pop_heap操作, 弹出堆顶元素;
//向下调整;
template <class RandomIterator , class Distance, class T, class Compare>
void __adjust_heap(RandomIterator first, 
        Distance holeIndex, Distance len, T value , Compare cmp) { 
    Distance father = holeIndex;
    Distance child = 2 * holeIndex + 1;
    while (child < len) {
        if (cmp(*(first + child), *(first + child + 1) ) && child + 1 < len) {
            ++child;
        }
        if (cmp(value ,*(first + child)) ) {
            *(first + father) = *(first + child);
            father = child;
            child = 2 *father + 1;
            continue;
        } 
        break;
    }
    *(first + father) = value;
    return ;
}

template <class RandomIterator , class T, class Distance, class Compare>
inline 
void __pop_heap(RandomIterator first, RandomIterator last, 
                RandomIterator result, T value, Compare cmp, Distance *) 
{
    *result = *first;
    // 将堆顶元素丢末尾, 向下调整; 从0号位开始; value 为最后填坑的值;
    __adjust_heap(first, Distance(0), Distance(last - first), value, cmp);
}

template <class RandomIterator, class T, class Compare>
inline 
void __pop_heap_aux(RandomIterator first, RandomIterator last, Compare cmp,  T* ) { 
    __pop_heap(first, last - 1, last -1, T(*(last - 1)), cmp, get_difference_type(first));
}

template <class RandomIterator, class Compare>
inline 
void pop_heap(RandomIterator first, RandomIterator last, Compare cmp) {
    __pop_heap_aux(first, last, cmp, get_value_type(first));
}

// sort_heap 算法 实现
template <class RandomIterator, class Compare>
void sort_heap(RandomIterator first, RandomIterator last, Compare cmp) {
    while (last - first > 1) {
        pop_heap(first, last--, cmp);
    }
}

// make_heap 算法实现
template<class RandomIterator, class T, class Distance, class Compare>
void __make_heap(RandomIterator first, RandomIterator last , Compare cmp, T*, Distance *)
{
    if (last - first < 2) return;
    Distance len = last - first;
    // 找出最后一个非叶子节点, 进行线性建堆;
    Distance father = (len - 2) / 2;

    for (Distance cur = father; cur >= 0; --cur) {
        __adjust_heap(first, cur, len, T(*(first + cur)), cmp);
    }
}

template <class RandomIterator, class Compare>
inline
void make_heap(RandomIterator first, RandomIterator last, Compare cmp) {
    __make_heap(first, last, cmp, get_value_type(first), get_difference_type(first));
}


_MY_NAMESPACE_END

#endif
