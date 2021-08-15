#include "my_alloc.h"
#include <iostream>
#include "my_construct.h"
#include "my_slist.h"
#define BEGIN(x) namespace x {
#define END(x)   } // end of namespace x
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
    vector<int> vc1(5, 7);
    for (auto it = vc1.begin(); it != vc1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    vector<int> vc2(6);
    for (auto it = vc2.begin(); it != vc2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl << "vec2 size() = " << vc2.size();
    std::cout << " |vec2 empty() =" << std::boolalpha << vc2.empty() << std::endl;
    for (int i = 0; i < vc2.size(); ++i) {
        vc2[i] = i + 1;
    }
    std::cout << "after modify vec2 :";
    for (auto it : vc2) std::cout << it << " ";
    std::cout << "vec2.front() = " << vc2.front() << std::endl;
    std::cout << "vec2.back() = " << vc2.back() << std::endl;
    std::cout << std::endl;
    vector<Node> vc3(5, Node());
    vc3.pop_back();
    std::cout << "\nafter vc3 pop_back() :"; 
    for (auto it = vc3.begin(); it != vc3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    vc3.push_back(1);
    auto iter = vc2.begin() + 1;
    vc2.erase(iter);
    std::cout << "\nafter erase ve2 position = 1 : ";
    for (auto it = vc2.begin(); it != vc2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "--------------------------\n";
    feiger::vector<int>  v4(1, 2);
    for (int i = 0; i < 6; ++i) {
        v4.push_back(i);
    } 
    for (auto x : v4) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}

END(vec_test)

int main() {
    //allocator_test::main();
    //slist_test::main();
    //algo_test::main();
    //algo_test2::main();
    vec_test::main();

    return 0;
}
