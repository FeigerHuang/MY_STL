#include "my_alloc.h"
#include <iostream>
#include "my_construct.h"
#include "my_slist.h"
#define BEGIN(x) namespace x {
#define END(x)   } // end of namespace x

#define BLU  "\033[34m"
#define YEL  "\033[33m"
#define RED  "\033[31m"
#define FIN   "\033[0m"


#define Test(func, args...) {\
    std::cout << RED << #func  << "("<< #args <<") = " << FIN \
    << func(args) << std::endl;\
}



using namespace feiger;

struct AP{
    AP() {
        std::cout << "AP() constructor done!\n";
    }
};

struct Node{
    Node() :x(-1), obj(){
        std::cout << this << ":default Node() constructor\n";
    }
    Node(int x) :x(x), obj(){
        std::cout << this << ":Node() constructor\n";
    }
    Node(const Node& rhs) :x(rhs.x), obj() {
        //std::cout << this << ":Node(&) copy constructor\n";
    }
    ~Node() {
        std::cout << this << ":~Node() destructor\n";
    }
    Node& operator=(const Node& rhs){
        std::cout << this << "assignment operator=\n";
        x = rhs.x;
    }
    AP  obj;
    int x;
};

std::ostream& operator<<(std::ostream& out, const Node& rhs) {
    return out << "Node.x = " << rhs.x;
}

BEGIN(allocator_test) 

int main(){
    
    int * t;
    simple_alloc<int> alc;
    t = alc.allocate(3);
    t[0] = 1;
    t[1] = 2;
    t[2] = 3;
    destroy(t, t+3);
    std::cout << "--------------------" << std::endl;
    simple_alloc<Node>  myalc; 
    Node *p = allocator<Node>().allocate(1);
    construct(p, 666);
    printf("%p\n", p);
    std::cout << p->x << std::endl;
    destroy(p);
    return 0;
}


END(allocator_test)

#include "my_slist.h"

BEGIN(slist_test)

