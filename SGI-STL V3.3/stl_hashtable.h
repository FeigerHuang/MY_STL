/*
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_HASHTABLE_H
#define __SGI_STL_INTERNAL_HASHTABLE_H

// Hashtable class, used to implement the hashed associative containers
// hash_set, hash_map, hash_multiset, and hash_multimap.

#include <stl_algobase.h>
#include <stl_alloc.h>
#include <stl_construct.h>
#include <stl_tempbuf.h>
#include <stl_algo.h>
#include <stl_uninitialized.h>
#include <stl_function.h>
#include <stl_vector.h>
#include <stl_hash_fun.h>

__STL_BEGIN_NAMESPACE

template <class _Val>
struct _Hashtable_node
{
  _Hashtable_node* _M_next;
  _Val _M_val;
}; // node 是链表节点; 

template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc = alloc>
class hashtable;

template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
struct _Hashtable_iterator;

template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
struct _Hashtable_const_iterator;

// Hashtable的迭代器;
template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
struct _Hashtable_iterator {
  typedef hashtable<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>
          _Hashtable;
  typedef _Hashtable_iterator<_Val, _Key, _HashFcn, 
                              _ExtractKey, _EqualKey, _Alloc>
          iterator;
  typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, 
                                    _ExtractKey, _EqualKey, _Alloc>
          const_iterator;
  typedef _Hashtable_node<_Val> _Node;  // 迭代器所指元素为 Node;

  typedef forward_iterator_tag iterator_category;
  typedef _Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef _Val& reference;
  typedef _Val* pointer;

  _Node* _M_cur;         // 迭代器存放所指Node指针;
  _Hashtable* _M_ht;      // 以及 指向hashtable的指针;

    //构造函数传入 所指节点指针和hashtable指针;
  _Hashtable_iterator(_Node* __n, _Hashtable* __tab) 
    : _M_cur(__n), _M_ht(__tab) {}
  _Hashtable_iterator() {}
    //解引用运算访问node里存的Val
  reference operator*() const { return _M_cur->_M_val; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
    //箭头运算访问Val的地址;
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
  iterator& operator++();  // 这里为了实现++ 所有前面有hashtable的指针;
  iterator operator++(int);
  bool operator==(const iterator& __it) const
    { return _M_cur == __it._M_cur; }
  bool operator!=(const iterator& __it) const
    { return _M_cur != __it._M_cur; }
};


template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
struct _Hashtable_const_iterator {
  typedef hashtable<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>
          _Hashtable;
  typedef _Hashtable_iterator<_Val,_Key,_HashFcn, 
                              _ExtractKey,_EqualKey,_Alloc>
          iterator;
  typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, 
                                    _ExtractKey, _EqualKey, _Alloc>
          const_iterator;
  typedef _Hashtable_node<_Val> _Node;

  typedef forward_iterator_tag iterator_category;
  typedef _Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  // 和普通迭代器的区别这儿加上了 const 关键字; 
  typedef const _Val& reference;
  typedef const _Val* pointer;

  const _Node* _M_cur;
  const _Hashtable* _M_ht; // 利用_M_ht回溯到哈希table

  _Hashtable_const_iterator(const _Node* __n, const _Hashtable* __tab)
    : _M_cur(__n), _M_ht(__tab) {}
  _Hashtable_const_iterator() {}
  _Hashtable_const_iterator(const iterator& __it) 
    : _M_cur(__it._M_cur), _M_ht(__it._M_ht) {}
  reference operator*() const { return _M_cur->_M_val; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
  const_iterator& operator++();
  const_iterator operator++(int);
  bool operator==(const const_iterator& __it) const 
    { return _M_cur == __it._M_cur; }
  bool operator!=(const const_iterator& __it) const 
    { return _M_cur != __it._M_cur; }
};

// Note: assumes long is at least 32 bits.
enum { __stl_num_primes = 28 };
// 准备的很周到, 素数已经备好, 方便扩容
static const unsigned long __stl_prime_list[__stl_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long __n)
{
  const unsigned long* __first = __stl_prime_list;
  const unsigned long* __last = __stl_prime_list + (int)__stl_num_primes;
    //找到第一个大于等于 _n的位置;
  const unsigned long* pos = lower_bound(__first, __last, __n);
  return pos == __last ? *(__last - 1) : *pos;
}

// Forward declaration of operator==.

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
class hashtable;

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
bool operator==(const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht1,
                const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht2);


// Hashtables handle allocators a bit differently than other containers
//  do.  If we're using standard-conforming allocators, then a hashtable
//  unconditionally has a member variable to hold its allocator, even if
//  it so happens that all instances of the allocator type are identical.
// This is because, for hashtables, this extra storage is negligible.  
//  Additionally, a base class wouldn't serve any other purposes; it 
//  wouldn't, for example, simplify the exception-handling code.

//利用6个模板参数,泛化: 值, 键, 哈希函数, 提起键, 比较键, 空间配置器
template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
class hashtable {
public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef _HashFcn hasher;
  typedef _EqualKey key_equal;

  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;

  hasher hash_funct() const { return _M_hash; }
  key_equal key_eq() const { return _M_equals; }

private:      // 定义Node和迭起器里的定义做到一致;
  typedef _Hashtable_node<_Val> _Node;

#ifdef __STL_USE_STD_ALLOCATORS
public:
  typedef typename _Alloc_traits<_Val,_Alloc>::allocator_type allocator_type;
  allocator_type get_allocator() const { return _M_node_allocator; }
private:
  typename _Alloc_traits<_Node, _Alloc>::allocator_type _M_node_allocator;
    // 分别用来构造和销毁Node;
  _Node* _M_get_node() { return _M_node_allocator.allocate(1); }
  void _M_put_node(_Node* __p) { _M_node_allocator.deallocate(__p, 1); }
# define __HASH_ALLOC_INIT(__a) _M_node_allocator(__a), 
#else /* __STL_USE_STD_ALLOCATORS */
public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }
private:
  typedef simple_alloc<_Node, _Alloc> _M_node_allocator_type;
  _Node* _M_get_node() { return _M_node_allocator_type::allocate(1); }
  void _M_put_node(_Node* __p) { _M_node_allocator_type::deallocate(__p, 1); }
