#include <iostream>
#include <stdint.h>
#include <vector>
using namespace std;

template<typename T> void chmin(T& a, T b) {
    if (a > b) {
        a = b;
    }
}

int main() {
    constexpr int N = 7;
    constexpr long int INF = INT64_MAX;
    vector<long int> h{ 2, 9, 4, 5, 1, 6, 10 };
    vector<long int> dp(N, INF);
    dp[0] = 0;

    for (int i = 1; i < N; ++i) {
        chmin(dp[i], dp[i-1] + abs(h[i] - h[i-1]));
        if (i > 1) {
            chmin(dp[i], dp[i-2] + abs(h[i] - h[i-2]));
        }
    }

    cout << dp[N-1] << endl;

    return 0;
}