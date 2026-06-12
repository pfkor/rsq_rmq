#pragma once
#include <cstddef>
#include <vector>

template <typename T>
class FenwickTree {
private:
    size_t n;
    std::vector<T> bit;
    std::vector<T> data;

    T prefix_sum (size_t idx) const;

public:
    FenwickTree(const std::vector<T>& arr);

    void update(size_t idx, const T& value);
    T query(size_t l, size_t r) const;
};

#include "../src/fenwick.cpp"