# define __HASH_ALLOC_INIT(__a)
#endif /* __STL_USE_STD_ALLOCATORS */

private:
  hasher                _M_hash;  // hash func 
  key_equal             _M_equals; // 判断键值相等的function 
  _ExtractKey           _M_get_key; // 提前键值 的function
  vector<_Node*,_Alloc> _M_buckets;   // buckets, 存放链表头节点;
  size_type             _M_num_elements; 

public:
  typedef _Hashtable_iterator<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>
          iterator;
  typedef _Hashtable_const_iterator<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,
                                    _Alloc>
          const_iterator;

  friend struct
  _Hashtable_iterator<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>;
  friend struct
  _Hashtable_const_iterator<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>;

public:
  hashtable(size_type __n,
            const _HashFcn&    __hf,
            const _EqualKey&   __eql,
            const _ExtractKey& __ext,
            const allocator_type& __a = allocator_type())
    : __HASH_ALLOC_INIT(__a)  // 就是define了一个 _M_node_allocator(__a),
      _M_hash(__hf),
      _M_equals(__eql),
      _M_get_key(__ext),
      _M_buckets(__a),
      _M_num_elements(0)
  {
    _M_initialize_buckets(__n);
  }

  hashtable(size_type __n,
            const _HashFcn&    __hf,
            const _EqualKey&   __eql,
            const allocator_type& __a = allocator_type())
    : __HASH_ALLOC_INIT(__a)
      _M_hash(__hf),
      _M_equals(__eql),
      _M_get_key(_ExtractKey()),
      _M_buckets(__a),
      _M_num_elements(0)
  {
    _M_initialize_buckets(__n);
  }

  hashtable(const hashtable& __ht)
    : __HASH_ALLOC_INIT(__ht.get_allocator())
      _M_hash(__ht._M_hash),
      _M_equals(__ht._M_equals),
      _M_get_key(__ht._M_get_key),
      _M_buckets(__ht.get_allocator()),
      _M_num_elements(0)
  {
    _M_copy_from(__ht);
  }

