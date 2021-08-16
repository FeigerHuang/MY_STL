/*************************************************************************
	> File Name: my_function.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Mon 16 Aug 2021 04:00:16 PM CST
 ************************************************************************/

#ifndef _MY_FUNCTION_H
#define _MY_FUNCTION_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

//每个 Adaptable Unary Function 都应继承此一元函数类别;
template <class Arg, class Result>
struct unary_function{
    typedef Arg argument_type;
    typedef Result result_type;
};

//二元仿函数 继承此类别;
template <class Arg1, class Arg2, class Result>
struct binary_function{
    typedef Arg1    first_argument_type;
    typedef Arg2    second_argument_type;
    typedef Result  result_type;
};

// 算术类仿函数的实现 BEGIN

template <class T>
struct plus : public binary_function<T, T, T>
{
    T operator()(const T& x, const T& y) const {
        return x + y;
    } 
};

template <class T>
struct minus : public binary_function<T, T, T>
{
    T operator()(const T& x, const T& y) const {
        return x - y;
    }
};

template<class T>
struct multiplies : public binary_function<T, T, T>
{
    T operator()(const T& x ,const T& y) const {
        return x * y;
    }
};

template<class T>
struct divides : public binary_function<T, T, T>
{
    T operator()(const T& x, const T& y) const {
        return x / y;
    }  
};

template<class T>
struct modulus : public binary_function<T, T, T>
{
    T operator()(const T& x, const T& y) const {
        return x % y;
    }
};

template <class T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x) const {
        return -x;
    }
};


// 算术类仿函数的实现 END 

// 关系类仿函数 BEGIN

template <class T>
struct equal_to : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x == y;
    }
};

template <class T>
struct not_equal_to : public binary_function<T, T, bool> 
{
    bool operator()(const T& x, const T& y) const {
        return x != y;
    }
};

template <class T>
struct greater : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x > y;
    }  
};

template <class T>
struct greater_equal : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x >= y;
    }  
};

template <class T>
struct less : public binary_function<T, T, bool> 
{
    bool operator()(const T& x ,const T& y) const {
        return x < y;
    }  
};

template <class T>
struct less_euqal : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x <= y;
    }
};

// 关系类仿函数 END;

// 逻辑运算仿函数 BEGIN

template <class T>
struct logic_and : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x && y;
    }  
};

template <class T>
struct logic_or : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return x || y;
    }
};
    
template <class T>
struct logic_not : public unary_function<T, bool>
{
    bool operator()(const T& x) const {
        return !x;
    }
};

// 逻辑运算仿函数 END;

// 其他 仿函数 BEGIN

template <class T>
struct identify : public unary_function<T, T> {
    const T& operator()(const T& x) const {
        return x;
    }
};

//选择 pair的第一个元素;
template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type>
{
    const typename Pair::first_type& operator()(const Pair& x) const {
        return x.first;
    }
};

//选择 pair的第二个元素;
template <class Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type>
{
    const typename Pair::second_type& operator()(const Pair& x) const {
        return x.second;
    }
};

// 投射函数 返回第一个参数
template <class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1>
{
    Arg1 operator()(const Arg1& x, const Arg2& y) const {
        return x;
    }  
};

// 投射函数 返回第二个参数
template <class Arg1, class Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2>
{
    Arg2 operator()(const Arg1& x, const Arg2& y) const {
        return y;
    }  
};

// 其他 仿函数 END;
_MY_NAMESPACE_END

#endif