int main() {

    //slist<int> l1;
    //auto t = l1.create_node(1);
    //std::cout << t->data << std::endl; 
    //auto t2 = slist<Node>::create_node(666);
    //std::cout << t2 << " = address : "<< t2->data.x << std::endl;
    //std::cout << "\n1.---------------\n";
    //slist<Node>::destroy_node(t2);
    slist<int> l3;
    l3.push_front(3);
    std::cout << l3.front() << std::endl;
    l3.push_front(5);
    l3.push_front(8);
    slist<int>::iterator it;
    for ( it = l3.begin(); it != l3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    l3.pop_front();
    std::cout << " after pop_front head = "<< l3.front() << std::endl;
    std::cout << "**************************************\n";
    it = l3.begin();
    it++;
    l3.insert_after(it, 65);
    l3.insert_after(it, 6);
    l3.insert_after(it, 987);
    for ( it = l3.begin(); it != l3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n***************************************\n";
    slist<int> l4(std::move(l3));
    for ( it = l4.begin(); it != l4.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "\n***************************************\n";
    
    if (l4.find(65) != l4.end()){
        std::cout << "l4 find 65" << std::endl;
    }
    std::cout << std::boolalpha  << bool(l4.begin()) << ":" << l4.end() << std::endl; 
    if (l4.find(15) == l4.end()){
        std::cout << "l4 can't find 15" << std::endl;
    }
    //l3.clear();
    return 0;
}


END(slist_test)

#include "my_algobase.h"
#include <list>
#include <vector>

BEGIN(algo_test)

int main() {
    const char ccs[5] = {'a', 'b', 'c', 'd', 'e'} ;
    char ccd[5];
    feiger::copy(ccs, ccs + 5, ccd);
    for (int i = 0; i < 5; ++i) std::cout << ccd[i] << " ";
    std::cout << std::endl;
    
    slist<int> l1;
    l1.push_front(3);
    l1.push_front(13);
    l1.push_front(23);
    std::list<int> l2(3);
    feiger::copy(l1.begin(), l1.end(), l2.begin());
    for (auto it : l1) std::cout << it << " ";
    std::cout << std::endl;
    for (auto it : l2) std::cout << it << " ";
    std::cout << std::endl;
    
    std::vector<int> iv(15);
    feiger::uninitialized_fill_n(iv.begin(), 10, 789);
    for (auto x : iv) std::cout << x << " ";
    std::cout << std::endl;

    //slist<Node> iv1;
    //iv1.push_front(1);
    //iv1.push_front(3);
    //iv1.push_front(5);
    //iv1.push_front(3);
    //iv1.push_front(9);
    //slist<Node> iv2;
    //iv2.push_front(3);
    //iv2.push_front(13);
    //iv2.push_front(13);
    //iv2.push_front(13);
    //iv2.push_front(13);
    //feiger::copy(iv1.begin(), iv1.end(), iv2.begin());
    //for (auto it : iv2) std::cout << it << " "; 
    //std::cout << std::endl;

    return 0;
}

END(algo_test)


#include <deque>

BEGIN(algo_test2)

int main() {
    int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    feiger::copy_backward(ia + 2, ia + 7, ia + 9);
    for (int x : ia) std::cout << x << " ";
    std::cout << std::endl;
    
    int ia2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    feiger::copy_backward(ia2 + 2, ia2 + 7, ia2 + 5);
    for (int x : ia2) std::cout << x << " ";
    std::cout << std::endl;
    
    Node id[] = {0 , 1, 2, 3, 4, 5, 6, 7,8};
    auto first = id + 2;
    auto last = id + 7;
    std::cout << "before copy_backward :\n";
    for(auto x : id) std::cout << x << " ";

    std::cout << "["<< *first << ", " << *last << ")" << std::endl;
    auto result = id + 9;
    feiger::copy_backward(first, last, result);
    for(auto x : id) std::cout << x << " ";
    std::cout << std::endl;
}


END(algo_test2)


#include "my_vector.h"

BEGIN(vec_test)


int main() {
    //vector<int> vc1(5, 7);
    //for (auto it = vc1.begin(); it != vc1.end(); ++it) {
    //    std::cout << *it << " ";
    //}
    //std::cout << std::endl;
    //vector<int> vc2(6);
    //for (auto it = vc2.begin(); it != vc2.end(); ++it) {
    //    std::cout << *it << " ";
    //}
    //std::cout << std::endl << "vec2 size() = " << vc2.size();
    //std::cout << " |vec2 empty() =" << std::boolalpha << vc2.empty() << std::endl;
    //for (int i = 0; i < vc2.size(); ++i) {
    //    vc2[i] = i + 1;
    //}
    //std::cout << "after modify vec2 :";
    //for (auto it : vc2) std::cout << it << " ";
    //std::cout << "vec2.front() = " << vc2.front() << std::endl;
    //std::cout << "vec2.back() = " << vc2.back() << std::endl;
    //std::cout << std::endl;
    //vector<Node> vc3(5, Node());
    //vc3.pop_back();
    //std::cout << "\nafter vc3 pop_back() :"; 
    //for (auto it = vc3.begin(); it != vc3.end(); ++it) {
    //    std::cout << *it << " ";
    //}
    //std::cout << std::endl;
    //vc3.push_back(1);
    //auto iter = vc2.begin() + 1;
    //vc2.erase(iter);
    //std::cout << "\nafter erase ve2 position = 1 : ";
    //for (auto it = vc2.begin(); it != vc2.end(); ++it) {
    //    std::cout << *it << " ";
    //}
    //std::cout << std::endl;
    //std::cout << "--------------------------\n";
    //feiger::vector<int>  v4(1, 2);
    //for (int i = 0; i < 6; ++i) {
    //    v4.push_back(i);
    //} 
    //for (auto x : v4) std::cout << x << " ";
    //std::cout << std::endl;
    //auto first = v4.begin();
    //auto last = v4.begin() + 2;
    //v4.erase(first, last); 
    //std::cout << "erase v4 at 0 to 2 :";
    //for (auto x : v4) std::cout << x << " ";
    //std::cout << std::endl <<  "v4 capacity ="<<v4.capacity() << std::endl;
    //v4.insert(v4.begin() + 2, 8, 6);
    //
    //std::cout << "after v4 insert 8 cnt of 6 in 2 capacity = "<<v4.capacity() << std::endl;
    //for (auto x : v4) std::cout << x << " ";
    //std::cout << std::endl;

    //std::cout << v4.size() << " capacity = " << v4.capacity() << std::endl;
    //
    //for (int i = 0; i < 6; ++i) {
    //    v4.push_back(1);
    //    std::cout << "now v4.capacity = " << v4.capacity() << std::endl;
    //}

    //v4.clear();
    //std::cout << "after v4 clear() : v4.size() = ";
    //std::cout << v4.size() << " capacity = " << v4.capacity() << std::endl;

    //v4.resize(28);
    //std::cout << "after v4.reserve(28) :";
    //std::cout << v4.size() << " capacity = " << v4.capacity() << std::endl;
    //for (auto x : v4) std::cout << x << " ";
    //std::cout << std::endl;

    const vector<int> v5(12, 3);
    //v5[3] = 6;
    //std::cout << "v5[3] = "<< v5[3] << std::endl;
    //for (auto x : v5) std::cout << x << " ";
    

    feiger::vector<int>  v6(1, 2);
    for (int i = 1; i < 6; ++i) {
        v6.push_back(i);
    } 
    for (auto x : v6) std::cout << x << " ";
    std::cout << std::endl;
    
    vector<int> v7(std::move(v6));
    for (auto x : v7) std::cout << x << " ";
    std::cout << std::endl;
    
    v7 = std::move(v5);
    for (auto x : v7) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}

END(vec_test)

#include "my_function.h"

BEGIN(func_test)

int main() {
    
    using std::cout;
    using std::endl;

    plus<int> plusobj;
    minus<int>  minusobj;
    multiplies<double>  mulobj;
    divides<double>  divobj;
    modulus<int> modobj;
    negate<int>  negobj;
    
    //Test(plusobj, 1, 2);
    //Test(minusobj, 123, 23);
    //Test(mulobj, 34, 2.5);
    //Test(divobj, 6.9, 0.3);
    //Test(modobj, 34234123, 2);
    //Test(negobj,   -2342);
    
    equal_to<int> eqto_obj;
    not_equal_to<int> not_eq_obj;
    greater<int>  greater_obj;
    greater_equal<int> great_equal_obj;
    less<int> less_obj;
    less_euqal<int> less_euqal_obj;
    
    //Test(eqto_obj, 3, 5);
    //Test(not_eq_obj, 1, 1.5);
    //Test(great_equal_obj, 3, 2);
    //Test(greater_obj, 3,  3);
    //Test(less_euqal_obj, 2, 2);
    //Test(less_obj, 4, 3);
    
    logic_and<int> and_obj;
    logic_or<int>  or_obj;
    logic_not<int> not_obj;
    
    Test(and_obj, 0, 1);
    Test(or_obj, 1, 0);
    Test(and_obj, 1, 2);
    Test(not_obj, 0);
    Test(not_obj, 7);


    return 0;
}

END(func_test)
#include "my_functional.h"
#include <algorithm>

BEGIN(funp_test)

using std::cout;
using std::endl;

int add(int a, int b) {
    cout << "call add(a, b)" << endl;
    return a + b;
}

struct CF {
    int operator()(int a, int b) {
        cout << "call obj.operator()" << endl;
        return a * b;
    }  
};

int main() {
    
    function<int(int, int)> mfu;
    mfu = add;
    Test(mfu, 1, 5);
    
    CF obj;
    function<int(int, int)> fp(obj);     
    Test(fp, 5, 7);

    fp = mfu;
    cout << "after swap mfu, fp" << endl; 
    Test(fp, 5, 7);
    
    function<int(int, int)>  fp2(std::move(fp));
    Test(fp2, 5, 7);
    
    fp2 = mfu;
    Test(fp2, 6, 7);

    mfu = std::move(fp2);
    Test(mfu, 6, 7);

    return 0;
}

END(funp_test)


#include "my_algorithm.h"
BEGIN(algori_test)

using std::cin;
using std::cout;
using std::endl;

struct Node{
    int x, y;
};

std::ostream& operator<<(std::ostream& out, const Node& b) {
    return out << "( " << b.x << "," << b.y << " )";
}

struct CMP{
    bool operator()(const Node& a, const Node& b) const {
        return a.x * a.y < b.x * b.y;
    }
};


int main() {
    srand(time(NULL));
    vector<int> v1(150, 0);
    int val;
    for (int i = 0; i < 150; ++i) {
        val = rand() % 1000;
        v1[i] = val;
    }
    for (auto x : v1) cout << x << " ";
    cout << endl;
    
    sort(v1.begin(), v1.end());
    cout << "after sort :" << endl;
    for (auto x : v1) cout << x << " ";
    cout << endl;
    
    //int x;
    //while (cin >> x) {
    //    auto it = upper_bound(v1.begin(), v1.end(), x);
    //    cout << x << " : upper_bound in v1 = " << *it << endl;
    //}
    //while (cin >> x) {
    //    auto it = lower_bound(v1.begin(), v1.end(), x);
    //    cout << x << " : lower_bound in v1 = " << *it << endl;
    //}

   // insertion_sort(v1.begin(), v1.end());
   // cout << "after insertion_sort :" << endl;
   // for (auto x : v1) cout << x << " ";
   // cout << endl;
    
   // quick_sort(v1.begin(), v1.end());
   // cout << "after quick_sort :" << endl;
   // for (auto x : v1) cout << x << " ";
   // cout << endl;
    
   // sort(v1.begin(), v1.end());
   // cout << "after my_sort :" << endl;
   // for (auto x : v1) cout << x << " ";
   // cout << endl;
    vector<Node> vec(15);
    int vb;
    for (int i = 0; i < 15; ++i) {
        val = rand() % 100;
        vb = rand() % 10;
        vec[i] = {val, vb};
    }
    for (auto x : vec) cout << x << " ";
    cout << endl;
    sort(vec.begin(), vec.end(), CMP());
    cout << "after my_sort :" << endl;
    for (auto x : vec) cout << x << " ";
    cout << endl;

    int x, y;
    while (cin >> x >> y) {
        auto it = binary_search(vec.begin(), vec.end(), Node{x, y}, CMP());
        cout << x << " : upper_bound in vec = " << std::boolalpha << it << endl;
    }
    
    return 0;
}

END(algori_test)

#include "my_unordered_set.h"

BEGIN(hash_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    unordered_set<int> mst1;
    
   // cout << mst1.rep.bucket_count() << endl;
   // mst1.rep.resize(200);
   // cout << mst1.rep.bucket_count() << endl;
   // cout << "elems_count in set = " << mst1.rep.elems_count() << endl;
    
    //for(int i = 0; i < 15; ++i) {
    //    mst1.rep.insert_equal(i);
    //    mst1.rep.insert_equal(i);
    //    cout << "elems_count in set = " << mst1.rep.elems_count() << endl;
    //}
    
    //for(int i = 0; i < 18; ++i) {
    //    auto x = mst1.rep.insert_unique(i);
    //    auto y = mst1.rep.insert_unique(i);
    //    cout << "elems_count in set = " << mst1.rep.elems_count() << endl;
    //    cout << "x.frsit->val =" << *x.first  << "second =" << std::boolalpha << x.second << endl;
    //    cout << "y.frsit->val =" << *y.first  << "second =" << std::boolalpha << y.second << endl;
    //}
    //
    //int key;
    //while (cin >> key) {
    //    auto it = mst1.rep.find(key);
    //    auto cnt = mst1.rep.count(key);
    //    cout << "find key int hashtable iterator->val = " << *it << endl;
    //    cout << "key int hashtable count = " << cnt << endl;
    //    mst1.rep.insert_equal(key);
    //}
    
    unordered_set<long long>  mst2;
    cout << std::boolalpha << "mst2.empty() = "<<mst2.empty() << endl;
    long long buf;
    
    for (int i = 0; i < 6; ++i) {
        cin >> buf;
        auto it = mst2.insert(buf);
        cout << "insert " << buf << " to mst2 = " << it.second << endl;
    }
    
    for (int i = 0; i < 1; ++i) {
        cin >> buf;
        auto it = mst2.find(buf);
        if (it == mst2.end()) {
            cout << buf << " no find!" << endl;
        } else{
            cout << "find " << buf << " in mst2" << endl;
        }
        cout << "mst2.count() = " << mst2.count(buf) << endl;
    }

    cout << "range in hashtable :";
    for(auto it = mst2.begin(); it != mst2.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
   // unordered_set<long long> mst3;
   // mst2.swap(mst3);
   // cout << "range in hashtable mst3:";
   // for(auto it = mst3.begin(); it != mst3.end(); ++it) {
   //     cout << *it << " ";
   // }
   // cout << endl;

   // for (int i = 0; i < 3; ++i) {
   //     cin >> buf;
   //     mst3.erase(buf);
   //     cout << "after erase element from mst3 :";
   //     for(auto it = mst3.begin(); it != mst3.end(); ++it) {
   //         cout << *it << " ";
   //     }
   //     cout << endl;
   // }
   // 
   // for (int i = 0; i < 3; ++i) {
   //     auto first = mst3.begin();
   //     mst3.erase(first);
   //     cout << "after erase first element from mst3 :";
   //     for(auto it = mst3.begin(); it != mst3.end(); ++it) {
   //         cout << *it << " ";
   //     }
   //     cout << endl;
   // }
   // 
   // mst2.rep.clear();
   // mst2.rep.copy_from(mst3.rep);
   // cout << "range in hashtable mst2:";
   // for(auto it = mst2.begin(); it != mst2.end(); ++it) {
   //     cout << *it << " ";
   // }
   // cout << endl;

    return 0;
}


END(hash_test)


BEGIN(set_test)

using std::cin;
using std::cout;
using std::endl;

int main() {

    unordered_mulitset<int> st1;
    
    int val;
    cout << "please insert 8 elements to set : \n";
    for (int i = 0; i < 8; ++i) {
        cin >> val;
        st1.insert(val);
    }
    cout << "range in hashtable st1:";
    for(auto it = st1.begin(); it != st1.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    cout << "please cin 3 elems that you want to find : \n";
    for (int i =  0; i < 3; ++i) {
        cin >> val;
        if (st1.find(val) != st1.end()) {
            cout << "element in set and the amount = ";
            cout << st1.count(val) <<  " .\n";
        } else {
            cout << "can not find\n";
        }
    }
    cout << "please cin 5 elements that you want to erase : \n";
    for (int i = 0; i < 5; ++i) {
        cin >> val;
        st1.erase(val);
        cout << "now the size() of the set = " << st1.size() << endl;
        cout << "now the bucket_count = " << st1.bucket_count() << endl;
    }

    return 0;
}

END(set_test)

#include "my_unordered_map.h"

BEGIN(map_test)

using std::cin;
using std::cout;
using std::endl;

int main() {

    unordered_map<int, char> mp1;
    cout << "mp1.size() = " << mp1.size() << endl;
    cout << "mp1.Max_size() = " << mp1.max_size() << endl;
    cout << "mp1.empty() = " << std::boolalpha<< mp1.empty() << endl;
    
    for (int i =  0; i < 9; ++i) {
        mp1.insert(make_pair<int, char>(i, 'a' + i));
    }
    auto result = mp1.insert(make_pair<int, char>(0, 'x'));
    cout << result.second << endl;
    cout << "key:"<< result.first->first << "=" << result.first->second << endl;
    for(auto it = mp1.begin(); it != mp1.end(); ++it){
        cout << "pair(" << it->first << "," << it->second << ")" << endl;
    }
    int p;
    for (int i = 0; i < 3; ++i) {
        cout << "please input what you want to find : ";
        cin >> p;
        if (mp1.find(p) != mp1.end()) {
            cout << "element in map1 ," ;
            cout << "elems_count = " << mp1.count(p) << endl;
        } else{
            cout << "not find element\n";
        }
    }
    for (int i = 0; i < 3; ++i) {
        cout << "please input what you want to erase : ";
        cin >> p;
        mp1.erase(p);
        for(auto it = mp1.begin(); it != mp1.end(); ++it){
            cout << "pair(" << it->first << "," << it->second << ")" << endl;
        }
    }
    cout << mp1.size() << endl;
    cout << mp1[100] << endl;
    cout << mp1.size() << endl;
    mp1[0] = 'x';
    mp1[1] = 'y';
    for(auto it = mp1.begin(); it != mp1.end(); ++it){
        cout << "pair(" << it->first << "," << it->second << ")" << endl;
    }

    return 0;
}

END(map_test)

BEGIN(multimap_test)

using std::cin;
using std::cout;
using std::endl;

int main() {

    unordered_mulitmap<int, char> mp1;
    cout << "mp1.size() = " << mp1.size() << endl;
    cout << "mp1.Max_size() = " << mp1.max_size() << endl;
    cout << "mp1.empty() = " << std::boolalpha<< mp1.empty() << endl;
    
    for (int i =  0; i < 9; ++i) {
        mp1.insert(make_pair<int, char>(i, 'a' + i));
    }
    auto result = mp1.insert(make_pair<int, char>(0, 'x'));
    cout << result->first << endl;
    for(auto it = mp1.begin(); it != mp1.end(); ++it){
        cout << "pair(" << it->first << "," << it->second << ")" << endl;
    }
    pair<int, char> pii;
    for (int i = 0; i < 3; ++i) {
        cout << "please input what you want to insert  pair<int,char> :";
        cin >> pii.first >> pii.second;
        mp1.insert(pii);
    }
    int p;
    for (int i = 0; i < 3; ++i) {
        cout << "please input what you want to find : ";
        cin >> p;
        if (mp1.find(p) != mp1.end()) {
            cout << "element in map1 ," ;
            cout << "elems_count = " << mp1.count(p) << endl;
        } else{
            cout << "not find element\n";
        }
    }
    for (int i = 0; i < 3; ++i) {
        cout << "please input what you want to erase : ";
        cin >> p;
        mp1.erase(p);
        for(auto it = mp1.begin(); it != mp1.end(); ++it){
            cout << "pair(" << it->first << "," << it->second << ")" << endl;
        }
    }
    
    for(auto it = mp1.begin(); it != mp1.end(); ++it){
        cout << "pair(" << it->first << "," << it->second << ")" << endl;
    }
    
    cout << "elems_in_bucket[0] = " << mp1.elems_in_bucket(0) << endl;

    for(int i = 0; i < 3; ++i) {
        cout << "input elements find equal_range : ";
        cin >> p;
        auto pii = mp1.equal_range(p);
        if (pii.first != mp1.end())
            cout << "key p equal_range = [ (" << pii.first->first <<  ", " << pii.first->second <<") , (";
        if (pii.second != mp1.end())
            cout << pii.second->first << ", " << pii.second->second <<") )\n";
        else
            cout << "end() ) )\n";
    }
    
    
    return 0;

}

END(multimap_test)

#include "my_numeric.h"

BEGIN(numeric_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    vector<int>  vec;
    for (int i = 1; i <= 9; ++ i) {
        vec.push_back(i);
    }
    
    for (auto x : vec) cout << x  << " ";
    cout << endl;

    int result = 0;
    result = accumulate(vec.begin(), vec.end(), 0);
    cout << "accumulate(vec, +) = " << result << endl;
    
    result = accumulate(vec.begin(), vec.begin() + 5, 1, multiplies<int>());
    cout << "accumulate( 0, 5, *) = " << result << endl;
    
    vector<int>  ans(15, 0);
    adjacent_different(vec.begin(), vec.end(), ans.begin());
    cout << "adjacent_different(vec, -) = ";
    for (auto x : ans) cout << x  << " ";
    cout << endl;
    
    adjacent_different(vec.begin(), vec.end(), ans.begin(), plus<int>());
    cout << "adjacent_different(vec, +) = ";
    for (auto x : ans) cout << x  << " ";
    cout << endl;
    
    result = inner_product(vec.begin(), vec.begin() + 5, vec.begin(), 0);
    cout << "inner_product(vec[1,5], *) = " << result << endl;
    
    result = inner_product(vec.begin(), vec.begin() + 5, ans.begin(), 0, plus<int>(), plus<int>());
    cout << "inner_product(vec[1, 5], ans[1, 5], +, +) = " << result << endl;
    
    partial_sum(vec.begin(), vec.end(), ans.begin());
    cout << "partial_sum(vec, +) = ";
    for (auto x : ans) cout << x  << " ";
    cout << endl;

    partial_sum(vec.begin(), vec.end(), ans.begin(), minus<int>());
    cout << "partial_sum(vec, -) = ";
    for (auto x : ans) cout << x  << " ";
    cout << endl;

    return 0;
}

END(numeric_test)

#include "my_heap.h"
BEGIN(heap_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    vector<int>  vc;
    
    auto output = [](vector<int>& vc){
        for (auto i : vc) cout << i << " ";
        cout << endl;
    };

    int t = 0;
   //for (int i = 0; i < 15; ++i) {
   //    cin >> t;
   //    vc.push_back(t);
   //    push_heap(vc.begin(), vc.end());
   //    output(vc);
   //}
   //sort_heap(vc.begin(), vc.end());
   //output(vc);
   //for (int i = 0; i < 15; ++i) {
   //    int t =vc[0];
   //    cout << "heap top = " << t << ", after heap pop :\n";
   //    pop_heap(vc.begin(), vc.end());
   //    vc.pop_back();
   //    output(vc);
   //}
    
   // vector<int> vc2;
   // for (int i = 0; i < 10; ++i) {
   //     cin >> t;
   //     vc2.push_back(t);
   // }
   // output(vc2);
   // cout << "after map heap :\n";
   // make_heap(vc2.begin(), vc2.end());
   // output(vc2);
    for (int i = 0; i < 10; ++i) {
       cin >> t;
       vc.push_back(t);
       //push_heap(vc.begin(), vc.end(), [](int a, int b){return a > b;});
       //output(vc);
    }
   // for (int i = 0; i < 10; ++i) {
   //    int t =vc[0];
   //    cout << "heap top = " << t << ", after heap pop :\n";
   //    pop_heap(vc.begin(), vc.end(), [](int a, int b){return a > b;});
   //    vc.pop_back();
   //    output(vc);
   // }
    auto cmp = [](int a, int b) {return a > b;};
    output(vc);
    make_heap(vc.begin(), vc.end(), cmp);
    cout << "after make_heap :\n";
    output(vc);
    sort_heap(vc.begin(), vc.end(), cmp);
    cout << "after sort_heap :\n";
    output(vc);

    return 0;
}

END(heap_test)

#include "my_queue.h"

BEGIN(prique_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    
    priority_queue<int> mqu;

    cout << "mqu.szie() = " << mqu.size() << ", mqu.empty() = " << std::boolalpha << mqu.empty() << endl;
    int val;
    for (int i = 0; i < 10; ++i) {
        cin >> val;
        mqu.push(val);
    }
    
    while (!mqu.empty()) {
        cout << "mqu.top() = "<< mqu.top() << endl;
        mqu.pop();
    } 

    return 0;
}

END(prique_test)

BEGIN(numeric_test2)

using std::cin;
using std::cout;
using std::endl;

int main() {
    
    vector<int> vec(6, 0);
    
    auto output = [](vector<int>& vc){
        cout << "vec[] = ";
        for (auto i : vc) cout << i << " ";
        cout << endl;
    };
    
    for (int i = 0; i < 6; ++i) {
        cin >> vec[i];
    }
    output(vec);
    
    cout << "adjacent_find = "<< *adjacent_find(vec.begin(), vec.end()) << endl;
    cout << "adjacent_find first less = " << *adjacent_find(vec.begin(), vec.end(), less<int>()) << endl;

    cout << "count(3) = " << count(vec.begin(), vec.end(), 3) << endl;
    cout << "count_if( >3) = " << count_if(vec.begin(), vec.end(), [](int a){return a > 3;}) << endl;
    
    cout << "find(5) = " << *find(vec.begin(), vec.end(), 5) << endl;
    cout << "find_if(>5) = " << *find_if(vec.begin(), vec.end(), [](int a){return a > 5;} ) << endl;

    return 0;
}

END(numeric_test2)

#include "my_list.h"

BEGIN(list_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    
    list<Node> lt;
    auto output = [](list<Node>& t) {
        cout << "list = ";
        for (auto it = t.begin(); it != t.end(); ++it) {
            cout << *it << "->";
        }
        cout << endl;
    };
    //auto p = lt.create_node(666);
    //cout << "p->data = " << p->data << endl;
    //lt.destroy_node(p);
    cout << "lt.empty() = " << std::boolalpha << lt.empty() << endl;
    lt.erase(lt.begin());
    for (int i = 0; i < 6; ++i) {
        lt.insert(lt.begin(), i + 1);
        output(lt);
    }
    cout << "lt.empty() = " << std::boolalpha << lt.empty() << endl;
    for (int i = 0; i < 6; ++i) {
        lt.push_back(i + 1);
        output(lt);
    }
     
    for (int i = 0; i < 6; ++i) {
        lt.erase(lt.begin());
        output(lt);
    }
    
    //for (int i = 0; i < 6; ++i) {
    //    lt.pop_back();
    //    output(lt);
    //}
    //auto func = [](list<Node>::iterator x) {cout << "x=" << *x << endl;};
    //func(--lt.end());
    //lt.clear();
    //cout << "lt.empty() = " << std::boolalpha << lt.empty() << endl;
    
    list<int>  lst;
    for (int i = 0; i < 6; ++i) {
        lst.push_front(i + 1);
        lst.push_front(i + 1);
    }
    
    auto output2 = [](list<int>& lt) {
        for (auto it = lt.begin(); it != lt.end(); ++it) {
            cout << *it << "->";
        }
        cout << endl;
    };
    output2(lst);
    lst.remove(3);
    cout << "after remove 3 :"; 
    output2(lst);
    cout << "list.size() = " << lst.size() << endl;

    lst.unique();
    cout << "after unique :"; 
    output2(lst);
    cout << "list.size() = " << lst.size() << endl;
    


    return 0;
}

END(list_test)

#include "my_deque.h"

BEGIN(deque_test)

using std::cin;
using std::cout;
using std::endl;

int main() {

    deque<Node> que(6, Node());
    for (auto it = que.begin(); it != que.end(); ++it) {
        cout << *it << " ,";
    }
    cout << endl;
    
    auto output = [](deque<Node>& q) {
        for (auto it = q.begin(); it != q.end(); ++it) 
            cout << *it << " ,";
        cout << endl;
    };

    for (int i = 0;  i < 6; ++i) {
        que.push_back(i);
    }
    output(que);
    
    for (int i = 6; i < 12; ++i) {
        que.push_front(i);
    }
    output(que);
    cout << RED <<  "que.front() = " << que.front() << ", que.back() = " << que.back() << FIN <<endl;  
   // while (!que.empty()) {
   //     //que.pop_back();
   //     que.pop_front();
   //     output(que);
   //     cout << BLU << "que.size() = " << que.size() << FIN << endl; 
   // }
    
    que.clear();
    output(que);
    
    deque<int> que2(1, 0);
    auto output2 = [](deque<int>& q) {
        for (auto it = q.begin(); it != q.end(); ++it) 
            cout << *it << " ,";
    };
    cout << BLU <<  "size = " << que2.size() << FIN << endl; 
    
    //output2(que2);

    //for (int i = 0 ; i < 600; ++i) {
    //    que2.push_back(i + 1);
    //    //que2.push_front(i + 1);
    //    //output2(que2);
    //    cout << BLU <<  "size = " << que2.size() << FIN << ","; 
    //}
    //output2(que2);
    
    //cout << "deque<int>buffer_size = " << deque_iterator<int,int&, int *>::buffer_size() << endl;
    //cout << que2.map_size << endl;
    //cout << "start last = "<< que2.start.M_node - que2.map << " , back last = " << que2.map_size - (que2.finish.M_node - que2.map);
    // deque
    //for (int i = 0 ; i < 6000; ++i) {
    //    que2.push_back(i + 1);
    //    que2.push_front(i + 1);
    //    //output2(que2);
    //    cout << BLU <<  "size = " << que2.size() << FIN << endl;
    //}
    //cout << "deque<int>buffer_size = " << deque_iterator<int,int&, int *>::buffer_size() << endl;
    //cout << que2.map_size << endl;
    //cout << RED << que2.size() << " = que2.size() " << FIN << endl;
    //output2(que2);
    //for (int i = 0; i < que2.size(); ++i) {
    //    cout << que2[i] << ",";
    //}
    //cout << endl;
    //cout << "start last = "<< que2.start.M_node - que2.map << " , back last = " << que2.map_size - (que2.finish.M_node - que2.map);
    
    deque<int> que3(1, 6);
    for (int i = 0; i < 6; ++i) {
        que3.push_front(i + 1);
    }
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 
    
    que3.erase(que3.begin() + 1 );
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 
    
    que3.erase(que3.end() - 2);
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 
    
    que3.erase(que3.begin() + 2, que3.end());
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 

    que3.insert(que3.begin() + 2, 888);
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 
    
    que3.insert(que3.begin() + 2, 777);
    output2(que3);
    cout << "que3.size() = " << que3.size() << endl; 
    
    for (int i = 0; i < 5; ++i) {
        que3.insert(que3.begin() + 2, (i + 1) * 11);
        output2(que3);
        cout << "que3.size() = " << que3.size() << endl; 
    }
    for (int i = 5; i < 10; ++i) {
        que3.insert(que3.end() - 3, (i + 1) * 11);
        output2(que3);
        cout << "que3.size() = " << que3.size() << endl; 
    }
    
    return 0;
}

END(deque_test)

#include "my_stack.h"

BEGIN(que_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    queue<Node>  que;
    //cout << que.front() << endl;
    cout << "que.size() = " << que.size() << " , que.empty()=" << std::boolalpha << que.empty() << endl;
    for (int i = 0; i < 20; ++i) {
        que.push(i + 1);
        cout << "que.size() = " << que.size() << " , que.empty()=" << que.empty() << endl;
    }
    
    while (!que.empty()) {
        cout << RED << "que.front() = " << que.front() << FIN << endl;    
        que.pop();
    }
    
    cout << "--------------------------------\n";
    stack<Node> sta;
    cout << "sta.size() = " << sta.size() << " , sta.empty()=" << std::boolalpha << sta.empty() << endl;
    for (int i = 0; i < 20; ++i) {
        sta.push(i + 1);
        cout << "sta.size() = " << sta.size() << " , sta.empty()=" << std::boolalpha << sta.empty() << endl;
    }
    
    while (!sta.empty()) {
        cout << RED << "sta.top() = " << sta.top() << FIN << endl;    
        sta.pop();
    }
    
    return 0;
}

END(que_test)

BEGIN(alloc_test)


using std::cin;
using std::cout;
using std::endl;

int main() {
    super_allocator<int> myalloc;
    int *arr = (int *)myalloc.allocate(sizeof(int) * 20);
    for (int i = 0; i < 20; ++i) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < 20; ++i) {
        cout << arr[i] << ", ";
    }
    
    arr = (int *)super_allocator<int>::reallocate(arr,sizeof(int)*20, sizeof(int) * 30);
    for (int i = 20; i < 30; ++i) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < 30; ++i) {
        cout << arr[i] << ", ";
    }
    
    super_allocator<int>::deallocate(arr, sizeof(int) * 30);
    
    Node *vec;
    vec = (Node *)super_allocator<Node>::allocate(sizeof(Node)*10);
    for (int i = 0; i < 10; ++i) {
        construct(vec + i, i + 1);
    }

    for (int i = 0; i < 10; ++i) {
        cout << BLU << vec[i] << ", " << FIN;
    }
    cout << endl;
    for (int i = 0; i < 10; ++i) {
        destroy(vec + i);
    }
    super_allocator<int>::deallocate(arr, sizeof(int) * 30);
    return 0;
}

END(alloc_test)

#include "my_rbtree.h"

BEGIN(rbtree_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    RB_Tree<int, int> tree;
    cout << "tree.empty() = " << std::boolalpha << tree.empty() << ", tree.size()" << tree.size() << endl;
    //cout << "NIL->val = "<< tree.NIL->value << " , NIL->color = " << tree.NIL->color << endl;
    //cout << "NIL->child->val = "<< tree.NIL->lchild->value << " , NIL->child->color = " << tree.NIL->lchild->color << endl;
 
    //RB_Tree_Alloc<Node> alloc;
    //auto p = alloc.get_node();
    //construct(p, 23);
    //cout << p->value << endl;
    //alloc.put_node(p);

    //auto p = tree.new_node(666);
    //cout << p->value << endl;
    //tree.delete_node(p);
    
    //RB_Tree<int, Node> tee;
    //auto p2 = tee.new_node(666);
    //cout << p2->value << endl;
    //tee.delete_node(p2);
    int val;
    for (int i = 0; i < 8; ++i) {
        cin >> val;
        //tree.insert_unique(val);
        tree.insert_equal(val);
        tree.range();
        cout << "tree.empty() = " << std::boolalpha << tree.empty() << ", tree.size()" << tree.size() << endl;
    }
    

    for (int i = 0; i < 5; ++i) {
        cin >> val;
        //cout << "tree.find() :" << val << " = " << tree.find(val) << endl;
        //cout << "tree.find_or_insert() :" << val << " = " << (tree.find_or_insert(val)->value)<< endl;
        tree.range();
    }


    tree.clear();
    cout << "tree.empty() = " << std::boolalpha << tree.empty() << ", tree.size()" << tree.size() << endl;

    while (cin >> val) {
        tree.erase_unique(val);
        tree.inorder();
    }


    return 0; 
}

END(rbtree_test)

#include "my_set.h"

BEGIN(mset_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    set<int> s1;
    
    for (int i = 1; i <= 28; ++i) {
        s1.insert(i);
        cout << "set.size()=" << s1.size() << endl;
        cout << "set = {";
        for (auto it = s1.begin(); it != s1.end(); ++it) {
            cout << *it << " ,";
        }
        cout << "}\n";
    }
    s1.range();
    //s1.clear();
    cout << "set.empty()="<< std::boolalpha << s1.empty() << endl;
    int x;
    for (int i = 1; i < 5; ++i) {
        cout << "input what you want to find : ";
        cin >> x;
        cout << "set.find (" << x << ")=" << s1.find(x)  << endl;
    }
 
    cout << "input what you want to erase : ";
    while (cin >> x) {
        s1.erase(x);
    
        s1.range();
        cout << endl;
        cout << "input what you want to erase : ";
    }
}

END(mset_test)

#include "my_map.h"

BEGIN(mmap_test)

using std::cin;
using std::cout;
using std::endl;

int main() {
    
    map<int, char> mp1;
    
    for (int i = 1; i <= 25; ++i) {
        mp1.insert(make_pair<int,char>(i, 'a' + i));
        cout << "map.size()=" << mp1.size() << endl;
    }
    mp1.range();
    cout << "map.empty()="<< std::boolalpha << mp1.empty() << endl;
    
    auto it = mp1.begin();
    //while (it != mp1.end() ) {
    //    cout << "key["<<it->first << "]=" << it->second <<  endl;  
    //    it.increment();
    //}
    
    while (it != mp1.end() ) {
        auto tmp = it++;
        cout << "key["<<tmp->first << "]=" << tmp->second <<  endl;  
        cout << "after iterator++ :";
        //it->first = 233;
        cout << "key["<<it->first << "]=" << it->second <<  endl;  
        cout << "-------------------------------------------------\n";
    }
    
    mp1.range();
    cout << endl << "scanf what you want to erase :";
    int key;
    char val;
    while (cin >> key) {
        //cout << "key[" << key << "]=" << mp1[key] << endl;
        mp1.erase(key);
        mp1.range();
        cout << "mp1.size()="<< mp1.size() << endl;
        cout << endl << "scanf what you want to erase :";
    }
    
    cout << endl << "scanf what you want to query :";
    while (cin >> key) {
        cout << "key[" << key << "]=" << mp1[key] << endl;
        cout << "value modify to : ";  
        cin >> val;
        mp1[key] = val;
        cout << "key[" << key << "]=" << mp1[key] << endl;
        mp1.range();
        cout << endl << "scanf what you want to query :";
    }

}

END(mmap_test)

int main() {
    //allocator_test::main();
    //slist_test::main();
    //algo_test::main();
    //algo_test2::main();
    //vec_test::main();
    //func_test::main();
    //funp_test::main();
    //algori_test::main();
    //hash_test::main();
    //set_test::main();
    //map_test::main();
    //multimap_test::main();
    //numeric_test::main();
    //heap_test::main();    
    //prique_test::main();
    //numeric_test2::main();
    //list_test::main();
    //deque_test::main();
    //que_test::main();
    //alloc_test::main();
    //rbtree_test::main();
    //mset_test::main();
    mmap_test::main();


    return 0;
}
