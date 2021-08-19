/*************************************************************************
	> File Name: my_hash_map.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 18 Aug 2021 05:22:58 PM CST
 ************************************************************************/

#ifndef _MY_HASHTABLE_H
#define _MY_HASHTABLE_H

#ifdef BUG_HASH
#define LOG_HASH(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_HASH(msg)
#endif

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

#include "my_pair.h"

_MY_NAMESPACE_BEGIN

// hash_node 定义 BEGIN

template <class T>
struct __hash_node {
    __hash_node *next;
    T val;
};

// hash_node 定义 END
template<class Value, class Key, class HashFcn,
        class ExtractKey, class EqualKey>
class hashtable;

// hash_map iterator 定义 BEGIN

template <class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
struct __hash_iterator {           
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>
        _M_hashtable;
    typedef __hash_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>
        iterator;
    typedef __hash_node<Value>  node;
    
    typedef forward_iterator_tag iterator_category;
    typedef Value                value;
    typedef ptrdiff_t            difference_type;
    typedef size_t               size_type;
    typedef Value&               reference;
    typedef Value*               pointer;

    node *cur;
    _M_hashtable *hs_table;
    
    __hash_iterator(node *n, _M_hashtable* tab) 
        : cur(n), hs_table(tab) {}
    __hash_iterator() {}
    
    reference operator*() const {return cur->val;}
    pointer operator->() const {return &(operator*());}
    
    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& it) const {
        return cur == it.cur;
    }
    bool operator!=(const iterator& it) const {
        return cur != it.cur;
    }
};

template <class V, class K, class HF, class ExK, class EqK>
__hash_iterator<V, K, HF, ExK, EqK>&
__hash_iterator<V, K, HF, ExK, EqK>::operator++()
{
    const node *old = cur;
    cur = cur->next;
    if (!cur) {
        // 定位到正确的位置;
        size_type bucket = hs_table->bkt_num(old->val);
        while (!cur && ++bucket < hs_table->buckets.size())
        {
            cur = hs_table->buckets[bucket];
        }
    }
    return *this;
}


template <class V, class K, class HF, class ExK, class EqK>
__hash_iterator<V, K, HF, ExK, EqK>
__hash_iterator<V, K, HF, ExK, EqK>::operator++(int)
{
    iterator tmp(*this);
    ++this;
    return tmp;
}

// hash_map iterator 定义 END 
   
