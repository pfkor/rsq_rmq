#pragma once
#include <cstddef>
#include <vector>

template <typename T>
class BasicRangeQuery {
public:
    virtual ~BasicRangeQuery() = default;
    virtual T query(size_t l, size_t r) const = 0;
};

template <typename T>
class RSQ1D : public BasicRangeQuery<T> {
private:
    size_t n;
    std::vector<T> prefix;

public:
    RSQ1D(const std::vector<T>& arr);
    T query(size_t l, size_t r) const override;
};

template <typename T>
class RSQ2D {
private:
    size_t n, m;
    std::vector<std::vector<T>> prefix;

public:
    RSQ2D(const std::vector<std::vector<T>>& matrix);
    T query(size_t x1, size_t y1, size_t x2, size_t y2) const;
};

template <typename T>
class RMQ1D : public BasicRangeQuery<T> {
private:
    size_t n;
    std::vector<std::vector<T>> ranges;

public:
    RMQ1D(const std::vector<T>& arr);
    T query(size_t l, size_t r) const override;
};

#include "../src/basic.cpp"