#undef __HASH_ALLOC_INIT
    // 赋值运算符;
  hashtable& operator= (const hashtable& __ht)
  {
    if (&__ht != this) {
      clear();
      _M_hash = __ht._M_hash;
      _M_equals = __ht._M_equals;
      _M_get_key = __ht._M_get_key;
      _M_copy_from(__ht);
    }
    return *this;
  }

  ~hashtable() { clear(); }
                // _M_num_elements 用来记录元素数量;
  size_type size() const { return _M_num_elements; }
  size_type max_size() const { return size_type(-1); }
  bool empty() const { return size() == 0; }

  //定义swap函数, 当时还没出C++11;设计移动构造用这个方便;
  void swap(hashtable& __ht)
  {
    __STD::swap(_M_hash, __ht._M_hash);
    __STD::swap(_M_equals, __ht._M_equals);
    __STD::swap(_M_get_key, __ht._M_get_key);
    _M_buckets.swap(__ht._M_buckets);
    __STD::swap(_M_num_elements, __ht._M_num_elements);
  }

  iterator begin()
  { // 找到第一个存值的节点, 构造迭代器; 
    for (size_type __n = 0; __n < _M_buckets.size(); ++__n)
      if (_M_buckets[__n])
        return iterator(_M_buckets[__n], this);
    return end();
  }

  iterator end() { return iterator(0, this); }

  const_iterator begin() const
  {
    for (size_type __n = 0; __n < _M_buckets.size(); ++__n)
      if (_M_buckets[__n])
        return const_iterator(_M_buckets[__n], this);
    return end();
  }

  const_iterator end() const { return const_iterator(0, this); }

#ifdef __STL_MEMBER_TEMPLATES
  template <class _Vl, class _Ky, class _HF, class _Ex, class _Eq, class _Al>
  friend bool operator== (const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al>&,
                          const hashtable<_Vl, _Ky, _HF, _Ex, _Eq, _Al>&);
#else /* __STL_MEMBER_TEMPLATES */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const hashtable&, const hashtable&);
#endif /* __STL_MEMBER_TEMPLATES */

public:
        // _M_buckets是vector<Node*>;
  size_type bucket_count() const { return _M_buckets.size(); }

  size_type max_bucket_count() const
    { return __stl_prime_list[(int)__stl_num_primes - 1]; } 

  size_type elems_in_bucket(size_type __bucket) const
  {    // 计算vector有多少卡槽被占用了;
    size_type __result = 0;
    for (_Node* __cur = _M_buckets[__bucket]; __cur; __cur = __cur->_M_next)
      __result += 1;
    return __result;
  }

  pair<iterator, bool> insert_unique(const value_type& __obj)
  { // 先检查是否需要扩容, 然后调用不可重复的插入;
    resize(_M_num_elements + 1);
    return insert_unique_noresize(__obj);
  }

  iterator insert_equal(const value_type& __obj)
  {
    resize(_M_num_elements + 1);
    return insert_equal_noresize(__obj);
  }

  pair<iterator, bool> insert_unique_noresize(const value_type& __obj);
  iterator insert_equal_noresize(const value_type& __obj);
 
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  void insert_unique(_InputIterator __f, _InputIterator __l)
  {
    insert_unique(__f, __l, __ITERATOR_CATEGORY(__f));
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator __f, _InputIterator __l)
  {
    insert_equal(__f, __l, __ITERATOR_CATEGORY(__f));
  }
    // 当迭代器类型是input_iterator类型时;
  template <class _InputIterator>
  void insert_unique(_InputIterator __f, _InputIterator __l,
                     input_iterator_tag)
  {
    for ( ; __f != __l; ++__f)
      insert_unique(*__f);
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator __f, _InputIterator __l,
                    input_iterator_tag)
  {
    for ( ; __f != __l; ++__f)
      insert_equal(*__f);
  }

  template <class _ForwardIterator>
  void insert_unique(_ForwardIterator __f, _ForwardIterator __l,
                     forward_iterator_tag)
  { // 先检查是否需要扩容 ,然后调用 insert_unique_noresize;
    size_type __n = 0;
    distance(__f, __l, __n);
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_unique_noresize(*__f);
  }

  template <class _ForwardIterator>
  void insert_equal(_ForwardIterator __f, _ForwardIterator __l,
                    forward_iterator_tag)
  {
    size_type __n = 0;
    distance(__f, __l, __n);
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_equal_noresize(*__f);
  }

