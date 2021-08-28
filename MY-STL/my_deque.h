/*************************************************************************
	> File Name: my_deque.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 25 Aug 2021 04:58:25 PM CST
 ************************************************************************/

#ifndef _MY_DEQUE_H
#define _MY_DEQUE_H
#define BLU  "\033[34m"
#define YEL  "\033[33m"
#define RED  "\033[31m"
#define FIN   "\033[0m"

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
    typedef ptrdiff_t           difference_type;
    
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
public:
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
        LOG_DEQ("deallocate_node finish!");
    }
    void fill_initialize(size_type n, const value_type& x = T()); 
    void create_map_and_nodes(size_type num_elements);
    size_type init_map_size() {
        return static_cast<size_type>(_M_init_map_size);
    }
public:
    deque() : start(), finish(), map(0), map_size(0) {
        fill_initialize(0);
    }
    deque(int n, const value_type& x) 
        : start(), finish(), map(0), map_size(0)
    {
        LOG_DEQ("deque constructor finish!"); 
        fill_initialize(n, x);
    }
    ~deque() {
        if (map) {
            for (Map_pointer it = start.M_node; it != finish.M_node + 1; ++it) {
                deallocate_node(*it);
            }
        }
        map_allocatetor::deallocate(map, map_size);
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
    
    reference operator[](size_type x) {return start[difference_type(x)];} 
    bool empty() const {return start == finish;}
    void push_back(const value_type& x);
    void push_back_aux(const value_type& x);
    void push_front(const value_type& x);
    void push_front_aux(const value_type& x);
    void pop_back();
    void pop_back_aux();
    void pop_front();
    void pop_front_aux();
    void clear();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator pos, const value_type& x);
    iterator insert_aux(iterator pos, const value_type& x);
protected:
    void reserve_map_at_back(size_type nodes_to_add = 1);
    void reserve_map_at_front(size_type nodes_to_add = 1);
    void reallocate_map(size_type nodes_to_add, bool add_to_front);
};

// 检查空间,是否扩容;
template <class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add) {
    // 当 加入新节点数 大于|map_size| - | 已用节点数 | = 尾部剩余可用空间;
    if (nodes_to_add  > map_size - (finish.M_node - map + 1)) {
        reallocate_map(nodes_to_add, false);
        LOG_DEQ(RED << "reserve_map_at_back" << FIN);
    }
}

// 在指定位置插入元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert(iterator pos, const value_type& x) {
    if (pos == start) {
        push_front(x);
        return start;
    }
    else if (pos == finish){
        push_back(x);
        iterator tmp = finish;
        --tmp;
        return tmp;
    }
    else{
        return insert_aux(pos, x);
    } 
}

// 封装 insert_aux ,用来处理复杂情况;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert_aux(iterator pos, const value_type& x) {
    difference_type index = pos - start; // 计算插入点之前的元素个数;
    value_type x_copy = x;
    // 分类处理, 1.插入点前方元素少 2. 插入点后边元素少;
    if (index < size() / 2) {
        push_front(front());
        iterator front1 = start; 
        ++front1; // start, -> front1, -> front2;
        iterator front2 = front1;
        ++front2;
        pos = start + index; 
        iterator pos1 = pos;
        ++pos1;  //  将 pos 之前的元素向前移动一位;
        copy(front2, pos1, front1);
    } else {
        push_back(back());
        iterator back1 = finish;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start + index; // 将pos 后面的元素整体后移一位;
        copy_backward(pos, back2, back1);
    }
    *pos = x_copy;
    return pos;
}

//从指定迭代器位删除元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::erase(iterator pos) {
    iterator next = pos;
    ++next;
    difference_type index = pos - start;
    if (index < (size() >> 1)) { // 当pos前方元素较少时;
        copy_backward(start, pos, next);
        pop_front();
    } else {
        copy(next, finish, pos);
        pop_back();
    }
    return start + index;
}

