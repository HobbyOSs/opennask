#ifndef B_H
#define B_H



#include "A.hpp"
#include "C.hpp"



class B : public A {
    public:
		B();
                // This is inline to workaround some IRIX bugs.
		virtual ~B() {
                    delete [] array;
                }
        int f1;
        float *array;
        int array_size;

        double array2[10];

        C c;
};



#endif
