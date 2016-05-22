#ifndef D_HPP
#define D_HPP

#include "C.hpp"

class D {
public:
    D() {}
    virtual ~D() {}

    int m0();
    void m0_void();
    int m1(int x);
    void m1_void(int x);
    int m2(int x, int y);
    C m1_c(C c);
	void m1_ref_c(C &c);
};



#endif
