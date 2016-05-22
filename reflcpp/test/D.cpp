#include "D.hpp"
#include <iostream>

using namespace std;

int
D::m0() {
	return 5;
}

void
D::m0_void() {
	cout << "D::m0_void() invoked." << endl;
}

int 
D::m1(int x) {
	return 5+x;
}

void 
D::m1_void(int x) {
	cout << "D::m1_void() invoked with x = " << x << "." << endl;
}

int
D::m2(int x, int y) {
    return (x + y);
}

C 
D::m1_c(C c) {
    c.cf1 = 20;
    return c;
}

void 
D::m1_ref_c(C &c) {
    c.cf1 = 29;
}
