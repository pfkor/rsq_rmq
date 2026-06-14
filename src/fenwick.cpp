#pragma once

#include "../include/fenwick.h"

template <typename T>
FenwickTree<T>::FenwickTree(const std::vector<T>& arr) : n(arr.size()), data(arr) {
    _build_ops = 0;
    bit.assign(n + 1, T(0));
    for (size_t i = 0; i < n; ++i) {
        size_t idx = i + 1;
        while (idx <= n) {
            bit[idx] += arr[i];
            _build_ops++;
            idx += idx & -idx;
        }
    }
}

template <typename T>
T FenwickTree<T>::prefix_sum(size_t idx) const {
    T res = T(0);
    ++idx;
    while (idx > 0) {
        res += bit[idx];
        _query_ops++;
        idx -= idx & -idx;
    }
    return res;
}

template <typename T>
void FenwickTree<T>::update(size_t pos, const T& value) {
    if (pos >= n) return;
    T delta = value - data[pos];
    _update_ops++;
    data[pos] = value;
    size_t idx = pos + 1;
    while (idx <= n) {
        bit[idx] += delta;
        _update_ops++;
        idx += idx & -idx;
    }
}

template <typename T>
T FenwickTree<T>::query(size_t l, size_t r) const {
    if (l > r || r >= n) return T(0);
    T res = prefix_sum(r);
    if (l > 0) {
        res -= prefix_sum(l - 1);
        _query_ops++;
    }
    return res;
}