//删除迭代器范围内的元素;
template <class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::erase(iterator first, iterator last) {
    if (first == start && last == finish) {
        clear();
        return finish;
    }
    else {
        difference_type n = last - first; // 计算要删除元素的多少;
        difference_type elems_before = first - start;
        if (elems_before < (size() - n) / 2) { // first前面的元素较少时;
            copy_backward(start, first, last);
            iterator new_start = start + n;
            destroy(start, new_start);   // 删除前面空的节点;
            for (Map_pointer cur = start.M_node; cur < new_start.M_node; ++cur) { 
                data_allocatetor::deallocate(*cur, buffer_size());
            }
            start = new_start;
        } else {  // 移动后面的元素;
            copy(last, finish, first);
            iterator new_finish = finish - n;
            destroy(new_finish, finish); // 析构掉多余元素;
            for (Map_pointer cur = new_finish.M_node + 1; cur <= finish.M_node; ++cur) { 
                data_allocatetor::deallocate(*cur, buffer_size());
            }
            finish = new_finish;
        }
        return start + elems_before;
    }
}

template <class T, class Alloc>
void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add) {
    // 当 加入新节点数 大于|开始节点位| - |其实空间位 | = 头部剩余可用空间;
    if (nodes_to_add > start.M_node - map) {
        reallocate_map(nodes_to_add, true);
        LOG_DEQ(RED << "reserve_map_at_front" << FIN);
    }
}

// 重新配置map的大小,拷贝原来的node节点过来, 只用拷贝指针, 原map上存的就是指针;
template <class T, class Alloc>
void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_to_front) {
    // 计算扩容后的节点数量;
    size_type old_num_nodes = finish.M_node - start.M_node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;
    
    Map_pointer new_start;
    // 如果当前map大小大于加入新空间的2倍,那么移动指针就行;
    if (map_size > 2 * new_num_nodes) {
        LOG_DEQ(YEL << "just move pointer at map BEGIN" << FIN);
        new_start = map + (map_size - new_num_nodes) / 2 + (add_to_front ? nodes_to_add : 0);
        if (new_start < start.M_node) {
            copy(start.M_node, finish.M_node + 1, new_start);
        } else {
            copy_backward(start.M_node, finish.M_node + 1, new_start + old_num_nodes);
        }
        LOG_DEQ(YEL << "just move pointer at map END" << FIN);
    } // 当空间不足, 需要重新分配map;
    else {
        size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
        Map_pointer new_map = map_allocatetor::allocate(new_map_size);
        // 计算新的开始位;
        new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_to_front ? nodes_to_add : 0);
        // 只用拷贝原map指针过来; 这里注意 copy的last= finish.M_node + 1;
        copy(start.M_node, finish.M_node + 1, new_start);
        LOG_DEQ(RED << "old_map = " << map << FIN);
        LOG_DEQ(RED << "new_map = " << new_map << FIN);
        map_allocatetor::deallocate(this->map, map_size);
        this->map = new_map;
        this->map_size = new_map_size;
        LOG_DEQ(BLU << "reallocate_map size = " << map_size << FIN);
    }
    // 这个时候只是扩容, 并未插入元素;
    start.set_node(new_start);
    finish.set_node(new_start + old_num_nodes - 1);
    //start 和 finish中的cur, first , last都还是指向原来的元素, 变得不过是map中存放的指向node的指针;
    
}


// 从头部弹出元素;    
template <class T, class Alloc>
void deque<T, Alloc>::pop_front() {
    if (start.cur + 1 != start.last) {
        destroy(start.cur);
        ++start.cur;
    } else {
        pop_front_aux();
    }
}
    
template <class T, class Alloc>
void deque<T, Alloc>::pop_front_aux() {
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.M_node + 1);
    start.cur = start.first;
    return ;
}

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
    reserve_map_at_back(1);
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
    reserve_map_at_front(1);
    *(start.M_node - 1) = allocate_node();
    start.set_node(start.M_node - 1);
    start.cur = start.last - 1;
    construct(start.cur, x_copy);
}


template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(size_type n, const value_type& x ) {
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

template<class T, class Alloc>
void deque<T, Alloc>::clear() {
    for (Map_pointer node = start.M_node + 1;node < finish.M_node; ++node) {
        // 将 中间装满buf的节点先调用destroy在释放内存;
        destroy(*node, *node + buffer_size());
        data_allocatetor::deallocate(*node, buffer_size());
    }
    // 然后保留一个节点;
    if (start.M_node != finish.M_node) {
        destroy(start.cur, start.last);
        destroy(finish.first, finish.cur);
        data_allocatetor::deallocate(finish.first, buffer_size());
    } else {
        destroy(start.cur, finish.cur);
    }

    finish = start;
}

_MY_NAMESPACE_END

#undef LOG_DEQ
#endif
