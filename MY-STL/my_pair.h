/*************************************************************************
	> File Name: my_pair.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Thu 19 Aug 2021 08:49:26 PM CST
 ************************************************************************/

#ifndef _MY_PAIR_H
#define _MY_PAIR_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN
// pair 实现BEGIN
template <class T, class U>
struct pair {
    typedef T first_type;
    typedef U second_type;
    T first;
    U second;

    pair() : first(T()), second(U()) {}
    pair(const T& x, const U& y) : first(x), second(y) {}

    template<class T2, class U2>
    pair(const pair<T2, U2>& rhs) : first(rhs.first) , second(rhs.second) {}
};

template <class T, class U>
inline
bool operator==(const pair<T,U>& x, const pair<T, U>& y) {
    return x.first == y.first && x.second == y.second;
}

template <class T, class U>
inline 
bool operator!=(const pair<T, U>& x, const pair<T, U>& y) {
    return !(x == y);
}

template <class T, class U>
inline 
bool operator<(const pair<T, U>& x, const pair<T, U>& y) {
    return  x.first < y.first || 
        (!(y.first < x.first) && x.second < y.second);
}

template <class T, class U>
inline
bool operator>(const pair<T, U>& x, const pair<T, U>& y){
    return y < x;
}

template<class T, class U>
inline
bool operator<=(const pair<T, U>& x, const pair<T,U>& y) {
    return !(y < x);
}

template<class T, class U>
inline 
bool operator>=(const pair<T, U>& x, const pair<T, U>& y){
    return !(x < y);
}

template<class T, class U>
inline 
pair<T, U> make_pair(const T& x, const T& y) {
    return pair<T, U>(x, y);
}

// pair 实现END

_MY_NAMESPACE_END


#endif
