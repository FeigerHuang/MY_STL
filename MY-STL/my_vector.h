/*************************************************************************
	> File Name: my_vector.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Sat 14 Aug 2021 02:22:28 PM CST
 ************************************************************************/
#ifndef _MY_VECTOR_H
#define _MY_VECTOR_H

#ifdef BUG_VEC
#define LOG_VEC(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_VEC(msg)
#endif
#include "my_algobase.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN
//vector 实现BEGIN
template <class T, class Alloc = allocator<T> >
class vector {
public:
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator;
    typedef value_type&     reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

protected:
    typedef simple_alloc<T> data_allocatetor;
    
    iterator start;
    iterator finish;
    iterator end_of_storage;
    
    void insert_aux(iterator pos, const T& x);
    
    //分配一段空间并且完成初始化;
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    void deallocate() {
        if (start) {
            data_allocatetor::deallocate(start, end_of_storage - start);
        }
    }

    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocatetor::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
public:
    iterator begin() {return start;}
    iterator end()   {return finish;}
    size_type size() {return size_type(end() - begin());}
    size_type capacity() {return size_type(end_of_storage - start);}
    reference front() {return *begin();}
    reference back() {return *(end() - 1);}
    bool empty() {return begin() == end();}
    reference operator[](size_type n) {return *(begin() + n);}

    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value) {fill_initialize(n, value);}
    vector(int n, const T& value) {fill_initialize(n, value);}
    vector(long n, const T& value){fill_initialize(n, value);}
    explicit vector(size_type n) {fill_initialize(n, T());}
    
    ~vector(){
        destroy(start, finish);
        deallocate();
    }
    
    void push_back(const T&);
    void pop_back();
    iterator erase(iterator pos);
};

template <class T, class Alloc>
void vector<T, Alloc>::pop_back() {
    --finish;
    destroy(finish);
    return;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
    if (pos != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        copy_backward(pos, finish - 2, finish - 1);
        *pos = x;
    }
    else{
        const size_type old_size = size();
        const size_type len = old_size != 0 ? old_size * 2 : 1;

        iterator new_start = data_allocatetor::allocate(len);
        iterator new_finish = new_start;
        try {
            new_finish = copy(start, pos, new_start);
            construct(new_finish, x);
            ++new_finish;
            new_finish = copy(pos, finish, new_finish);
        } 
        catch(...) {
            destroy(new_start, new_finish);
            data_allocatetor::deallocate(new_start, len);
            throw;
        }
        destroy(begin(), end());
        deallocate();
        
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    } 
}

template<class T, class Alloc>
void vector<T, Alloc>::push_back(const T& x) {
    if (finish != end_of_storage) {
        construct(finish, x);
        ++finish;
    } else {
        insert_aux(end(), x);
    }
}

template<class T, class Alloc>
typename vector<T,Alloc>::iterator 
vector<T, Alloc>::erase(iterator pos) {
    if (pos + 1 != end()) {
        copy(pos + 1, finish, pos);
        --finish;
    } 
    destroy(finish);
    return pos;
}

//vector 实现 END
_MY_NAMESPACE_END


#undef LOG_VEC
#endif
