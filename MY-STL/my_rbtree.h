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
    TreeNode(const value_type& x, color_type c = 0, link_type l = nullptr, link_type r = nullptr)
        : value(x), color(c), lchild(l), rchild(r) 
    {
        LOG_RBT("Node construct done x = " << x << " ,color = " << c);    
    }
    ~TreeNode() {
        LOG_RBT("~Node() done");
    }
};

// 红黑树专属空间配置器;
template<class TP, class Alloc >
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
        node_allocator::deallocate(x, sizeof(TP));
        return ;
    }
};

template<class Key, class Value, class ExtractKey = identify<Key>, class EqualKey = equal_to<Key>,
        class Alloc = simple_alloc<TreeNode<Value>> >
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
    typedef const link_type             const_link;

    typedef EqualKey                    key_equal;
protected:
    using Base::get_node;
    using Base::put_node;

private:
    TreeNode<Value>*  root;
    EqualKey          equals;
    ExtractKey        get_key;
    size_type         M_node_cnt;
public:
    RB_Tree() 
        : root(NIL) , equals(EqualKey()), get_key(ExtractKey()) , M_node_cnt(0){}
    RB_Tree(const EqualKey& cmp) 
        : root(NIL), equals(cmp), get_key(ExtractKey()), M_node_cnt(0) {}
    ~RB_Tree() {
        this->clear();
        return;
    }
    void insert_unique(const value_type& x); 
    void erase_unique(const value_type& x); 
    void insert_equal(const value_type& x);
    void clear();
    void range();
    void inorder();
    size_type size() const {return M_node_cnt;}
    bool empty() const {return 0 == M_node_cnt;}
    bool find(const value_type& x);
    const link_type find_or_insert(const value_type& x);
protected:
    link_type __find_or_insert(link_type node, const value_type& x, link_type& p);
    link_type __find(link_type node, const value_type& x);
    link_type __insert_unique(link_type node, const value_type& x);
    link_type __insert_equal(link_type node, const value_type& x);
    link_type insert_maintain(link_type node);
    link_type __erase_unique(link_type node, const value_type& x);
    link_type erase_maintain(link_type node);
    bool has_red_child(link_type node);
    link_type left_rotate(link_type node);
    link_type right_rotate(link_type node);
    link_type predecessor(link_type node);
    link_type successor(link_type node);
    void __preorder(link_type node);
    void __clear(link_type node);
private:
    // 配置一个值为x的节点
    link_type new_node(const value_type& x) {
        link_type tmp = get_node();
        construct(tmp, x);
        tmp->color = red;
        tmp->lchild = NIL;
        tmp->rchild = NIL;
        LOG_RBT("alloc tmp->value = " << tmp->value); 
        ++M_node_cnt;
        return tmp;
    }
    // 释放一个节点;
    void delete_node(link_type p) {
        destroy(p);
        put_node(p);
        --M_node_cnt;
        return ;
    }
    
    static TreeNode<Value> __NIL;
    static TreeNode<Value>* NIL;
};

// 定义类属性共用一个NIL指针替代 nullptr指针;
template<class Key, class Value, class Extr, class Eq,  class Alloc>
TreeNode<Value> RB_Tree<Key, Value, Extr, Eq, Alloc>::__NIL(Value(), black, &(__NIL), &(__NIL));
// NIL叶子节点颜色全部为黑色;
template<class Key, class Value,class Extr, class Eq, class Alloc>
TreeNode<Value>* RB_Tree<Key, Value,Extr, Eq, Alloc>::NIL = &__NIL;
//-----------------------RB_TREE成员方法实现----------------------------//
    
// 向红黑树中插入一个元素;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::insert_unique(const value_type& x) {
    this->root = __insert_unique(root, x);
    root->color = black;
    return ;
}


