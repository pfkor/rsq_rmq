#pragma once

#include <cstddef>
#include <vector>
#include <functional>

template <typename T, typename BinaryOp>
class SegmentTree {
private:
    size_t n;
    std::vector<T> tree;
    BinaryOp op;
    T identity;

    void build(const std::vector<T>& arr, size_t node, size_t left, size_t right);
    T query(size_t node, size_t left, size_t right, size_t l, size_t r) const;
    void update_tree (size_t node, size_t left, size_t right, size_t pos, const T& value);

public:
    SegmentTree(const std::vector<T>& arr, BinaryOp op, T identity);
    T query(size_t l, size_t r) const;
    void update (size_t pos, const T& value);
};

#include "../src/segment_tree.cpp"