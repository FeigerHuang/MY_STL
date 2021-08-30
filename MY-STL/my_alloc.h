/*************************************************************************
	> File Name: my_alloc.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Wed 11 Aug 2021 09:23:16 PM CST
 ************************************************************************/

#ifndef _MY_ALLOC_H
#define _MY_ALLOC_H
#include <cstring>
#include <iostream>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#define BLU  "\033[34m"
#define YEL  "\033[33m"
#define RED  "\033[31m"
#define FIN   "\033[0m"
#ifdef BUG_ALL
#define LOG(msg) {std::cerr << msg << std::endl;}
#else
#define LOG(msg)
#endif

//一级配置器的实现 BEGIN
template <class T>
inline 
T* allocate(ptrdiff_t size, T*) {
    //set_new_handler(0);
    T *tmp = (T *)(::operator new((size_t)(size *sizeof(T))));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    //LOG("allocate done");
    return tmp;
}

template <class T>
inline 
void deallocate(T *buffer) {
    ::operator delete(buffer);
    //LOG("deallocate done");
}

//一级配置器的实现 BEGIN

template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    static pointer allocate(size_type n) {
        pointer result = (pointer)(malloc(n));
        if (0 == result) {
            std::cerr << "malloc out of memory" << std::endl;
        }
        LOG(YEL << "malloc address = " << result << ", size =" << n << FIN);
        return result;
    }

    static void deallocate(pointer p, size_t) {
        free(p);
    }

    static pointer address(reference x) {return (pointer)&x;}

    static const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    static size_type init_page_size() {
        return std::max(size_type(1), size_type(4096 / sizeof(T)) );
    }

    static size_type max_size() {
        return std::max(size_type(1), size_type(UINT_MAX / sizeof(T)) );
    }
};

template<>
class allocator<void> {
public:
    typedef void *pointer;
};

//一级配置器的实现 END 

//二级级配置器的实现 BEGIN

enum {__ALIGN = 8}; // 以8字节为单位对齐;
enum {__MAX_BYTES = 128}; // 链表分配内存上限; 
enum {__NFREELISTS = __MAX_BYTES / __ALIGN}; // free-lists的个数;

template <class T>
class super_allocator {
private:
    static size_t ROUND_UP(size_t bytes) {
        return ((bytes) + __ALIGN - 1) & ~(__ALIGN - 1); // 大于8的数 和1111111111000这个数&运算结尾为8字节的倍数
    }
private:
    union obj {
        union obj *next; // 链表指针;
        char client_datta[1];  // 数据存储区;
    };
private:
    static obj* volatile free_list[__NFREELISTS];

    static size_t FREELIST_INDEX(size_t bytes) { // 向8取整 在除 8 计算下标;
        return ((bytes) + __ALIGN - 1) / __ALIGN - 1 ; 
    }
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
private:
    // 给内存池中注水, 并返回n-Byte大小的空间
    static void *refill(size_t n);
    // 配置一个chunk, 可以容纳nobjs * size的区块; 
    static char *chunk_alloc(size_t size, int &nobjs);
    
    // chunk allocation state 
    static char *start_free;  // 记录内存池开始
    static char *end_free;    // 和结束位置
    static size_t  heap_size; // 向系统申请的堆空间大小;
public:
    // 申请空间
    static void *allocate(size_t n) {
        if (n > (size_t) __MAX_BYTES) {
            return malloc(n);
        } 
        else {
            obj* volatile* my_free_list = free_list + FREELIST_INDEX(n);
            obj* result = *my_free_list; // 定位到对应大小链表位;
            if (result == 0) {
                //当前链表槽并未挂载内存块;
                void *r = refill(ROUND_UP(n));
                return r;
            }
            // 链表下挂载内存块, 返回该内存块,并让槽指向下个内存块;
            *my_free_list = result->next;
            return result;
        }
    }

    // 释放空间;
    static void deallocate(void *p, size_t n) {
        if (n > (size_t)__MAX_BYTES) {
            free(p);
            return;
        }
        obj *q = (obj *)p; // 定位到对应大小链表位
        obj* volatile* my_free_list = free_list + FREELIST_INDEX(n);
        q->next = *my_free_list; // 归还节点next指向槽所挂内存块
        *my_free_list = q; // 跟新槽位;
    }
    // 重新配置的空间; 
    static void *reallocate(void *p, size_t old_sz, size_t new_sz) {
        void *result;
        size_t copy_size;
        // 看看是否调用二级配置器;
        if (old_sz > (size_t)__MAX_BYTES && new_sz > (size_t)__MAX_BYTES) {
            return realloc(p, new_sz);
        }
        if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
        // 重新配置的大小;
        result = allocate(new_sz);
        copy_size = new_sz > old_sz ? old_sz : new_sz;
        // 把原内存上的内容拷贝过来;
        memcpy(result, p, copy_size);
        // 释放原空间;
        deallocate(p, old_sz);
        return result;
    }
};

