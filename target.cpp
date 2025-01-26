#include <iostream>

using namespace std;

const int MAXN = 1002;

int dp[MAXN];
int n;
int book[4] = {10, 20, 50, 100};

int main() {
    cin >> n; 
    dp[0] = 1;
    
    int cost;
    for (int i = 0; i < 4; ++i) {
        cost = book[i];
        for (int j = cost; j <= n; ++j) {
            dp[j] += dp[j-cost];
        }
    }
    cout << dp[n] << endl;

    return 0;
}
