#pragma once

#include "../include/basic.h"
#include <algorithm>

template <typename T>
RSQ1D<T>::RSQ1D (const std::vector<T>& arr) : n(arr.size()), data(arr) {
    rebuild();
}   

template <typename T>
void RSQ1D<T>::rebuild() {
    _build_ops = 0;
    prefix.resize(n + 1, T(0));
    for (size_t i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + data[i];
        _build_ops++;
    }
}

template <typename T>
T RSQ1D<T>::query (size_t l, size_t r) const {
    if (l > r || r >= n) return T(0);
    _query_ops++;
    return prefix[r + 1] - prefix[l];
}

template <typename T>
void RSQ1D<T>::update (size_t pos, const T& value) {
    if (pos >= n) return;
    data[pos] = value;
    rebuild();
}

template <typename T>
RSQ2D<T>::RSQ2D (const std::vector<std::vector<T>>& matrix) {
    if (matrix.empty()) {
        n = m = 0;
        return;
    }

    n = matrix.size();
    m = matrix[0].size();
    data = matrix;

    rebuild();
}

template <typename T>
void RSQ2D<T>::rebuild() {
    _build_ops = 0;
    prefix.assign(n + 1, std::vector<T>(m + 1, T(0)));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            prefix[i + 1][j + 1] = data[i][j]
                                 + prefix[i + 1][j]
                                 + prefix[i][j + 1]
                                 - prefix[i][j];
            _build_ops += 3;
        }
    }
}


template <typename T>
T RSQ2D<T>::query (size_t x1, size_t y1, size_t x2, size_t y2) const {
    if (x1 > x2 || y1 > y2 || x2 >= n || y2 >= m) return T(0);
    _query_ops += 3;
    return prefix[x2 + 1][y2 + 1]
         - prefix[x1][y2 + 1]
         - prefix[x2 + 1][y1]
         + prefix[x1][y1];
}

template <typename T>
void RSQ2D<T>::update (size_t x, size_t y, const T& value) {
    if (x >= n || y >= n) return;
    data[x][y] = value;
    rebuild();
}

template <typename T>
RMQ1D<T>::RMQ1D (const std::vector<T>& arr) : n(arr.size()), data(arr) {
    rebuild();
}

template <typename T>
void RMQ1D<T>::rebuild() {
    _build_ops = 0;
    ranges.assign(n, std::vector<T>(n));

    for (size_t l = 0; l < n; ++l) {
        T min_val = data[l];
        for (size_t r = l; r < n; ++r) {
            if (r > l) {
                min_val = std::min(min_val, data[r]);
                _build_ops++;
            }
            ranges[l][r] = min_val;
        }
    }
}

template <typename T>
T RMQ1D<T>::query (size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return T(0);
    return ranges[l][r];
}

template <typename T>
void RMQ1D<T>::update (size_t pos, const T& value) {
    if (pos >= n) return;
    data[pos] = value;
    rebuild();
}