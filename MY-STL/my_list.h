/*************************************************************************
	> File Name: my_list.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Tue 24 Aug 2021 08:26:41 PM CST
 ************************************************************************/

#ifndef _MY_LIST_H
#define _MY_LIST_H

#ifdef BUG_LIS
#define LOG_LIS(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_LIS(msg)
#endif
#include "my_iterator.h"
#include "my_alloc.h"
#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

// list的节点定义
template <class T>
struct __list_node{
    typedef void* node_pointer;
    node_pointer prev;
    node_pointer next;
    T data;
};

// list 的迭代器定义
template <class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*>              iterator;
    typedef __list_iterator<T, const T&, const T*>  const_iterator;
    typedef __list_iterator<T, Ref, Ptr>            self;
    
    typedef bidirectional_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Ptr                         pointer;
    typedef Ref                         reference;
    typedef __list_node<T>*             link_type;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    
    link_type   node;   // 指向节点;
    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator& x) : node(x.node) {}

    bool operator==(const self& x) {
        return node == x.node;
    }
    bool operator!=(const self& x) {
        return node != x.node;
    }

    reference operator*(){return node->data;}
    pointer operator->(){return &(operator*());}
    self operator++() {
        node = static_cast<link_type>(node->next);
        return *this;
    }
    self operator++(int) {
        iterator ans(*this);
        ++*this;
        return ans; 
    }
    self operator--() {
        node = static_cast<link_type>(node->prev);
        return *this;
    }
    self operator--(int) {
        iterator ans(*this);
        --*this;
        return ans;
    }
};

// list实现
template <class T, class Alloc = allocator<T>>
class list {
public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef __list_iterator<T, T&, T*>             iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;

private:
    typedef __list_node<T>      list_node;
    typedef simple_alloc<list_node>  list_node_allocator;   
protected:
    typedef list_node*          link_type;
    link_type                   virtual_node;
private:
    static list_node *create_node(const value_type& x) {
        list_node *node = list_node_allocator::allocate();
        construct(&node->data, x);
        node->next = nullptr;
        node->prev = nullptr;
        LOG_LIS("address=" << node <<"list_node_allocator ok !"); 
        return node;
    }
    
    static list_node *create_node() {
        list_node *node = list_node_allocator::allocate();
        construct(&node->data, T());
        node->next = nullptr;
        node->prev = nullptr;
        LOG_LIS("address=" << node <<"list_node_allocator ok !"); 
        return node;
    }
    
    static void destroy_node(list_node* node) {
        destroy(&node->data);
        list_node_allocator::deallocate(node);
        LOG_LIS("node address =" << node << " :destroy done!");
    }
protected:
    void empty_initialize() {
        virtual_node = create_node();
        virtual_node->next = virtual_node;
        virtual_node->prev = virtual_node;
    }
public:
    list() {empty_initialize();}
    ~list() {
        clear();
        destroy_node(virtual_node);
        return;
    }
    size_type size() const {
        size_type result = 0;
        result = distance(cbegin(), cend());
        return result;
    }
    iterator begin() {return iterator(link_type(virtual_node->next));}
    iterator end() {return iterator(virtual_node);}
    const_iterator cbegin() const {return const_iterator(link_type(virtual_node->next));}
    const_iterator cend() const {return const_iterator(virtual_node);}
    bool empty() const {return cbegin() == cend();}

    reference front() {return *begin();}
    reference back() {return *(--end());}
    // 插入元素到pos的位置;
    iterator insert(iterator pos, const value_type& x) {
        list_node *tmp = create_node(x);
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        link_type(pos.node->prev)->next = tmp;
        pos.node->prev = tmp;
        return iterator(tmp);
    }
    void push_front(const value_type& x) {
        insert(begin(), x);
    }
    void push_back(const value_type& x) {
        insert(end(), x);
    }
    //移除节点相关操作; 
    iterator erase(iterator pos) {
        if (empty()) return end();
        link_type prev_node = static_cast<link_type>(pos.node->prev);
        link_type next_node = static_cast<link_type>(pos.node->next);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(pos.node);
        return next_node;
    }
    void pop_front() {
        erase(begin());
    }
    void pop_back() {
        iterator tmp = end();
        erase(--tmp);
        // 不能直接写成erase(--end());?rvalue 
    }
    void remove(const T& value); 
    void unique();
    void clear();
};

template<class T,class Alloc>
void list<T, Alloc>::unique() {
    iterator first = begin();
    iterator last = end();
    if (first == last) return;
    iterator next = first;
    while (++next != last) {
        if (*first == *next) {
            erase(next);
        } else {
            first = next;
        }
        next = first;
    }

}

template<class T, class Alloc>
void list<T,Alloc>::remove(const T& value) {
    iterator first = begin();
    iterator last = end();
    
    while (first != last) {
        iterator next = first;
        ++next;
        if (*first == value) erase(first);
        first = next;
    }
}


template<class T, class Alloc>
void list<T,Alloc>::clear() {
    link_type cur = static_cast<link_type>(virtual_node->next);
    while (cur != virtual_node) {
        virtual_node->next = cur->next;
        destroy_node(cur);
        cur = static_cast<link_type>(virtual_node->next);
    }
    virtual_node->next = virtual_node;
    virtual_node->prev = virtual_node;
}

_MY_NAMESPACE_END
#undef LOG_LIS
#endif
