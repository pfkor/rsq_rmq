#pragma once

#include "../include/sqrt_decomp.h"

#include <cmath>
#include <algorithm>

template <typename T, typename BinaryOp>
SqrtDecomposition<T, BinaryOp>::SqrtDecomposition(const std::vector<T>& arr, BinaryOp op, T identity) 
    : n(arr.size()), op(op), identity(identity) {

    if (n == 0) {
        block_size = 1;
        num_blocks = 0;
        return;
    }

    block_size = static_cast<size_t>(std::sqrt(n)) + 1;
    num_blocks = (n + block_size - 1) / block_size;

    data = arr;
    block_agg.assign(num_blocks, identity);

    _build_ops = 0;
    for (size_t i = 0; i < n; ++i) {
        size_t b = i / block_size;
        block_agg[b] = op(block_agg[b], data[i]);
        _build_ops++;
    }
}

template <typename T, typename BinaryOp>
T SqrtDecomposition<T, BinaryOp>::query(size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return identity;

    size_t left_block = l / block_size;
    size_t right_block = r / block_size;
    T res = identity;

    _query_ops = 0;

    if (left_block == right_block) {
        for (size_t i = l; i <= r; ++i) {
            res = op(res, data[i]);
            _query_ops++;
        }
        return res;
    }

    size_t left_end = (left_block + 1) * block_size - 1;
    for (size_t i = l; i <= left_end; ++i) {
        res = op(res, data[i]);
        _query_ops++;
    }

    for (size_t b = left_block + 1; b < right_block; ++b) {
        res = op(res, block_agg[b]);
        _query_ops++;
    }

    size_t right_start = right_block * block_size;
    for (size_t i = right_start; i <= r; ++i) {
        res = op(res, data[i]);
        _query_ops++;
    }

    return res;
}

template <typename T, typename BinaryOp>
void SqrtDecomposition<T, BinaryOp>::update (size_t pos, const T& value) {
    if (pos >= n) return;

    data[pos] = value;
    size_t block = pos / block_size;

    T new_agg = identity;
    size_t start = block * block_size;
    size_t end = std::min(start + block_size, n);

    for (size_t i = start; i < end; ++i) {
        new_agg = op(new_agg, data[i]);
    }
    block_agg[block] = new_agg;
}