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
    multimap_test::main();

    return 0;
}
