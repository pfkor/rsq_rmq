#pragma once

#include "../include/sparse_table.h"

#include <algorithm>
#include <cmath>

template <typename T, typename BinaryOp>
SparseTable<T, BinaryOp>::SparseTable(const std::vector<T>& arr, BinaryOp op, T identity)
    : n(arr.size()), data(arr), op(op), identity(identity) {
    _build_ops = 0;
    build();
}

template <typename T, typename BinaryOp>
void SparseTable<T, BinaryOp>::build() {
    if (n == 0) return;

    int k = static_cast<int>(std::log2(n)) + 1;
    st.assign(k, std::vector<T>(n));

    for (size_t i = 0; i < n; ++i) st[0][i] = data[i];

    for (int j = 1; (1 << j) <= n; ++j) {
        for (size_t i = 0; i + (1 << j) <= n; ++i) {
            st[j][i] = op(st[j-1][i], st[j-1][i + (1 << (j-1))]);
            _build_ops++;
        }
    }
}

template <typename T, typename BinaryOp>
T SparseTable<T, BinaryOp>::query(size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return identity;

    int j = static_cast<int>(std::log2(r - l + 1));

    _query_ops++;
    return op(st[j][l], st[j][r - (1 << j) + 1]);
}

template <typename T, typename BinaryOp>
void SparseTable<T, BinaryOp>::update(size_t pos, const T& value) {
    if (pos >= n) return;
    data[pos] = value;
    build();
}