#else /* __STL_MEMBER_TEMPLATES */
  void insert_unique(const value_type* __f, const value_type* __l)
  {
    size_type __n = __l - __f;
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_unique_noresize(*__f);
  }

  void insert_equal(const value_type* __f, const value_type* __l)
  {
    size_type __n = __l - __f;
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_equal_noresize(*__f);
  }

  void insert_unique(const_iterator __f, const_iterator __l)
  {
    size_type __n = 0;
    distance(__f, __l, __n);
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_unique_noresize(*__f);
  }

  void insert_equal(const_iterator __f, const_iterator __l)
  {
    size_type __n = 0;
    distance(__f, __l, __n);
    resize(_M_num_elements + __n);
    for ( ; __n > 0; --__n, ++__f)
      insert_equal_noresize(*__f);
  }
#endif /*__STL_MEMBER_TEMPLATES */

  reference find_or_insert(const value_type& __obj);
    
    // 通过键返回迭代器(内含node *cur->(*next, val));
  iterator find(const key_type& __key) 
  {
      // 首先通过hash函数计算key定义卡槽;
    size_type __n = _M_bkt_num_key(__key);
    _Node* __first;
    for ( __first = _M_buckets[__n];   // 提取key(node*->val) , 与 __key做比较;
          __first && !_M_equals(_M_get_key(__first->_M_val), __key);
          __first = __first->_M_next)
      {}      // __first找到了, 或者为nullptr
    return iterator(__first, this);
  } 

  const_iterator find(const key_type& __key) const
  {
    size_type __n = _M_bkt_num_key(__key);
    const _Node* __first;
      // 计算出元素的槽位, 遍历;
    for ( __first = _M_buckets[__n];
          __first && !_M_equals(_M_get_key(__first->_M_val), __key);
          __first = __first->_M_next)
      {}
    return const_iterator(__first, this);
  } 

  size_type count(const key_type& __key) const
  {
    const size_type __n = _M_bkt_num_key(__key);
    size_type __result = 0;
        // 计算出元素的槽位 计算有多少个元素
    for (const _Node* __cur = _M_buckets[__n]; __cur; __cur = __cur->_M_next)
      if (_M_equals(_M_get_key(__cur->_M_val), __key))
        ++__result;
    return __result;
  }

  pair<iterator, iterator> 
  equal_range(const key_type& __key);

  pair<const_iterator, const_iterator> 
  equal_range(const key_type& __key) const;

  size_type erase(const key_type& __key);
  void erase(const iterator& __it);
  void erase(iterator __first, iterator __last);

  void erase(const const_iterator& __it);
  void erase(const_iterator __first, const_iterator __last);

  void resize(size_type __num_elements_hint);
  void clear();

