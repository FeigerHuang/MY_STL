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
    
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
        : hash(hf), equals(eql),get_key(ExtractKey()) , num_elements(0)
    {
        initialize_buckets(n);
    }

    size_type bucket_count() const {return buckets.size();}

    node *new_node(const value_type& obj) {
        node *x = node_allocator::allocator();
        construct(&x->val, obj);
        return x;
    }
    void delete_node(node *x) {
        destroy(&x->val);
        node_allocator::deallocate(x);
    }
private:
    size_type next_size(size_type n) {
        return __get_next_prime(n);
    }

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


// hashtable END

_MY_NAMESPACE_END

#undef LOG_HASH
#endif
