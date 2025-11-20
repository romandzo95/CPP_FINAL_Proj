#ifndef KOLLATZ_H
#define KOLLATZ_H
#include <atomic>

struct KollatzRes
{
    unsigned long long num;
    unsigned long long length;
};
extern std::atomic<unsigned long long> globalCounter;
KollatzRes KollatzFunc(unsigned long long n, std::atomic<bool>& stopFlag);
#endif // KOLLATZ_H
