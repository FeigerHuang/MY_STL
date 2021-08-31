/*************************************************************************
	> File Name: my_rbtree.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Tue 31 Aug 2021 07:29:49 PM CST
 ************************************************************************/

#ifndef _MY_RBTREE_H
#define _MY_RBTREE_H

#ifdef BUG_RBT
#define LOG_RBT(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_RBT(msg)
#endif
#define BLU  "\033[34m"
#define YEL  "\033[33m"
#define RED  "\033[31m"
#define FIN   "\033[0m"

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }
#include "my_pair.h"
#include "my_function.h"
_MY_NAMESPACE_BEGIN


// 红黑树节点定义
template<class Value>
struct TreeNode {
    typedef  Value              value_type;
    typedef  unsigned int       color_type;
    typedef  TreeNode<Value>*   link_type;
    value_type     value;
    color_type     color;
    link_type      lchild;
    link_type      rchild;
    TreeNode(value_type x, color_type c = 0, link_type l = nullptr, link_type r = nullptr)
        : value(x), color(c), lchild(l), rchild(r) 
    {
        LOG_RBT("Node construct done x = " << x << " ,color = " << c);    
    }
    ~TreeNode() {
        LOG_RBT("~Node() done");
    }
};

// 红黑树专属空间配置器;
template<class TP, class Alloc = super_allocator<TP> >
class RB_Tree_Alloc {
public:
    typedef Alloc          node_allocator;
    typedef TreeNode<TP>*  pointer;
protected:
    pointer get_node() {
        void *result = node_allocator::allocate(sizeof(TP));
        return static_cast<pointer>(result);
    }
    void put_node(pointer x) {
        node_allocator::deallocate((void *)x, sizeof(TP));
        return ;
    }
};

template<class Key, class Value, class ExtractKey = identify<Key>, class EqualKey = equal_to<Key>,
        class Alloc = super_allocator<Value> >
class RB_Tree : protected RB_Tree_Alloc<Value, Alloc>  {
public:
    enum color{red = 0, black = 1, dblack = 2};
    typedef Key                         key_type;
    typedef Value                       value_type;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef value_type*                 pointer;
    typedef const value_type            const_pointer;
    typedef value_type&                 reference;
    typedef const value_type            const_reference;
    typedef RB_Tree_Alloc<Value, Alloc> Base;
    typedef TreeNode<Value>*            link_type;

protected:
    using Base::get_node;
    using Base::put_node;

private:
    TreeNode<Value>*  root;
    EqualKey          equals;
    ExtractKey        get_key;
public:
    RB_Tree() : root(NIL) , equals(EqualKey()), get_key(ExtractKey()) {}
    void insert_unique(const value_type& x); 
    
    void range();
protected:
    link_type __insert_unique(link_type node, const value_type& x);
    link_type insert_maintain(link_type node);
    bool has_red_child(link_type node);
    link_type left_rotate(link_type node);
    link_type right_rotate(link_type node);
    void __preorder(link_type node);
private:
public:
    // 配置一个值为x的节点
    link_type new_node(const value_type& x) {
        link_type tmp = get_node();
        construct(tmp, x);
        tmp->color = red;
        tmp->lchild = (tmp->rchild = NIL);
        return tmp;
    }
    // 释放一个节点;
    void delete_node(link_type p) {
        destroy(p);
        put_node(p);
        return ;
    }
    
    static TreeNode<Value> __NIL;
    static TreeNode<Value>* NIL;
};

// 定义类属性共用一个NIL指针替代 nullptr指针;
template<class Key, class Value, class Extr, class Eq,  class Alloc>
TreeNode<Value> RB_Tree<Key, Value, Extr, Eq, Alloc>::__NIL(Value(), black, &__NIL, &__NIL);
// NIL叶子节点颜色全部为黑色;
template<class Key, class Value,class Extr, class Eq, class Alloc>
TreeNode<Value>* RB_Tree<Key, Value,Extr, Eq, Alloc>::NIL = &__NIL;
//-----------------------RB_TREE成员方法实现----------------------------//
    
// 向红黑树中插入一个元素;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::insert_unique(const value_type& x) {
    root = __insert_unique(root, x);
    root->color = black;
    return ;
}

// 封装一成插入函数;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__insert_unique(link_type node, const value_type& x) {
    if (node == NIL) return new_node(x);
    if (equals(get_key(x), get_key(node->value)) ) return node;
    if (get_key(x) < get_key(node->value)) {
        node->lchild = __insert_unique(node->lchild, x); 
        LOG_RBT("node->lchild->v = " << node->lchild->value);
    } else {
        node->rchild = __insert_unique(node->rchild, x);
    }
    return node;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
inline
bool RB_Tree<Key, Value, Extr, Eq, Alloc>::has_red_child(link_type node) {
    return node->lchild->color != red && node->rchild->color != red;
}

// 左旋及右旋的封装;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::left_rotate(link_type node) {
    link_type tmp = node->rchild;
    node->rchild = tmp->lchild;
    tmp->lchild = node;
    return tmp;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::right_rotate(link_type node) {
    link_type tmp = node->lchild;
    node->lchild = tmp->rchild;
    tmp->rchild = node;
    return tmp;
}

// 插入调整操作, 祖父视角, 并且目标消除双红;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::insert_maintain(link_type node) {
    if(!has_red_child(node)) return node;
    if (node->lchild->color == red && node->rchild->color == red) {
        // 当孩子都是黑色直接返回;
        if (!has_red_child(node->lchild) && !has_red_child(node->rchild)) 
            return node;
        // 否则进行红色上浮;
        node->color = red;
        node->lchild->color = node->rchild->color = black;
        return node;
    } 
    // 当有一个红色和一个黑色孩子时;
    if (node->lchild->color == red) {
        if (!has_red_child(node->lchild)) return node;
        // LR 时
        if (node->lchild->rchild->color == red) {
            node->lchild = left_rotate(node->lchild);
        } // LL
        node = right_rotate(node);
    } else { // RL
        if (!has_red_child(node->rchild)) return node;
        if (node->rchild->lchild->color == red) {
            node->rchild = right_rotate(node->rchild);
        } //RR
        node = left_rotate(node);
    }
    // 进行红色上浮操作;
    node->color = red;
    node->lchild->color = black;
    node->rchild->color = black;
    return node;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::range() {
    __preorder(root);
    return ;
}

// 前序遍历扫描一遍RB_Tree;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::__preorder(link_type node) {
    if (node == NIL) return;
    printf("[%s]%d| %d, %d\n", node->color == red ? "red" : "blk", 
           node->value, node->lchild->value, node->rchild->value);
    __preorder(node->lchild);
    __preorder(node->rchild);
}


_MY_NAMESPACE_END
#undef LOG_RBT
#endif