// 搜寻或者插入元素;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::const_link
RB_Tree<Key, Value, Extr, Eq, Alloc>::find_or_insert(const value_type& x) {
    link_type result;
    __find_or_insert(root, x, result);
    return (const_link)result;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__find_or_insert(link_type node, const value_type& x, link_type& result) {
    if (node == NIL) {
        result = new_node(x);
        return result;
    }
    if (equals(get_key(x), get_key(node->value)) ) {
        return node;
    } 
    else if (get_key(node->value) < get_key(x) ) {
        node->rchild = __find_or_insert(node->rchild, x, result);
    } 
    else {
        node->lchild = __find_or_insert(node->lchild, x, result);
    }
    return insert_maintain(node);
}

// 从红黑树中查找元素;
template <class Key, class Value, class Extr, class Eq, class Alloc>
bool RB_Tree<Key, Value, Extr, Eq, Alloc>::find(const value_type& x) {
    link_type result = __find(root, x);
    return result != NIL;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__find(link_type node, const value_type& x) {
    if (node == NIL) return NIL;
    if (equals(get_key(node->value), get_key(x)) ) {
        return node;
    } 
    else if (get_key(node->value) < get_key(x)) {
        return __find(node->rchild, x);
    } else {
        return __find(node->lchild, x);
    }
}

// 封装一成插入函数;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__insert_unique(link_type node, const value_type& x) {
    if (node == NIL) return new_node(x);
    if (equals(get_key(x), get_key(node->value)) ) return node;
    else if (get_key(x) < get_key(node->value)) {
        node->lchild = __insert_unique(node->lchild, x); 
    } else {
        node->rchild = __insert_unique(node->rchild, x);
    }
    return insert_maintain(node);
}

// 可重复性插入
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::insert_equal(const value_type& x) {
    this->root = __insert_equal(root, x);
    root->color = black;
    return ;
}

// 左孩子key小于当前node->key, 右孩子大于等于当前key;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__insert_equal(link_type node, const value_type& x) {
    if (node == NIL) return new_node(x);
    if (equals(get_key(node->value), get_key(x))) {
        link_type tmp = new_node(x);
        tmp->rchild = node->rchild;
        node->rchild = tmp;
    } else if (get_key(node->value) < get_key(x) ) {
        //LOG_RBT("access to rchild");
        node->rchild = __insert_equal(node->rchild, x);
    } else {
        node->lchild = __insert_equal(node->lchild, x);
    }
    return insert_maintain(node);
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
inline
bool RB_Tree<Key, Value, Extr, Eq, Alloc>::has_red_child(link_type node) {
    return node->lchild->color == red || node->rchild->color == red;
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

// 前驱及后继节点的查找;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::predecessor(link_type node) {
    link_type tmp = node->lchild;
    while (tmp->rchild != NIL) tmp = tmp->rchild;
    return tmp;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::successor(link_type node) {
    link_type tmp = node->rchild;
    while (tmp->lchild != NIL) tmp = tmp->lchild;
    return tmp;
}

// 红黑树删除调整, 删除调整的目的干掉双重黑,站在父节点视角;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::erase_maintain(link_type node) {
    // 当没有双重黑节点时;
    if (node->lchild->color != dblack && node->rchild->color != dblack) return node;
    // 下面都是存在双重黑的情况, 当双重黑的兄弟为红色时;
    if (has_red_child(node)) {
        bool flg = false;
        node->color = red;
        if (node->lchild->color == red) {
            flg = true;
            node = right_rotate(node);
        } else {
            node = left_rotate(node);
        }
        node->color = black;
        if (flg) { // 此时dblck节点下沉到了右边, 递归到右边调整;
            node->rchild = erase_maintain(node->rchild);
        } else {
            node->lchild = erase_maintain(node->lchild);
        }
        return node;
    }
    // 当dblack的兄弟节点为黑色的情况
    if ((node->lchild->color == black && !has_red_child(node->lchild)) ||
        (node->rchild->color == black && !has_red_child(node->rchild)) )
    {  // 当黑色兄弟下面没有红色节点, 进行黑色上浮操作;
        node->color += 1;
        node->lchild->color -= 1;
        node->rchild->color -= 1;
        return node; // 交给上面继续调整;
    }
    if (node->lchild->color == dblack) {
        node->lchild->color = black;
        if (node->rchild->rchild->color != red) { // RL
            node->rchild->color = red;
            node->rchild = right_rotate(node->rchild);
            node->rchild->color = black;
        } // RR
        node = left_rotate(node);
        node->color = node->lchild->color;
    } else {
        node->rchild->color = black;
        if (node->lchild->lchild->color != red) { // LR
            node->lchild->color = red;
            node->lchild = left_rotate(node->lchild);
            node->lchild->color = black;
        }
        node = right_rotate(node);
        node->color = node->rchild->color;
    } // 新根颜色改原根, 左右孩子全黑;
    node->lchild->color = node->rchild->color = black;
    return node;
}

// 红黑树删除操作;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::erase_unique(const value_type& x) {
    this->root = __erase_unique(root, x);
    root->color = black;
    return ;
}

// 红黑树删除操作;
template<class Key, class Value,class Extr, class Eq, class Alloc>
typename RB_Tree<Key, Value, Extr, Eq, Alloc>::link_type
RB_Tree<Key, Value, Extr, Eq, Alloc>::__erase_unique(link_type node, const value_type& x) {
    if (node == NIL) return NIL;
    if (equals(get_key(x), get_key(node->value)) ) {
        // 当删除节点度为0 或者 为1;
        if (node->lchild == NIL || node->rchild == NIL) {
            link_type tmp = (node->lchild == NIL ? node->rchild : node->lchild);
            tmp->color += node->color; // 将删除节点的颜色加到其子节点;以一抵三;
            delete_node(node);
            return tmp;
        } else {
            link_type tmp = predecessor(node);
            node->value = tmp->value;
            node->lchild = __erase_unique(node->lchild, tmp->value);
            return node;
        }
    } 
    else if (get_key(x) < get_key(node->value)) {
        node->lchild = __erase_unique(node->lchild, x);
    } else {
        node->rchild = __erase_unique(node->rchild, x);
    }
    return erase_maintain(node);
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
    LOG_RBT( (node->color == red ? "red" : "blk") << 
           node->value << "|" << node->lchild->value << ", " << node->rchild->value << "|");
    __preorder(node->lchild);
    __preorder(node->rchild);
}

// 中序遍历扫描一遍RB_Tree;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::inorder() {
    link_type cur = root;
    std::cerr << "{ ";
    while (cur != NIL) {
        if (cur->lchild == NIL) {
            std::cerr << cur->value << ", ";
            cur = cur->rchild;
        }
        else {
            link_type tmp = cur->lchild;
            while (tmp->rchild != NIL && tmp->rchild != cur) {
                tmp = tmp->rchild;
            }
            if (tmp->rchild == NIL) {
                tmp->rchild = cur;
                cur = cur->lchild;
            } else {
                tmp->rchild = NIL;
                std::cerr << cur->value << ", ";
                cur = cur->rchild;
            }
        }
    }
    std::cerr << " }";
    return ;
}

// 清除所有节点;
template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::clear() {
    __clear(this->root);
    root = NIL;
    return ;
}

template<class Key, class Value,class Extr, class Eq, class Alloc>
void RB_Tree<Key, Value, Extr, Eq, Alloc>::__clear(link_type x) {
    if (x == NIL) return;
    __clear(x->lchild);
    __clear(x->rchild);
    delete_node(x);
    return ;
}

_MY_NAMESPACE_END
#undef LOG_RBT
#endif
