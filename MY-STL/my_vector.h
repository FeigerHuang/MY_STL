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
#include "my_algorithm.h"

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
    typedef const T               const_value_type;
    typedef const value_type*     const_pointer;
    typedef const value_type*     const_iterator;
    typedef const value_type&     const_reference;

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
    iterator begin() const {return start;}
    iterator end()  const  {return finish;}
    const_iterator cbegin() const {return start;}
    const_iterator cend()  const  {return finish;}
    size_type size() const {return size_type(end() - begin());}
    size_type capacity() const {return size_type(end_of_storage - start);}
    reference front() {return *begin();}
    reference back() {return *(end() - 1);}
    bool empty() {return begin() == end();}
    reference operator[](size_type n) {return *(begin() + n);}
    const_reference operator[](size_type n) const {return *(cbegin() + n);}
    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value) {fill_initialize(n, value);}
    vector(int n, const T& value) {fill_initialize(n, value);}
    vector(long n, const T& value){fill_initialize(n, value);}
    explicit vector(size_type n) {fill_initialize(n, T());}
    
    vector(const vector& rhs) {
        LOG_VEC("vector constructor call");
        size_type n = rhs.capacity();
        start = data_allocatetor::allocate(n);
        int i = 0;
        for (; i < rhs.size(); ++i) {
            construct(start + i, rhs[i]);
        }
        finish = start + i;
        end_of_storage = start + n;
    }
    
    vector(vector&& rhs) noexcept 
        :start(nullptr), finish(nullptr), end_of_storage(nullptr) 
    {
        LOG_VEC("vector move constructor call");
        this->swap(rhs);
    }

    vector& operator=(vector rhs) noexcept {
        LOG_VEC("vector move assignment operator=() call");
        if(this == &rhs) return *this;
        this->swap(rhs); 
    }

    ~vector(){
        destroy(start, finish);
        deallocate();
    }
    void clear() {erase(begin(), end());}
    void push_back(const T&);
    void pop_back();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void insert(iterator pos, const T& x);
    void insert(iterator pos, size_type n, const T& x);
    void resize(size_type n, const T& x);
    void resize(size_type n);
    
    template<class U,class Ac> 
    void swap(vector<U,Ac>& rhs) {
        using feiger::swap;
        swap(start, rhs.start);
        swap(finish, rhs.finish);
        swap(end_of_storage, rhs.end_of_storage);
        return ;
    }
};

    
template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n, const T& x)
{
    if (n < size()) {
        erase(begin() + n , end());
    }
    else{
        insert(end(), n - size(), x);
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type n) 
{
    resize(n, T());
}


template <class T, class Alloc>
typename vector<T,Alloc>::iterator 
vector<T, Alloc>::erase(iterator first, iterator last) {
    iterator new_finish = first;
    if (last != finish) {
        new_finish = copy(last, finish, first);
    }
    destroy(new_finish, finish);
    return finish = new_finish;
}

template <class T, class Alloc>
void vector<T, Alloc>::pop_back() {
    --finish;
    destroy(finish);
    return;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, const T& x) {
    insert_aux(pos, x);
    return ;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T& x) {
    if (n == 0) return ;
    //当备用空间够用时;
    if (size_type(end_of_storage - finish) >= n) {
        T x_copy = x;
        
        iterator old_finish = finish;
        copy(finish - n, finish, finish);
        finish += n;
        copy_backward(pos, old_finish - n, old_finish);
        _M_fill(pos, pos + n, x_copy);
    } 
    else {
        const size_type old_size = size();
        const size_type len = old_size + std::max(old_size, n);
        
        iterator new_start = data_allocatetor::allocate(len);
        iterator new_finish = new_start;
        
        try{
            new_finish = copy(start, pos, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, x);
            new_finish = copy(pos, finish, new_finish);
        }
        catch(...){
            destroy(new_start, new_finish);
            data_allocatetor::deallocate(new_start, len);
            throw;
        }
        destroy(start, finish);
        deallocate();
        
        start = new_start;
        finish = new_finish;
        end_of_storage =  new_start + len;
    }
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
