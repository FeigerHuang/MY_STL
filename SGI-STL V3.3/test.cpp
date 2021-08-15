/*************************************************************************
	> File Name: test.cpp
	> Author: huguang
	> Mail: hug@haizeix.com
	> Created Time: 
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <numeric>
#include <iterator>
#include <set>
using namespace std;

template<typename T>
struct display{
    void operator()(const T& x){
        cout << x << " ";
        return ;
    }
};


int main() {

    int ia1[] = {1, 3,  5, 7, 9,11};
    int ia2[] = {1, 1, 2, 3, 5, 8, 13};

    multiset<int> S1(ia1, ia1 + 6);
    multiset<int> S2(ia2, ia2 + 7);
    
    for_each(S1.begin(), S1.end(), display<int>());
    cout << endl;
    for_each(S2.begin(), S2.end(), display<int>());
    cout << endl;

    set_union(S1.begin(), S1.end(), S2.begin(), S2.end(), ostream_iterator<int>(cout, "|"));
    cout << endl << "Intersection of S :";
    set_intersection(S1.begin(), S1.end(), S2.begin(), S2.end(), ostream_iterator<int>(cout, "|"));
    cout << endl << "difference of S:";
    set_difference(S1.begin(), S1.end(), S2.begin(), S2.end(), ostream_iterator<int>(cout, "|"));
    cout << endl << "symmetric difference of S:";
    set_symmetric_difference(S1.begin(), S1.end(), S2.begin(), S2.end(), ostream_iterator<int>(cout, "|"));


//  cout << accumulate(iv.begin(), iv.end(), 0) << endl;
//  cout << accumulate(iv.begin(), iv.end(), 0, minus<int>()) << endl;
//  cout << inner_product(iv.begin(), iv.end(),iv.begin(), 0) << endl;
    
    //ostream_iterator<int> oite(cout, " ");
    //partial_sum(iv.begin(), iv.end(), oite);
    //cout << endl;
    
//    adjacent_difference(iv.begin(), iv.end(), oite);
//    adjacent_difference(iv.begin(), iv.end(), oite, multiplies<int>());

    return 0;
}
