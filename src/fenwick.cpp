#pragma once

#include "../include/fenwick.h"

template <typename T>
FenwickTree<T>::FenwickTree(const std::vector<T>& arr) : n(arr.size()), data(arr) {
    bit.assign(n + 1, T(0));
    for (size_t i = 0; i < n; ++i) {
        // добавляем arr[i] в структуру
        size_t idx = i + 1;
        while (idx <= n) {
            bit[idx] += arr[i];
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
        idx -= idx & -idx;
    }
    return res;
}

template <typename T>
void FenwickTree<T>::update(size_t pos, const T& value) {
    if (pos >= n) return;
    T delta = value - data[pos];
    data[pos] = value;
    size_t idx = pos + 1;
    while (idx <= n) {
        bit[idx] += delta;
        idx += idx & -idx;
    }
}

template <typename T>
T FenwickTree<T>::query(size_t l, size_t r) const {
    if (l > r || r >= n) return T(0);
    T res = prefix_sum(r);
    if (l > 0) res -= prefix_sum(l - 1);
    return res;
}