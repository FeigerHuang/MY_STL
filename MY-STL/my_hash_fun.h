/*************************************************************************
	> File Name: my_hash_fun.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 18 Aug 2021 08:07:34 PM CST
 ************************************************************************/

#ifndef _MY_HASH_FUN_H
#define _MY_HASH_FUN_H

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

template <class Key>
struct hash {};

// 字符串统一使用BKDR_HASH
inline 
size_t BKDRHash(const char *s) {
    size_t seed = 31;
    size_t h = 0;
    for (int i = 0; s[i]; ++i) {
        h = h * seed + s[i];
    }
    return h & 0x7fffffff;
}

template<>
struct hash<char *>
{
    size_t operator()(const char *s) const {
        return BKDRHash(s);
    }  
};

template<>
struct hash<const char *>
{
    size_t operator()(const char *s) const {
        return BKDRHash(s);
    }  
};

template<>
struct hash<char>
{
    size_t operator()(char x) const {
        return x;
    }  
};

template<>
struct hash<const char>
{
    size_t operator()(char x) const {
        return x;
    }  
};

template<>
struct hash<unsigned char>
{
    size_t operator()(unsigned char x) const {
        return x;
    }  
};

template<>
struct hash<signed char>
{
    size_t operator()(unsigned char x) const {
        return x;
    }  
};

template<>
struct hash<short>
{
    size_t operator()(short x) const {
        return x;
    }  
};

template<>
struct hash<int>
{
    size_t operator()(int x) const {
        return x;
    }  
};

template<>
struct hash<long>
{
    size_t operator()(long x) const {
        return x;
    }  
};

template<>
struct hash<long long int>
{
    size_t operator()(long long int x) const {
        return x;
    }  
};

template<>
struct hash<const short>
{
    size_t operator()(short x) const {
        return x;
    }  
};

template<>
struct hash<const int>
{
    size_t operator()(int x) const {
        return x;
    }  
};

template<>
struct hash<const long>
{
    size_t operator()(long x) const {
        return x;
    }  
};

template<>
struct hash<const long long int>
{
    size_t operator()(long long int x) const {
        return x;
    }  
};
_MY_NAMESPACE_END 

#endif
