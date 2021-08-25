/*************************************************************************
	> File Name: my_deque.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 25 Aug 2021 04:58:25 PM CST
 ************************************************************************/

#ifndef _MY_DEQUE_H
#define _MY_DEQUE_H

#ifdef BUG_DEQ
#define LOG_DEQ(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_DEQ(msg)
#endif
#include "my_iterator.h"
#include "my_alloc.h"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// 计算buf的数量,buf用于储存数据;
inline 
size_t __deque_buf_size(size_t size) {
    return size < 512 ? size_t(512 / size) : size_t(1);
}

// deque的迭代器设计,尤为重要,
// deque的迭代器可以说是deque的灵魂
template <class T, class Ref, class Ptr>
struct deque_iterator {
    typedef deque_iterator<T, T&, T*> iterator;
    typedef deque_iterator<T, const T&, const T*> const_iterator;
    static size_t buffer_size() {return __deque_buf_size(sizeof(T));}
    
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Ptr                         pointer;
    typedef Ref                         reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef T**                         Map_pointer;
    
    typedef deque_iterator              self;
    
    T*          cur;
    T*          first;
    T*          last;
    Map_pointer M_node; // 指向连续map的node, node->缓存区; 
    // iterator的构造函数
    deque_iterator() : cur(0), first(0), last(0), M_node(0) {}
    deque_iterator(T* x, Map_pointer y)
        : cur(x), first(*y), last(*y + buffer_size()), M_node(y) {}
    deque_iterator(const deque_iterator& rhs) 
        : cur(rhs.cur), first(rhs.first), last(rhs.last), M_node(rhs.M_node) {}
    
    reference operator*() { return *cur;}
    pointer operator->() {return cur;}
    
    difference_type operator-(const self& x) {   // 多借一个buffer的大小, 
        return difference_type(buffer_size()) * (M_node - x.M_node - 1) + 
                (cur - first) + (x.last - x.cur); // 这里还回去;
    }
    
    self& operator++() {
        ++cur;
        // 当指针前进到当前缓存的末尾, 跳到下个缓存的开头;
        if (cur == last) {
            set_node(M_node + 1);
            cur = first;
        }
        return *this;
    }
    self operator++(int) {
        self tmp(*this);
        ++*this;
        return tmp;
    }    
    // 当指针减之前指向缓存开头, 跳到前一个缓存的末尾;
    self& operator--() {
        if (cur == first) {
            set_node(M_node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self operator--(int) {
        self tmp(*this);
        --*this;
        return tmp;
    } 
    
    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        // 即使加上n也在当前缓存块
        if (offset >= 0 && offset < difference_type(buffer_size()) ) {
            cur += n;
        } else {
            difference_type node_offset = offset > 0 ?                    // 向前多移动一格;
                offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size() ) - 1;
            set_node(M_node + node_offset);
                                //如果node_offset 为负, 负负得正;
            cur = first + (offset - node_offset * difference_type(buffer_size()) );
        }
        return *this;
    }
    self operator+(difference_type n) {
        self tmp(*this);
        tmp += n;
        return tmp;
    }
    self& operator-=(difference_type n) {return *this += -n;}
    self operator-(difference_type n) {
        self tmp(*this);
        tmp += -n;
        return tmp;
    }

    reference operator[](difference_type n) {
        return *(*this + n);
    }
    // 重载比较运算符    
    bool operator==(const self& rhs) const {return cur == rhs.cur;}
    bool operator!=(const self& rhs) const {return !(*this == rhs);}
    bool operator<(const self& rhs) const {
        // 先比较节点是否相同, 相同比较节点内的指针;
        return (M_node == rhs.M_node) ? (cur < rhs.cur) : (M_node < rhs.M_node);
    }

    //跳到下一个节点;
    void set_node(Map_pointer new_node) {
        M_node = new_node;
        first = *new_node;
        last = *new_node + difference_type(buffer_size());
    }

};

template <class T, class Alloc = allocator<T> >
class deque {
public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    
public:
    typedef deque_iterator<T,T&,T*>           iterator;
    typedef typename iterator::const_iterator const_iterator;
protected:
    typedef pointer* Map_pointer;
    static size_type buffer_size() {return __deque_buf_size(sizeof(T));}
    
