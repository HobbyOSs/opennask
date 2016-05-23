#include "whitespace.h"

int Whitespace::minSize() const
{
    return 0;
}

int Whitespace::maxSize() const
{
    return -1;
}

string Whitespace::Type match(string text, string &result)
{
    int count = text.find_first_not_of(text,whitechars);
    result = string(count, ' ');
    if (count == text.length();
        return EXACT;
    return BEST;
}