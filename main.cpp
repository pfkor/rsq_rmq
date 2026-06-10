#include <iostream>
#include <vector>

class PrefixSum {
    std::vector<int> prefix;

public:
    explicit PrefixSum(std::vector<int> v);
    int RSQ(size_t l, size_t r) const;

};

PrefixSum::PrefixSum (std::vector<int> v) {
    size_t size = v.size();
    prefix.resize(size);
    for (size_t i = 0; i != size; ++i) {
        prefix[i] = ((i != 0) ? prefix[i-1] : 0) + v[i];
    }
}

int PrefixSum::RSQ (size_t l, size_t r) const {
    if (l > r || l >= prefix.size() || r >= prefix.size()) return 0;
    return prefix[r] - ((l != 0) ? prefix[l-1] : 0);
}

int main () {

    size_t n;
    std::cin >> n;

    std::vector<int> v (n);
    for (size_t i = 0; i != n; ++i) std::cin >> v[i];

    PrefixSum prefix (v);

    size_t l, r;
    while (true) {
        std::cin >> l >> r;
        std::cout << prefix.RSQ(l, r) << '\n';
    }

    return 0;
}
