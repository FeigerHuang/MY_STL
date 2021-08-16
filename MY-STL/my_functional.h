/*************************************************************************
	> File Name: my_functional.h
	> Author: Feiger
	> Mail: 1162006607@qq.com
	> Created Time: Mon 16 Aug 2021 08:18:32 PM CST
 ************************************************************************/

#ifndef _MY_FUNCTIONAL_H
#define _MY_FUNCTIONAL_H

#ifdef BUG_FUNP
#define LOG_FUN(msg) {std::cerr << msg << std::endl;}
#else
#define LOG_FUN(msg)
#endif

#define _MY_NAMESPACE_BEGIN     namespace feiger{
#define _MY_NAMESPACE_END       }

_MY_NAMESPACE_BEGIN

template <typename Ret_Type, typename ...ARGS>
class Func_base {
public:
    virtual Ret_Type  call(ARGS... args) = 0;
    virtual Func_base<Ret_Type, ARGS...>* getCopy() = 0;
    virtual ~Func_base() {};
};// 抽象基类实现接口定义;

template <typename Ret_Type, typename ...ARGS>
class Normal_Func : public Func_base<Ret_Type, ARGS...> {
public:
    typedef Normal_Func<Ret_Type, ARGS...>  self;

    Normal_Func(Ret_Type (*ptr)(ARGS...))
        : func_p(ptr) {  }

    Ret_Type call(ARGS... args) override {
        return this->func_p(std::forward<ARGS>(args)...);
    }
    
    self* getCopy() override {
        return new self(*this);
    }
    
    ~Normal_Func() {
        LOG_FUN("~Normal_Func() successed");
    }
private:
    Ret_Type (* func_p)(ARGS...);
}; // 实现普通函数调用的类;

template <typename CLASS_T,typename Ret_Type, typename ...ARGS>
class Class_Func : public Func_base<Ret_Type, ARGS...>{
public:
    typedef Class_Func<CLASS_T, Ret_Type, ARGS...> self;
    
    Class_Func(const CLASS_T& rhs)
        : func_obj(rhs){}

    Ret_Type call(ARGS... args) override {
        return func_obj(std::forward<ARGS>(args)...);
    }

    self* getCopy() override {
        return new self(*this);
    }
    ~Class_Func() {
        LOG_FUN("~Class_Func() successed");
    }
private:
    CLASS_T  func_obj;
}; // 实现可调用对象的类;


template <typename Ret_Type, typename ...ARGS>
class function;
// 真*函数指针类实现, 特化版本; 
template <typename Ret_Type, typename ...ARGS>
class function<Ret_Type(ARGS...)> 
{
public:
    typedef function<Ret_Type(ARGS...)> self;
    
    function() : ptr_base(nullptr) {}

    function(Ret_Type (* ptr)(ARGS...)) 
        : ptr_base(new Normal_Func<Ret_Type, ARGS...>(ptr))
    {    
        LOG_FUN("function pointer constructor");
    }
    
    template<typename CLASS_T>
    function(const CLASS_T& obj) 
        : ptr_base(new Class_Func<CLASS_T, Ret_Type, ARGS...>(obj))
    {
        LOG_FUN("function object constructor");
    }

    function(const function& rhs) 
    {
        LOG_FUN("call function copy  constructor");
        ptr_base = rhs.getCopy();
    }
    
    function& operator=(const function& rhs) {
        LOG_FUN("call assignment operator");
        if (this == &rhs) return *this;
        delete ptr_base;
        ptr_base = rhs.ptr_base->getCopy();
        return *this;
    }
    
    function& operator=(function&& rhs) noexcept 
    {
        LOG_FUN("call move assignment operator");
        std::swap(ptr_base, rhs.ptr_base);
        return *this;
    }

    function(function&& rhs) {
        LOG_FUN("call move copy constructor");
        std::swap(ptr_base, rhs.ptr_base);
        rhs.ptr_base = nullptr;
    }

    Ret_Type operator()(ARGS... args) {
        LOG_FUN("call operator()(args...)");
        return ptr_base->call(std::forward<ARGS>(args)...);
    }
    
    ~function() {
        if (ptr_base != nullptr) delete ptr_base;
    }
private:
    Func_base<Ret_Type, ARGS...>* ptr_base;
};

_MY_NAMESPACE_END

#undef LOG_FUN
#endif
