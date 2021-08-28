/*************************************************************************
	> File Name: my_stack.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Sat 28 Aug 2021 05:31:06 PM CST
 ************************************************************************/

#ifndef _MY_STACK_H
#define _MY_STACK_H
#include "my_deque.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// 适配器(配接器) queue的实现
template <class T, class Sequence = deque<T> >
class stack {
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
    reference top() {return que.back();}
    const_reference top() const {return que.back();}
    void push(const value_type& x) {
        que.push_back(x);
        return ;
    }
    void pop() {
        que.pop_back();
        return;
    }
};

_MY_NAMESPACE_END

#endif
