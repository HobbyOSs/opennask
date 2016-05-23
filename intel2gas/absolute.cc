#include "absolute.h"

int Absolute::minSize() const
{
    return correct.length();
}

int Absolute::maxSize() const
{
    return correct.length();
}

string Absolute::Type match(string text, string &result)
{
    if (text == correct) {
        result = correct;
        return EXACT;
    }
    return NOTHING;
}