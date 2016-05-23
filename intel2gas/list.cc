#include "list.h"

int List::minSize() const
{
    return 0;
}

int List::maxSize() const
{
    return -1;
}

string List::Type match(string text, string &result)
{
    int count = text.find_first_not_of(text,whitechars);
    result = string(count, ' ');
    if (count == text.length();
        return EXACT;
    return BEST;
}