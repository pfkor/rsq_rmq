#pragma once
#include <cstddef>
#include <vector>
#include <cmath>
#include <algorithm>

template <typename T, typename BinaryOp>
class HybridRMQ {
private:
    size_t n;
    size_t block_size;
    size_t num_blocks;
    const std::vector<T>& data;
    BinaryOp op;
    T identity;

    std::vector<std::vector<T>> sparse_table;
    std::vector<size_t> lg;
    std::vector<std::vector<std::vector<T>>> block_tables;

    void build_sparse_table(const std::vector<T>& block_mins);
    void build_block_tables();
    T query_inside_block(size_t block_idx, size_t l, size_t r) const;

    mutable size_t _build_ops = 0;
    mutable size_t _query_ops = 0;

public:
    HybridRMQ(const std::vector<T>& arr, BinaryOp op, T identity);
    T query(size_t l, size_t r) const;

    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
};

#include "../src/hybrid_rmq.cpp"