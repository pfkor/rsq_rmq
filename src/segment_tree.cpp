#pragma once

#include "../include/segment_tree.h"

#include <algorithm>

template <typename T, typename BinaryOp>
void SegmentTree<T, BinaryOp>::build(const std::vector<T>& arr, size_t node, size_t left, size_t right) {
    if (left == right) {
        tree[node] = arr[left];
        return;
    }
    
    size_t mid = left + (right - left) / 2;
    size_t left_child = node * 2;
    size_t right_child = node * 2 + 1;

    build(arr, left_child, left, mid);
    build(arr, right_child, mid + 1, right);
    
    tree[node] = op(tree[left_child], tree[right_child]);
}

template <typename T, typename BinaryOp>
T SegmentTree<T, BinaryOp>::query(size_t node, size_t left, size_t right, size_t l, size_t r) const {
    if (l > right || r < left) return identity;
    if (l <= left && right <= r) return tree[node];

    size_t mid = left + (right - left) / 2;
    size_t left_child = node * 2;
    size_t right_child = node * 2 + 1;
    
    T left_res = query(left_child, left, mid, l, r);
    T right_res = query(right_child, mid + 1, right, l, r);
    
    return op(left_res, right_res);
}

template <typename T, typename BinaryOp>
SegmentTree<T, BinaryOp>::SegmentTree(const std::vector<T>& arr, BinaryOp op, T identity)
    : n(arr.size()), op(op), identity(identity) {
    if (n == 0) {
        tree.resize(1);
        return;
    }
    tree.resize(4 * n);
    build(arr, 1, 0, n - 1);
}

template <typename T, typename BinaryOp>
T SegmentTree<T, BinaryOp>::query(size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return identity;
    return query(1, 0, n - 1, l, r);
}

template <typename T, typename BinaryOp>
void SegmentTree<T, BinaryOp>::update(size_t pos, const T& value) {
    if (pos >= n) return;
    update_tree(1, 0, n-1, pos, value);
}

template <typename T, typename BinaryOp>
void SegmentTree<T, BinaryOp>::update_tree(size_t node, size_t left, size_t right, size_t pos, const T& value) {
    if (left == right) {
        tree[node] = value;
        return;
    }

    size_t mid = left + (right - left) / 2;
    if (pos <= mid) update_tree(node*2, left, mid, pos, value);
    if (pos > mid) update_tree(node*2+1, mid+1, right, pos, value);
    tree[node] = op(tree[node*2], tree[node*2+1]);
}

