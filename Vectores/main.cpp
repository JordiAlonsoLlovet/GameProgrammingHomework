
#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<int> g1;
    vector<int> g2;

    for (int i = 1; i <= 5; i++)
        g1.push_back(i);

    for (int i = 1; i <= 10; i++)
        g2.push_back(i);

    auto x = zip(g1, g2)

    return 0;
}