#pragma once
#include <cstddef>
#include <vector>
#include <functional>

template <typename T, typename BinaryOp>
class SqrtDecomposition {
private:
    size_t n;
    size_t block_size;
    size_t num_blocks;
    std::vector<T> data;
    std::vector<T> block_agg;
    BinaryOp op;
    T identity;

public:
    SqrtDecomposition(const std::vector<T>& arr, BinaryOp op, T identity);
    T query(size_t l, size_t r) const;
    void update (size_t pos, const T& value);
};

#include "../src/sqrt_decomp.cpp"