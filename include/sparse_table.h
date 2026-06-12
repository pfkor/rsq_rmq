#pragma once
#include <cstddef>
#include <vector>
#include <functional>
#include <cmath>

template <typename T, typename BinaryOp>
class SparseTable {
private:
    size_t n;
    std::vector<T> data;
    std::vector<std::vector<T>> st;

    BinaryOp op;
    T identity;

    void build();

public:
    SparseTable(const std::vector<T>& arr, BinaryOp op, T identity);

    T query(size_t l, size_t r) const;
    void update(size_t pos, const T& value);
};

#include "../src/sparse_table.cpp"