enum { __num_primes = 28 };
//素数已经备好, 方便扩容
static const unsigned long __prime_list[__num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __get_next_prime(unsigned long n) 
{
    const unsigned long* first = __prime_list;
    const unsigned long* last = __prime_list + (int)(__num_primes);

    const unsigned long* pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

// hashtable 实现 BEGIN
  
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
class hashtable 
{
public:
    typedef Key              key_type;
    typedef Value            value_type;
    typedef size_t           size_type;
    typedef ptrdiff_t        difference_type;
    typedef value_type*      pointer;
    typedef const value_type const_pointer;
    typedef value_type&      reference;
    typedef const value_type const_reference;

    typedef HashFcn          hasher;
    typedef EqualKey         key_equal;
    
private:
    hasher     hash;
    key_equal  equals;
    ExtractKey get_key;

    typedef __hash_node<Value>  node;
    typedef simple_alloc<node>  node_allocator;
    
    vector<node*> buckets;
    size_type  num_elements;
public:
    typedef __hash_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>
            iterator;
    
    friend struct 
    __hash_iterator<Value, Key, HashFcn, ExtractKey, EqualKey>;

public:
    hashtable(size_type n, const HashFcn& hf,
            const EqualKey& eql, const ExtractKey& ext)
        : hash(hf), equals(eql),get_key(ext) , num_elements(0)
    {
        initialize_buckets(n);
    }
    size_type elems_count() {return num_elements;}
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
        : hash(hf), equals(eql),get_key(ExtractKey()) , num_elements(0)
    {
        initialize_buckets(n);
    }
    
    iterator begin() {
        // 找到第一个存在元素的槽;
        for (size_type index = 0; index < buckets.size(); ++index) {
            if (buckets[index] != 0) {
                return iterator(buckets[index], this);
            }
        }
        return end();
    }
    
    iterator end() {
        return iterator(0, this);
    }

    size_type bucket_count() const {return buckets.size();}
    // 用于新建节点;
    node *new_node(const value_type& obj) {
        node *x = node_allocator::allocate();
        construct(&x->val, obj);
        return x;
    }
    // 用于销毁节点;
    void delete_node(node *x) {
        destroy(&x->val);
        node_allocator::deallocate(x);
    }
     
    void resize(size_type n);
    iterator insert_equal(const Value& obj);
    iterator insert_equal_noresize(const Value& obj);
    pair<iterator, bool> insert_unique(const value_type& obj);
    pair<iterator, bool> insert_unique_noresize(const value_type& obj);
    iterator find(const key_type& obj);
    size_type count(const key_type& obj);
private:
    // 获得下一个质数大小的空间
    size_type next_size(size_type n) {
        return __get_next_prime(n);
    }
    // 初始化vector
    void initialize_buckets(size_type n) {
        const size_type _n_bucktes = next_size(n);
        buckets.clear();
        buckets.resize(_n_bucktes, (node*)0);
        num_elements = 0;
    }
    
    size_type bkt_num_key(const value_type& key, size_t n) const {
        return hash(key) % n;
    }

    size_type bkt_num_key(const value_type& key) const {
        return bkt_num_key(key, buckets.size());
    }
    
    size_type bkt_num(const value_type& obj) const {
        return bkt_num_key(obj);
    }

    size_type bkt_num(const value_type& obj, size_t n) const {
        return bkt_num_key(get_key(obj), n);
    }
};

// 查找元素;

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::iterator 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::find(const Key& x) {
    const size_type index = bkt_num(x);
    node* first = buckets[index];

    while (first && !equals(get_key(first->val), x)) {
        first = first->next;
    }
    
    return iterator(first, this);
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::size_type 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::count(const Key& x) 
{
    const size_type index = bkt_num(x);
    size_type ans = 0;
    node *first = buckets[index];

    while (first) {
        if (equals(get_key(first->val), x)) {
            ++ans;
        }
        first = first->next;
    } 
    return ans;
}

//非重复性插入操作;
template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::iterator , bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::insert_unique(const value_type& obj) {
    // 首先检查是否需要扩容;
    resize(num_elements + 1);
    return insert_unique_noresize(obj);
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::iterator , bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::insert_unique_noresize(const value_type& obj) {
    const size_type index = bkt_num(obj);
    node *first = buckets[index];
    
    // 检查index槽上挂着的链表节点是否存在当前插入键值
    for (node *cur = first; cur ; cur = cur->next) {
        if(equals(get_key(cur->val), get_key(obj)))
        {
            return pair<iterator,bool>(iterator(cur, this), false);
        }
    }
    //没有重复元素, 直接new 一个插入卡槽; 
    node *temp = new_node(obj);
    temp->next = first;
    buckets[index] = temp;
    ++num_elements;
    return pair<iterator, bool>(iterator(temp, this), true);
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::iterator 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::insert_equal_noresize(const Value& obj) {
    const size_type index = bkt_num(obj);
    node *first = buckets[index];
    // 首先计算插入 卡槽下标; 
    for (node *cur = first; cur; cur = cur->next) {
        //判断是否纯在相等键值, 如果存在
        if (equals(get_key(cur->val), get_key(obj))) {
            node *temp = new_node(obj);
            temp->next = cur->next;
            cur->next = temp;
            ++num_elements;
            return iterator(temp, this);
        }
    }
    // 不存在相同键值;
    node *temp = new_node(obj);
    temp->next = first;
    buckets[index] = temp;
    ++num_elements;
    return iterator(temp, this);
}

template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::iterator 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::insert_equal(const Value& obj) {
    resize(num_elements + 1);
    return insert_equal_noresize(obj);
}


template<class Value, class Key, class HashFcn,
            class ExtractKey, class EqualKey>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey>::resize(size_type n)
{
    const size_type old_n = buckets.size();
    if (n <= old_n) return;
    
    const size_type new_n = next_size(n);
    vector<node *> tmp(new_n, (node *)0);
    
    for (size_type slot = 0; slot < old_n; ++slot) {
        node *cur = buckets[slot];
        while(cur) {
            // 操作细节, 先找到新buckets的正确插入点;
            size_type new_slot = bkt_num(cur->val, new_n);
            buckets[slot] = cur->next; // 原槽位为 当前节点下一项;
            cur->next = tmp[new_slot]; // 当前节点的next指向 新槽的下一项;
            tmp[new_slot] = cur;       // 新的槽存放当前节点的地址;
            cur = buckets[slot];       // 当前cur指向原槽位;
        }
    }
    
    buckets.swap(tmp);
}

// hashtable END

_MY_NAMESPACE_END

#undef LOG_HASH
#endif
