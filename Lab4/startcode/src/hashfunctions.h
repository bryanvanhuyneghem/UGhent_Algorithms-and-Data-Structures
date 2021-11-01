
#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <string>

unsigned int worst_hash(const std::string &word)
{
    return 0;
}

unsigned int bad_hash(const std::string &word)
{
    return word.size();
}

unsigned int other_bad_hash(const std::string &word)
{
    unsigned int result = 0;

    for (const auto &c : word)
    {
        result += c;
    }

    return result;
}

unsigned int horner_hash(const std::string &word)
{
    unsigned int horner = 0;

    for (const auto &c : word)
    {
        horner = (horner * 256 + c);
    }

    return horner;
}

unsigned int good_hash(const std::string &word)
{
    unsigned int horner = 0;

    for (const char c : word)
    {
        horner = (horner * 131 + c);
    }

    return horner;
}

#endif