    typedef simple_alloc<value_type>         data_allocatetor;
    typedef simple_alloc<pointer>            map_allocatetor;
    enum {_M_init_map_size = 8} ; // 最少给一张map分配8个节点;
protected:
    iterator    start;
    iterator    finish;
    Map_pointer map;
    size_type   map_size;
//  节点配置与初始化相关函数;  
    pointer allocate_node() {
        return data_allocatetor::allocate(buffer_size());
    }
    void deallocate_node(pointer x) {
        data_allocatetor::deallocate(x);
        LOG_DEQ("node create finish!");
    }
    void fill_initialize(size_type n, const value_type& x); 
    void create_map_and_nodes(size_type num_elements);
    size_type init_map_size() {
        return static_cast<size_type>(_M_init_map_size);
    }
public:
    deque(int n, const value_type& x) 
        : start(), finish(), map(0), map_size(0)
    {
        LOG_DEQ("deque constructor finish!"); 
        fill_initialize(n, x);
    }
    
    size_type size() {return finish - start;}
    size_type max_size() {return size_type(-1);}

    iterator begin() {return start;}
    iterator end() {return finish;}
    reference front() {return *start;}
    reference back() {
        iterator tmp = finish;
        --tmp;
        return *tmp;
    } 
    
    const_reference front() const {return *start;}
    const_reference back() const {
        const_iterator tmp = finish;
        --tmp;
        return *tmp;
    } 

    bool empty() const {return start == finish;}

    void push_back(const value_type& x);
    void push_back_aux(const value_type& x);
    void push_front(const value_type& x);
    void push_front_aux(const value_type& x);
    void pop_back();
    void pop_back_aux();
};

// 从尾部弹出元素;
template <class T, class Alloc>
void deque<T, Alloc>::pop_back() {
    if (finish.cur != finish.first) {
        LOG_DEQ("call pop_back() && cur != first");
        --finish.cur;
        destroy(finish.cur);
    } else {
        pop_back_aux();
    }
}

template <class T, class Alloc>
void deque<T, Alloc>::pop_back_aux() {
    LOG_DEQ("call pop_back_aux()");
    deallocate_node(finish.first);
    finish.set_node(finish.M_node - 1);
    finish.cur = finish.last - 1;
    destroy(finish.cur);
    return ;
}

//从尾部插入元素;
template<class T, class Alloc>
void deque<T, Alloc>::push_back(const value_type& x) {
    if (finish.cur != finish.last - 1) {
        construct(finish.cur, x);
        ++finish.cur;
    } else {
        push_back_aux(x);
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::push_back_aux(const value_type& x) {
    value_type x_copy = x;
    //reserve_map_at_back();
    *(finish.M_node + 1) = allocate_node();
    construct(finish.cur, x_copy);
    finish.set_node(finish.M_node + 1);
    finish.cur = finish.first;
}

//从头部插入元素
template<class T, class Alloc>
void deque<T, Alloc>::push_front(const value_type& x) {
    if (start.cur != start.first) {
        --start.cur;
        construct(start.cur, x);
    } else {
        push_front_aux(x);
    }
}

template<class T, class Alloc>
void deque<T, Alloc>::push_front_aux(const value_type& x) {
    value_type x_copy = x;
    //reserve_map_at_front();
    *(start.M_node - 1) = allocate_node();
    start.set_node(start.M_node - 1);
    start.cur = start.last - 1;
    construct(start.cur, x_copy);
}


template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(size_type n, const value_type& x) {
    // 初始化map及map所指节点;
    create_map_and_nodes(n);
    Map_pointer cur;
    // 初始化node缓存
    for (cur = start.M_node; cur < finish.M_node; ++cur) {
        LOG_DEQ("deque buffer_size = "<<  buffer_size());
        uninitialized_fill(*cur, *cur + buffer_size(), x);
    }
    uninitialized_fill(finish.first, finish.cur, x);
    LOG_DEQ("deque fill_initialize done!");
}

template<class T, class Alloc>
void deque<T, Alloc>::create_map_and_nodes(size_type num_elements) {
    // 计算map上分配的node数量; 多分配一个节点;
    size_type num_nodes = num_elements / buffer_size() + 1;
    
    map_size = max(init_map_size(), num_nodes + 2 );
    map = map_allocatetor::allocate(map_size);

    // 初始化start 和 finish 的位置, 指向map 的中央;
    Map_pointer init_start = map + (map_size - num_nodes) / 2;
    Map_pointer init_finish = init_start + num_nodes - 1;
    
    Map_pointer cur;
    for (cur = init_start; cur <= init_finish; ++cur) {
        *cur = allocate_node();
    }
    start.set_node(init_start);
    finish.set_node(init_finish);
    start.cur = start.first;   // 如果刚好整除, finish.cur就指向多分配一个节点的first;
    finish.cur = finish.first + num_elements % buffer_size();
    LOG_DEQ("create_map_and_nodes done !");
    return;
}


_MY_NAMESPACE_END

#undef LOG_DEQ
#endif
