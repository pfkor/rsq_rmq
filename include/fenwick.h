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

    mutable size_t _query_ops = 0;
    mutable size_t _build_ops = 0;
    mutable size_t _update_ops = 0;

public:
    FenwickTree(const std::vector<T>& arr);

    void update(size_t idx, const T& value);
    T query(size_t l, size_t r) const;

    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    void reset_update_ops() const { _update_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
    size_t get_update_ops() const { return _update_ops; }
};

#include "../src/fenwick.cpp"