private:
  size_type _M_next_size(size_type __n) const
    { return __stl_next_prime(__n); }
    // 初始化 桶子;
  void _M_initialize_buckets(size_type __n)
  {
    const size_type __n_buckets = _M_next_size(__n);
    _M_buckets.reserve(__n_buckets);  // 设置桶子的数量;
    _M_buckets.insert(_M_buckets.end(), __n_buckets, (_Node*) 0);
    _M_num_elements = 0;
  }

  size_type _M_bkt_num_key(const key_type& __key) const
  {
    return _M_bkt_num_key(__key, _M_buckets.size());
  }

  size_type _M_bkt_num(const value_type& __obj) const
  {         // 通过get_key 取得key, 
    return _M_bkt_num_key(_M_get_key(__obj));
  }

  size_type _M_bkt_num_key(const key_type& __key, size_t __n) const
  {         // hash(key)-> 哈希值 % n;
    return _M_hash(__key) % __n;
  }

  size_type _M_bkt_num(const value_type& __obj, size_t __n) const
  {
    return _M_bkt_num_key(_M_get_key(__obj), __n);
  }

  _Node* _M_new_node(const value_type& __obj)
  {
    _Node* __n = _M_get_node();
    __n->_M_next = 0;
    __STL_TRY {
      construct(&__n->_M_val, __obj);
      return __n;
    }
    __STL_UNWIND(_M_put_node(__n));
  }
  
  void _M_delete_node(_Node* __n)
  {
    destroy(&__n->_M_val);
    _M_put_node(__n);
  }

  void _M_erase_bucket(const size_type __n, _Node* __first, _Node* __last);
  void _M_erase_bucket(const size_type __n, _Node* __last);

  void _M_copy_from(const hashtable& __ht);

};

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
_Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&
_Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++()
{
  const _Node* __old = _M_cur;
  _M_cur = _M_cur->_M_next;
    //如果元素不存在,切换下一个bucket
  if (!_M_cur) {
    size_type __bucket = _M_ht->_M_bkt_num(__old->_M_val);
    while (!_M_cur && ++__bucket < _M_ht->_M_buckets.size())
      _M_cur = _M_ht->_M_buckets[__bucket];
  }
  return *this;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>
_Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++(int)
{
  iterator __tmp = *this;
  ++*this;
  return __tmp;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
_Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&
_Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++()
{
  const _Node* __old = _M_cur;
  _M_cur = _M_cur->_M_next;
  if (!_M_cur) {
    size_type __bucket = _M_ht->_M_bkt_num(__old->_M_val);
    while (!_M_cur && ++__bucket < _M_ht->_M_buckets.size())
      _M_cur = _M_ht->_M_buckets[__bucket];
  }
  return *this;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>
_Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++(int)
{
  const_iterator __tmp = *this;
  ++*this;
  return __tmp;
}

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
  return forward_iterator_tag();
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Val* 
value_type(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
  return (_Val*) 0;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const _Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
  return (hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*) 0;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const _Hashtable_const_iterator<_Val,_Key,_HF,
                                                  _ExK,_EqK,_All>&)
{
  return forward_iterator_tag();
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline _Val* 
value_type(const _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
  return (_Val*) 0;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, 
          class _All>
inline hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>&)
{
  return (hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>::difference_type*) 0;
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
bool operator==(const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht1,
                const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht2)
{
  typedef typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::_Node _Node;
  if (__ht1._M_buckets.size() != __ht2._M_buckets.size())
    return false;
  for (int __n = 0; __n < __ht1._M_buckets.size(); ++__n) {
    _Node* __cur1 = __ht1._M_buckets[__n];
    _Node* __cur2 = __ht2._M_buckets[__n];
    for ( ; __cur1 && __cur2 && __cur1->_M_val == __cur2->_M_val;
          __cur1 = __cur1->_M_next, __cur2 = __cur2->_M_next)
      {}
    if (__cur1 || __cur2)
      return false;
  }
  return true;
}  

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
inline bool operator!=(const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht1,
                       const hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>& __ht2) {
  return !(__ht1 == __ht2);
}

template <class _Val, class _Key, class _HF, class _Extract, class _EqKey, 
          class _All>
inline void swap(hashtable<_Val, _Key, _HF, _Extract, _EqKey, _All>& __ht1,
                 hashtable<_Val, _Key, _HF, _Extract, _EqKey, _All>& __ht2) {
  __ht1.swap(__ht2);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */


template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator, bool> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::insert_unique_noresize(const value_type& __obj)
{
  const size_type __n = _M_bkt_num(__obj); // 根据 val提取出key计算出坐标;
  _Node* __first = _M_buckets[__n];

  for (_Node* __cur = __first; __cur; __cur = __cur->_M_next)       
    if (_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj)))    // get_key 完成提取key
      return pair<iterator, bool>(iterator(__cur, this), false);

  _Node* __tmp = _M_new_node(__obj);
  __tmp->_M_next = __first;
  _M_buckets[__n] = __tmp;
  ++_M_num_elements;
  return pair<iterator, bool>(iterator(__tmp, this), true);
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::insert_equal_noresize(const value_type& __obj)
{
  //计算出节点的index， 然后first指向槽
  const size_type __n = _M_bkt_num(__obj);
  _Node* __first = _M_buckets[__n];

  for (_Node* __cur = __first; __cur; __cur = __cur->_M_next) 
    if (_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj))) {
      // 找到于对应 键值相等的元素， 然后插入；
      _Node* __tmp = _M_new_node(__obj);
      __tmp->_M_next = __cur->_M_next;
      __cur->_M_next = __tmp;
      ++_M_num_elements;
      return iterator(__tmp, this);
    }
    //没有重复的键值， 直接插入；
  _Node* __tmp = _M_new_node(__obj);
  __tmp->_M_next = __first;
  _M_buckets[__n] = __tmp;
  ++_M_num_elements;
  return iterator(__tmp, this);
}

// 如果没找到就插入元素;
template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::reference 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::find_or_insert(const value_type& __obj)
{ // 先进行安全检查;
  resize(_M_num_elements + 1);
    // 计算出 正确坐标;
  size_type __n = _M_bkt_num(__obj);
  _Node* __first = _M_buckets[__n];
    // 遍历该卡槽上的链表;
  for (_Node* __cur = __first; __cur; __cur = __cur->_M_next)
    if (_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj)))
      return __cur->_M_val;   // 当发现当前值存在,返回已存在值的引用;
    // 否则插入卡槽首位,返回;
  _Node* __tmp = _M_new_node(__obj);
  __tmp->_M_next = __first;
  _M_buckets[__n] = __tmp;
  ++_M_num_elements;
  return __tmp->_M_val;
}

// equal_range 返回该键元素的 起 与止
template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator,
     typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::equal_range(const key_type& __key)
{
    // 计算出元素的槽位
  typedef pair<iterator, iterator> _Pii;
  const size_type __n = _M_bkt_num_key(__key);

  for (_Node* __first = _M_buckets[__n]; __first; __first = __first->_M_next)
    if (_M_equals(_M_get_key(__first->_M_val), __key)) { // 找到起 位
      for (_Node* __cur = __first->_M_next; __cur; __cur = __cur->_M_next)
        if (!_M_equals(_M_get_key(__cur->_M_val), __key)) // 找到 止位
          return _Pii(iterator(__first, this), iterator(__cur, this));
      for (size_type __m = __n + 1; __m < _M_buckets.size(); ++__m)
        if (_M_buckets[__m])   //如果该卡槽链表后面都等于 __key
          return _Pii(iterator(__first, this),
                     iterator(_M_buckets[__m], this));
      return _Pii(iterator(__first, this), end()); // 该卡槽链表后面都等于 __key且已是最后;
    }
  return _Pii(end(), end()); // 没有发现;
}

//返回const_iterator 的重载版本;
template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::const_iterator, 
     typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::const_iterator> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::equal_range(const key_type& __key) const
{
  typedef pair<const_iterator, const_iterator> _Pii;
  const size_type __n = _M_bkt_num_key(__key);

  for (const _Node* __first = _M_buckets[__n] ;
       __first; 
       __first = __first->_M_next) {
    if (_M_equals(_M_get_key(__first->_M_val), __key)) {
      for (const _Node* __cur = __first->_M_next;
           __cur;
           __cur = __cur->_M_next)
        if (!_M_equals(_M_get_key(__cur->_M_val), __key))
          return _Pii(const_iterator(__first, this),
                      const_iterator(__cur, this));
      for (size_type __m = __n + 1; __m < _M_buckets.size(); ++__m)
        if (_M_buckets[__m])
          return _Pii(const_iterator(__first, this),
                      const_iterator(_M_buckets[__m], this));
      return _Pii(const_iterator(__first, this), end());
    }
  }
  return _Pii(end(), end());
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::size_type 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const key_type& __key)
{
  const size_type __n = _M_bkt_num_key(__key);
  _Node* __first = _M_buckets[__n];
  size_type __erased = 0;

  if (__first) {
    _Node* __cur = __first;
    _Node* __next = __cur->_M_next;
    while (__next) {
      if (_M_equals(_M_get_key(__next->_M_val), __key)) {
        __cur->_M_next = __next->_M_next;
        _M_delete_node(__next);
        __next = __cur->_M_next;
        ++__erased;
        --_M_num_elements;
      }
      else {
        __cur = __next;
        __next = __cur->_M_next;
      }
    }
    if (_M_equals(_M_get_key(__first->_M_val), __key)) {
      _M_buckets[__n] = __first->_M_next;
      _M_delete_node(__first);
      ++__erased;
      --_M_num_elements;
    }
  }
  return __erased;
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const iterator& __it)
{
  _Node* __p = __it._M_cur;
  if (__p) {
    const size_type __n = _M_bkt_num(__p->_M_val);
    _Node* __cur = _M_buckets[__n];

    if (__cur == __p) {
      _M_buckets[__n] = __cur->_M_next;
      _M_delete_node(__cur);
      --_M_num_elements;
    }
    else {
      _Node* __next = __cur->_M_next;
      while (__next) {
        if (__next == __p) {
          __cur->_M_next = __next->_M_next;
          _M_delete_node(__next);
          --_M_num_elements;
          break;
        }
        else {
          __cur = __next;
          __next = __cur->_M_next;
        }
      }
    }
  }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::erase(iterator __first, iterator __last)
{   // 指定迭起器范围进行删除, 首先计算所指 桶的下标;
  size_type __f_bucket = __first._M_cur ? 
    _M_bkt_num(__first._M_cur->_M_val) : _M_buckets.size();
  size_type __l_bucket = __last._M_cur ? 
    _M_bkt_num(__last._M_cur->_M_val) : _M_buckets.size();

  if (__first._M_cur == __last._M_cur)
    return;
  else if (__f_bucket == __l_bucket)
    // 当删除迭代器的范围在一个桶内;
    _M_erase_bucket(__f_bucket, __first._M_cur, __last._M_cur);
  else {
    _M_erase_bucket(__f_bucket, __first._M_cur, 0);
    for (size_type __n = __f_bucket + 1; __n < __l_bucket; ++__n)
      _M_erase_bucket(__n, 0);
    if (__l_bucket != _M_buckets.size())
      _M_erase_bucket(__l_bucket, __last._M_cur);
  }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
inline void
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const_iterator __first,
                                             const_iterator __last)
{
  erase(iterator(const_cast<_Node*>(__first._M_cur),
                 const_cast<hashtable*>(__first._M_ht)),
        iterator(const_cast<_Node*>(__last._M_cur),
                 const_cast<hashtable*>(__last._M_ht)));
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
inline void
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::erase(const const_iterator& __it)
{
  erase(iterator(const_cast<_Node*>(__it._M_cur),
                 const_cast<hashtable*>(__it._M_ht)));
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::resize(size_type __num_elements_hint)
{
  const size_type __old_n = _M_buckets.size();
  // 如果 元素数量大于 buckets的大小;
  if (__num_elements_hint > __old_n) {
      //找到下一个质数
    const size_type __n = _M_next_size(__num_elements_hint);
    if (__n > __old_n) {
      //分配新的 buckets;
      vector<_Node*, _All> __tmp(__n, (_Node*)(0),
                                 _M_buckets.get_allocator());
      __STL_TRY {
        for (size_type __bucket = 0; __bucket < __old_n; ++__bucket) {
          _Node* __first = _M_buckets[__bucket];
          //找到每一个槽的头节点;
          while (__first) {
              //利用hash计算出在新buck的下标;
            size_type __new_bucket = _M_bkt_num(__first->_M_val, __n);
            _M_buckets[__bucket] = __first->_M_next; // 原槽指向下一个节点;
            __first->_M_next = __tmp[__new_bucket]; // 头插法;
            __tmp[__new_bucket] = __first;
            __first = _M_buckets[__bucket]; //令__frist再指回原槽节点;
          }
        }
        _M_buckets.swap(__tmp);
      }
#         ifdef __STL_USE_EXCEPTIONS
      catch(...) {
        for (size_type __bucket = 0; __bucket < __tmp.size(); ++__bucket) {
          while (__tmp[__bucket]) {
            _Node* __next = __tmp[__bucket]->_M_next;
            _M_delete_node(__tmp[__bucket]);
            __tmp[__bucket] = __next;
          }
        }
        throw;
      }
#         endif /* __STL_USE_EXCEPTIONS */
    }
  }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::_M_erase_bucket(const size_type __n, _Node* __first, _Node* __last)
{
  _Node* __cur = _M_buckets[__n];
  if (__cur == __first)
    _M_erase_bucket(__n, __last);
  else {
    _Node* __next;
    for (__next = __cur->_M_next; 
         __next != __first; 
         __cur = __next, __next = __cur->_M_next)
      ;
    while (__next != __last) {
      __cur->_M_next = __next->_M_next;
      _M_delete_node(__next);
      __next = __cur->_M_next;
      --_M_num_elements;
    }
  }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::_M_erase_bucket(const size_type __n, _Node* __last)
{
  _Node* __cur = _M_buckets[__n];
  while (__cur != __last) {
    _Node* __next = __cur->_M_next;
    _M_delete_node(__cur);
    __cur = __next;
    _M_buckets[__n] = __cur;
    --_M_num_elements;
  }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::clear()
{
  for (size_type __i = 0; __i < _M_buckets.size(); ++__i) {
    _Node* __cur = _M_buckets[__i];
    while (__cur != 0) {
      _Node* __next = __cur->_M_next;
      _M_delete_node(__cur); // 删除当前节点
      __cur = __next;   // 当前节点定位到next
    }
    _M_buckets[__i] = 0; // 槽位设空;
  }
  _M_num_elements = 0;
    // 注意 buckets并未释放空间， 仍未原大小；
}

    
template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::_M_copy_from(const hashtable& __ht)
{
    // 将己方 的空间清除， 然后设置的和对方一样大；
  _M_buckets.clear();
  _M_buckets.reserve(__ht._M_buckets.size());
    // 设置bucket所有的槽为 nullptr
  _M_buckets.insert(_M_buckets.end(), __ht._M_buckets.size(), (_Node*) 0);
  __STL_TRY {
    for (size_type __i = 0; __i < __ht._M_buckets.size(); ++__i) {
      const _Node* __cur = __ht._M_buckets[__i];
      if (__cur) {
          //如果当前节点不为 空， 复制节点到自己的bucket位置；
        _Node* __copy = _M_new_node(__cur->_M_val);
        _M_buckets[__i] = __copy;
         //将原槽位所有节点复制过来;
        for (_Node* __next = __cur->_M_next; 
             __next; 
             __cur = __next, __next = __cur->_M_next) {
          __copy->_M_next = _M_new_node(__next->_M_val);
          __copy = __copy->_M_next;
        }
      }
    }
    _M_num_elements = __ht._M_num_elements;
  }
  __STL_UNWIND(clear()); // 就是定义了这个 define __STL_UNWIND(action) catch(...) { action; throw; }
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_HASHTABLE_H */

// Local Variables:
// mode:C++
// End:
