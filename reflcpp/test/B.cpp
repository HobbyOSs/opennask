#include "B.hpp"

B::B() : array(new float[5]), array_size(5) {
    for (int i = 0; i < 5; i++) {
        // Cast to avoid loss-of-precision warning.
        array[i] = float(i);
    }
}

