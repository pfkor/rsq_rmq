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

    mutable size_t _build_ops = 0;
    mutable size_t _query_ops = 0;

public:
    SparseTable(const std::vector<T>& arr, BinaryOp op, T identity);

    T query(size_t l, size_t r) const;
    void update(size_t pos, const T& value);
    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
};

#include "../src/sparse_table.cpp"