#include <iostream>
#include <cstring>

using namespace std;

const int MAXO = 21 + 2, MAXN = 79 + 2;
const int NUL = 0x3f3f3f3f;

int M/* Oxygen */, N/* Nitrogen */, K;
int dp[MAXO][MAXN];

struct Test {
    int v, w;
};

inline bool testOneLine(int var) { return false; }

inline bool testTurmoil(int var) { // still has something here
    int i;
    while (true) {
        int b;
    false; } // still has something before and after

    while (true) {
        int b;
    } // still has something
    return true;
}

void testAnotherFormat() 
{ 
    // has space
    int i, j, k;
    for (int i = 0; i < 0; ++i) 
    { 
        // has space
        "nonesense";
        {
            // has no space
            true;
        }
    }
}

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

    for (int i = 0; i < 1; ++i)
    {
        int b = 0;
        { 
            false;
        }
    }

    return 0;
}
