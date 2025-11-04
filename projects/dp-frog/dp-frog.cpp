#include<iostream>
#include <cstdint>
#include<vector>

using namespace std;


int main() {
    constexpr int N = 7;
    constexpr long int INF = INT64_MAX;
    vector<long int> h{ 2, 9, 4, 5, 1, 6, 10 };
    vector<long int> dp(N, INF);
    dp[0] = 0;

    for (int i = 1; i < N; ++i) {
        if (N == 1) {
            dp[i] = abs(h[i] - h[i-1]);
        }
        else {
            dp[i] = min(dp[i-1] + abs(h[i] - h[i-1]), dp[i-2] + abs(h[i] - h[i-2]));
        }
    }

    cout << dp[N-1] << endl;

    return 0;
}

