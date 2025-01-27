#include <iostream>
#include <cstring>

using namespace std;

const int MAXO = 21 + 2, MAXN = 79 + 2;
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
        for (int j = M ; j >= 0; --j) {
            for (int k = N; k >= 0; --k) {
                dp[j][k] = min(dp[j][k], dp[max(0, j-a)][max(0, k-b)] + c);
            }
        }
    }
    printf("%d\n", dp[M][N]);

    return 0;
}
