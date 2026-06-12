#pragma once

#include "../include/basic.h"
#include <algorithm>

template <typename T>
RSQ1D<T>::RSQ1D (const std::vector<T>& arr) : n(arr.size()) {
    prefix.resize(n + 1, T(0));
    for (size_t i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + arr[i];
    }
}   

template <typename T>
T RSQ1D<T>::query (size_t l, size_t r) const {
    if (l > r || r >= n) return T(0);
    return prefix[r + 1] - prefix[l];
}


template <typename T>
RSQ2D<T>::RSQ2D (const std::vector<std::vector<T>>& matrix) {
    if (matrix.empty()) {
        n = m = 0;
        return;
    }

    n = matrix.size();
    m = matrix[0].size();

    prefix.assign(n + 1, std::vector<T>(m + 1, T(0)));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            prefix[i + 1][j + 1] = matrix[i][j]
                                 + prefix[i + 1][j]
                                 + prefix[i][j + 1]
                                 - prefix[i][j];
        }
    }
}   

template <typename T>
T RSQ2D<T>::query (size_t x1, size_t y1, size_t x2, size_t y2) const {
    if (x1 > x2 || y1 > y2 || x2 >= n || y2 >= m) return T(0);
    return prefix[x2 + 1][y2 + 1]
         - prefix[x1][y2 + 1]
         - prefix[x2 + 1][y1]
         + prefix[x1][y1];
}

template <typename T>
RMQ1D<T>::RMQ1D (const std::vector<T>& arr) : n(arr.size()) {
    if (n == 0) return;

    ranges.assign(n, std::vector<T>(n));

    for (size_t l = 0; l < n; ++l) {
        T min_val = arr[l];
        for (size_t r = l; r < n; ++r) {
            min_val = std::min(min_val, arr[r]);
            ranges[l][r] = min_val;
        }
    }
}

template <typename T>
T RMQ1D<T>::query (size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return T(0);
    return ranges[l][r];
}