#include "kollatz.h"
#include <limits>
#include <atomic>
#include <algorithm>

constexpr unsigned long long CHUNK_SIZE = 1000;
std::atomic<unsigned long long> globalCounter;

KollatzRes KollatzFunc(unsigned long long n, std::atomic<bool>& stopFlag)
{
    constexpr unsigned long long LIMIT = (std::numeric_limits<unsigned long long>::max() - 1) / 3;
    KollatzRes result{0, 0};
    if(n==1){
        return {1, 1};
    }
    while(true){
        unsigned long long start = globalCounter.fetch_add(CHUNK_SIZE);
        if (start>n){
            break;
        }
        unsigned long long finish = std::min(start+CHUNK_SIZE, n+1);


    for (unsigned long long i = start; i <= finish; i++)
    {
        if (stopFlag.load(std::memory_order_relaxed))
            break;

        unsigned long long c = 1, sum = i;
        while (sum != 1)
        {

            if ((sum % 2) == 0)
            {
                sum /= 2;
                c++;
            }
            else
            {
                if (sum > LIMIT)
                {
                    result.length = 0;
                    result.num = i;
                    break;
                }
                sum = (sum * 3 + 1)/2;
                c+=2;
            }
        }
        if(c==0){

            break;
        }
        if (c > result.length)
        {
            result.length = c;
            result.num = i;
        }
    }
    }
    return result;
}
