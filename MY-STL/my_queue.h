/*************************************************************************
	> File Name: my_queue.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Mon 23 Aug 2021 08:50:06 PM CST
 ************************************************************************/

#ifndef _MY_QUEUE_H
#define _MY_QUEUE_H
#include "my_deque.h"
#include "my_heap.h"
#include "my_vector.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// 适配器(配接器) queue的实现
template <class T, class Sequence = deque<T> >
class queue {
public:
    typedef typename Sequence::value_type       value_type;
    typedef typename Sequence::size_type        size_type;
    typedef typename Sequence::reference        reference;
    typedef typename Sequence::const_reference  const_reference;
protected:
    Sequence  que;
public:
    bool empty() {return que.empty();}
    size_type size() {return que.size();}
    reference front() {return que.front();}
    const_reference front() const {return que.front();}
    reference back() {return que.back();}
    const_reference back() const {return que.back();}
    void push(const value_type& x) {return que.push_back(x);}
    void pop() {return que.pop_front();}

};


// priority_queue container adapter 实现
template <class T, class Sequence = vector<T>,
         class Compare = less<typename Sequence::value_type> >
class priority_queue { 
public:
    typedef typename Sequence::value_type   value_type;
    typedef typename Sequence::size_type    size_type;
    typedef typename Sequence::reference    reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence vec;
    Compare  cmp;
public:
    priority_queue() : vec() {}
    explicit priority_queue(const Compare& cmp) : vec(), cmp(cmp) {}

public:
    bool empty() {return vec.empty();}
    size_type size() const {return vec.size();}
    const_reference top() {return vec.front();}
    void push(const value_type& x) {
        vec.push_back(x);
        push_heap(vec.begin(), vec.end(), cmp);
    }
    void pop() {
        pop_heap(vec.begin(), vec.end(), cmp);
        vec.pop_back();
    }
};


_MY_NAMESPACE_END

#endif
