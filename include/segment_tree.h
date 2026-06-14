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

    mutable size_t _build_ops = 0;
    mutable size_t _query_ops = 0;
    mutable size_t _update_ops = 0;

public:
    SegmentTree(const std::vector<T>& arr, BinaryOp op, T identity);
    T query(size_t l, size_t r) const;
    void update (size_t pos, const T& value);

    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    void reset_update_ops() const { _update_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
    size_t get_update_ops() const { return _update_ops; }
};

#include "../src/segment_tree.cpp"