template <class T>
typename super_allocator<T>::obj* volatile
super_allocator<T>::free_list[__NFREELISTS] = {0, 0, 0, 0,
                                                0, 0, 0, 0,
                                                0, 0, 0, 0,
                                                0, 0, 0, 0,};

template <class T>
char *super_allocator<T>::start_free = 0;  // 记录内存池开始

template <class T>
char *super_allocator<T>::end_free = 0;    // 和结束位置

template <class T>
size_t  super_allocator<T>::heap_size = 0; // 向系统申请的堆空间大小;


// 重新填充free_list, 并返回大小为 n(向8对齐)的对象
template <class T>
void *super_allocator<T>::refill(size_t n) {
    int nobjs = 20;
    // 调用chunk_alloc, 看看chunk内存池里有没有油水;
    char *chunk = chunk_alloc(n, nobjs);
    obj* volatile* my_free_list;
    obj* result;
    obj* current_obj, *next_obj;
    int i;
    // 如果只获得一个区块, 返回这个区块, free_list 未新增;
    if (1 == nobjs) return chunk;
    // 否则将多分配的区块, 存起来;
    my_free_list = free_list + FREELIST_INDEX(n);
    result = (obj *)chunk;
    // 第一块区块返回, 后面的区块串起来;
    *my_free_list = next_obj = (obj *)(chunk + n);

    for (i = 1; ; ++i) {
        current_obj = next_obj; // 前一块;
        next_obj = (obj *)(next_obj + n); // 下一块;
        if (nobjs - 1 == i) {
            current_obj->next = 0;
            break;
        } else {
            current_obj->next = next_obj;
        }
    }
    return result;
}

// 配置chunk, size大小位8的倍数; objs传出参数;
template <class T>
char* super_allocator<T>::chunk_alloc(size_t size, int& nobjs) {
    char *result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free; // 计算内存池所剩余大小;

    if (bytes_left >= total_bytes) {
        // 内存池水量够, 弱水三千只取 total_bytes, 调整起始位;
        result = start_free;
        start_free += total_bytes;
        return (char *)result;
    }
    else if (bytes_left >= size) {
        // 只够分一个size, 给你我的内存, 几乎毫无保留, 够义气;
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return (char *)result;
    }
    else {
        // 内存池没有水了, 速去找操作系统借内存;
        if (bytes_left > 0) { // 把渣渣都榨干;
            obj *volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free)->next = *my_free_list;
            *my_free_list = (obj *)start_free; // 把一点点剩下的内存挂上去;
        }
        // 重新申请内存;
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        start_free = (char *)malloc(bytes_to_get);
        if (0 == start_free ) {
            // 操作系统老大哥说他也没内存给咱们了
            int i;
            obj* volatile* my_free_list, *p;
            // 搜索 free_list 看看自家链表有没有可用的内存块;
            for (i = size; i < __MAX_BYTES; i += __ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) { // 这小子手上还有多的内存块;
                    *my_free_list = p->next;
                    start_free = (char *)p; // 拿来吧, 你!
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs);
                }
            }
        } // 大哥给了多的内存, 记录一下;
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

//二级配置器的实现 END 


// 封装空间配置器的对外接口 BEGIN
// 以便日后实现二级配置器进行封装

template<class T, class Alloc = allocator<T> >
class simple_alloc{
public:
    static T *allocate(size_t n) {
        LOG("allocate size = " << n);
        return 0 == n ? 0 : (T*)(Alloc::allocate(n * sizeof(T)) );
    }
    
    static T *allocate(void){
        return (T*)Alloc::allocate(sizeof(T));
    }
    
    static void deallocate(T* p, size_t n) {
        if (0 == n) return ;
        LOG("deallocate adress = " << p << " ,size = " << n);
        Alloc::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T* p) {
        Alloc::deallocate(p, sizeof(T));
    }
};
// 封装空间配置器的对外接口 END

#undef LOG

#endif
