
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

// Jenkins hash function
unsigned int jenkins_one_at_a_time_hash(const std::string &word){
    int i = 0;
    unsigned int hash = 0;
    while( i != word.length()){
        hash += word[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

// Dan Bernstein's algorithm with k = 33
unsigned int djb2(const std::string &word){
    unsigned int hash = 5381;
    int i = 0;
    int c;
    while(i != word.length() && (c = word[i++])){
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Sdbm
unsigned int sdbm(const std::string &word){
    unsigned int hash = 0;
    int i = 0;
    int c;
    while(i != word.length() && (c = word[i++])){
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// lose lose
unsigned int lose_lose(const std::string &word){
    unsigned int hash = 0;
    int i = 0;
    int c;
    while(i != word.length() && (c = word[i++])){
        hash += c;
    }
    return hash;
}

// found this in a dark alley
unsigned int rolling_hash_but_not_really(const std::string &word){
    const unsigned PRIME_BASE = 251; // https://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf
    const int unsigned PRIME_MOD = 4294967291;
    unsigned int rollingHash = 0;
    int i = 0;
    for (const char c : word)
    {
        rollingHash = rollingHash*PRIME_BASE + c;
        rollingHash %= PRIME_MOD;
    }
    return rollingHash;
}


#endif