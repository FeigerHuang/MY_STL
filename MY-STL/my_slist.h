/*************************************************************************
	> File Name: my_slist.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Tue 10 Aug 2021 10:18:18 PM CST
 ************************************************************************/

#ifndef _MY_SLIST_H
#define _MY_SLIST_H

#ifdef BUG_SLI
#define LOG(msg) {std::cerr << msg << std::endl;}
#else
#define LOG(msg)
#endif

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

//单向链表 的节点定义 BEGIN
struct __slist_node_base
{
    __slist_node_base *next;
};

//继承关系, 型别转换上比较复杂;
template<typename T>
struct __slist_node : public __slist_node_base
{
    T data; 
};

//单向链表 的节点定义 END


inline 
__slist_node_base* __insert_node_after(
    __slist_node_base *prev_node, 
    __slist_node_base* new_node) 
{
    new_node->next = prev_node->next;
    prev_node->next =  new_node;
    return new_node;
}

inline 
size_t __slist_size(__slist_node_base* node) {
    size_t result = 0;
    while (node != nullptr) 
    {
        node = node->next;
        ++result;
    }
    return result;
}

//单向链表 迭代器实现 BEGIN 
struct __slist_iterator_base
{
    typedef size_t               size_type;
    typedef ptrdiff_t            difference_type;
    typedef forward_iterator_tag iterator_category;
    //通过继承关系, 父类指针可以指向子类;
    __slist_node_base* node; // 指向节点的原生指针 node

    __slist_iterator_base(__slist_node_base* x) : node(x) {}

    void increase() { node = node->next; }

    bool operator==(const __slist_iterator_base& x) const {
        return node == x.node;
    }

    bool operator!=(const __slist_iterator_base& x) const {
        return node != x.node;
    }
};

// 3个模板参数, 方便我们内部定义const_iterator;
template<typename T, typename Ref, class Ptr>
struct __slist_iterator : public __slist_iterator_base 
{
    typedef __slist_iterator<T, T&, T*>              iterator;
    typedef __slist_iterator<T, const T&, const T*>  const_iterator;
    typedef __slist_iterator<T, Ref, Ptr>            self;

    typedef T                 value_type;
    typedef Ptr               pointer;
    typedef Ref               reference;
    typedef __slist_node<T>   list_node;
    
    __slist_iterator(list_node* rhs) : __slist_iterator_base(rhs) {}

    __slist_iterator() : __slist_iterator_base(0) {}

    __slist_iterator(const iterator& rhs) : __slist_iterator_base(rhs.node) {}

    reference operator*(){return ((list_node *)node)->data;}

    pointer operator->(){return &(operator*());}

    self& operator++() {
        increase();
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        increase();
        return tmp;
    }
    
    operator bool() const {
        return node;
    }
};

//单向链表 迭代器实现 END

// slist 实现 BEGIN

template <class T, class Alloc = allocator<T> >
class slist{
public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    typedef __slist_iterator<T, T&, T*>             iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;

private:
    typedef __slist_node<T>         list_node;
    typedef __slist_node_base       list_node_base;
    typedef __slist_iterator_base   iterator_base;
    typedef simple_alloc<list_node> list_node_allocator;
    
private:
    static list_node *create_node(const value_type& x) {
        list_node *node = list_node_allocator::allocate();
        //LOG("list_node_allocator ok !"); 
        construct(&node->data, x);
        node->next = nullptr;
        //LOG("x address = " << &x);
        return node;
    }
    
    static void destroy_node(list_node* node) {
        destroy(&node->data);
        list_node_allocator::deallocate(node);
        LOG("node address =" << node << " :destroy done!");
    }

    list_node* _MY_insert_after(list_node_base* node, const value_type& x) {
        return static_cast<list_node *>(__insert_node_after(node, create_node(x)));
    }
    
private:
    //虚拟头节点, 用base类节省了空间;
    list_node_base head;

public:
    slist() {
        LOG("slist constructor done!");    
        head.next = nullptr;
    }
    explicit slist(const slist& rhs) {
        //TODO 
    }
    slist& operator=(const slist& rhs) {
        //TODO
    }
    slist(slist&& rhs) noexcept {
        this->swap(rhs);
        rhs.head.next = nullptr;    
        return ;
    }
    ~slist() {
        clear();
        LOG("~slist() destructor finish!");
    }
public:
    iterator begin() {
        return iterator(static_cast<list_node*>(head.next));
    }
    iterator end() {
        return iterator(0);
    } 
    const_iterator cbegin() const {
        return const_iterator(static_cast<list_node*>(head.next));
    }
    const_iterator cend() const {
        return const_iterator(0);
    } 
    size_type size() const {
        return __slist_size(head.next);
    } 
    bool empty() const {
        return head.next == nullptr;
    }
    void swap(slist& rhs) {
        list_node_base* temp = head.next;
        head.next = rhs.head.next;
        rhs.head.next = temp;
        return;
    }

public:
    reference front() {return ((list_node*) head.next)->data;}
    
    void push_front(const value_type& x) {
        __insert_node_after(&head, create_node(x));
    }
    
    void pop_front() {
        list_node *node = static_cast<list_node *>(head.next);
        head.next = node->next;
        destroy_node(node);
    }
    
    iterator insert_after(iterator pos, const value_type& x) {
        return iterator(_MY_insert_after(pos.node, x));
    }
    
    iterator find(const value_type& x) {
        iterator  cur(this->begin());
        while (cur != this->end()) {
            if (*cur == x) {
                break;
            } 
            ++cur;
        }
        return cur;
    }

    void clear() {
        if (head.next == nullptr) return;
        list_node *cur = static_cast<list_node *>(head.next);
        while (cur != nullptr) {
            head.next = cur->next;
            destroy_node(cur);
            cur = static_cast<list_node *>(head.next);
        }
        return ;
    }
};


// slist 实现 END

_MY_NAMESPACE_END
#undef LOG

#endif
