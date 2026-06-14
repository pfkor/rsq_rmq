#pragma once
#include <cstddef>
#include <vector>

template <typename T>
class BasicRangeQuery {
public:
    virtual ~BasicRangeQuery() = default;
    virtual T query(size_t l, size_t r) const = 0;
    virtual void update (size_t pos, const T& value) = 0;
};

template <typename T>
class RSQ1D : public BasicRangeQuery<T> {
private:
    mutable size_t _query_ops = 0;
    mutable size_t _build_ops = 0;
    size_t n;
    std::vector<T> data;
    std::vector<T> prefix;
    void rebuild();

public:
    RSQ1D(const std::vector<T>& arr);
    T query(size_t l, size_t r) const override;
    void update (size_t pos, const T& value) override;
    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
};

template <typename T>
class RSQ2D {
private:
    size_t n, m;
    std::vector<std::vector<T>> data;
    std::vector<std::vector<T>> prefix;
    void rebuild();
    mutable size_t _query_ops = 0;
    mutable size_t _build_ops = 0;

public:
    RSQ2D(const std::vector<std::vector<T>>& matrix);
    T query(size_t x1, size_t y1, size_t x2, size_t y2) const;
    void update (size_t x, size_t y, const T& value);
    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
};

template <typename T>
class RMQ1D : public BasicRangeQuery<T> {
private:
    size_t n;
    std::vector<T> data;
    std::vector<std::vector<T>> ranges;
    void rebuild();
    mutable size_t _query_ops = 0;
    mutable size_t _build_ops = 0;

public:
    RMQ1D(const std::vector<T>& arr);
    T query(size_t l, size_t r) const override;
    void update (size_t pos, const T& value) override;
    void reset_query_ops() const { _query_ops = 0; }
    void reset_build_ops() const { _build_ops = 0; }
    size_t get_build_ops() const { return _build_ops; }
    size_t get_query_ops() const { return _query_ops; }
};

#include "../src/basic.cpp"