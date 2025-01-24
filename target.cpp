#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXO = 22, MAXN = 80;
const int NUL = 0x3f3f3f3f;

int M/* Oxygen */, N/* Nitrogen */, K;
int dp[MAXO][MAXN];

int main() {
    scanf("%d%d%d", &M, &N, &K);

    memset(dp, 0x3f, sizeof(dp));
    dp[0][0] = 0;

    int a, b, c;
    for (int i = 0; i < K; ++i) {
        scanf("%d%d%d", &a, &b, &c);
        for (int j = MAXO - 1; j >= a; --j) {
            for (int k = MAXN - 1; k >= b; --k) {
                dp[j][k] = min(dp[j][k], dp[j-a][k-b] + c);
            }
        }
    }
    cout << dp[M][N] << endl;

    int res = NUL;
    for (int j = M; j < MAXO; ++j) {
        for (int k = N; k < MAXN; ++k) {
            res = min(res, dp[j][k]);
        }
    }

    printf("%d\n", res);

    return 0;
}


//done