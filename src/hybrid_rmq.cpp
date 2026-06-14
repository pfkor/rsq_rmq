#pragma once

template <typename T, typename BinaryOp>
HybridRMQ<T, BinaryOp>::HybridRMQ(const std::vector<T>& arr, BinaryOp op, T identity)
    : n(arr.size()), data(arr), op(op), identity(identity) {

    if (n == 0) {
        block_size = 1;
        num_blocks = 0;
        return;
    }

    size_t log_n = 0;
    while ((size_t(1) << log_n) <= n) {
        log_n++;
    }
    block_size = std::max(size_t(1), log_n / 2);
    num_blocks = (n + block_size - 1) / block_size;

    lg.assign(std::max(n, num_blocks) + 1, 0);
    for (size_t i = 2; i <= std::max(n, num_blocks); ++i) {
        lg[i] = lg[i / 2] + 1;
    }

    std::vector<T> block_mins(num_blocks, identity);
    _build_ops = 0;
    for (size_t i = 0; i < n; ++i) {
        size_t b = i / block_size;
        if (i % block_size == 0) {
            block_mins[b] = data[i];
        } else {
            block_mins[b] = op(block_mins[b], data[i]);
            _build_ops++;
        }
    }

    build_sparse_table(block_mins);
    build_block_tables();
}

template <typename T, typename BinaryOp>
void HybridRMQ<T, BinaryOp>::build_sparse_table(const std::vector<T>& block_mins) {
    if (num_blocks == 0) return;

    size_t max_log = lg[num_blocks] + 1;
    sparse_table.assign(num_blocks, std::vector<T>(max_log, identity));

    for (size_t i = 0; i < num_blocks; ++i) {
        sparse_table[i][0] = block_mins[i];
    }

    for (size_t j = 1; j < max_log; ++j) {
        for (size_t i = 0; i + (size_t(1) << j) <= num_blocks; ++i) {
            sparse_table[i][j] = op(sparse_table[i][j - 1], sparse_table[i + (size_t(1) << (j - 1))][j - 1]);
            _build_ops++;
        }
    }
}

template <typename T, typename BinaryOp>
void HybridRMQ<T, BinaryOp>::build_block_tables() {
    if (num_blocks == 0) return;

    block_tables.resize(num_blocks);
    size_t max_log = lg[block_size] + 1;

    for (size_t b = 0; b < num_blocks; ++b) {
        size_t start = b * block_size;
        size_t end = std::min(start + block_size, n);
        size_t cur_block_len = end - start;

        block_tables[b].assign(cur_block_len, std::vector<T>(max_log, identity));

        for (size_t i = 0; i < cur_block_len; ++i) {
            block_tables[b][i][0] = data[start + i];
        }

        for (size_t j = 1; j < max_log; ++j) {
            for (size_t i = 0; i + (size_t(1) << j) <= cur_block_len; ++i) {
                block_tables[b][i][j] = op(block_tables[b][i][j - 1], block_tables[b][i + (size_t(1) << (j - 1))][j - 1]);
                _build_ops++;
            }
        }
    }
}

template <typename T, typename BinaryOp>
T HybridRMQ<T, BinaryOp>::query_inside_block(size_t block_idx, size_t l, size_t r) const {
    size_t start = block_idx * block_size;
    size_t local_l = l - start;
    size_t local_r = r - start;
    size_t len = local_r - local_l + 1;
    size_t k = lg[len];
    _query_ops++;
    return op(block_tables[block_idx][local_l][k], block_tables[block_idx][local_r - (size_t(1) << k) + 1][k]);
}

template <typename T, typename BinaryOp>
T HybridRMQ<T, BinaryOp>::query(size_t l, size_t r) const {
    if (n == 0 || l > r || r >= n) return identity;

    size_t left_block = l / block_size;
    size_t right_block = r / block_size;

    if (left_block == right_block) {
        return query_inside_block(left_block, l, r);
    }

    T res_left = query_inside_block(left_block, l, (left_block + 1) * block_size - 1);
    T res_right = query_inside_block(right_block, right_block * block_size, r);
    T res = op(res_left, res_right);
    _query_ops++;

    if (left_block + 1 < right_block) {
        size_t mid_blocks_count = right_block - left_block - 1;
        size_t k = lg[mid_blocks_count];
        T res_mid = op(sparse_table[left_block + 1][k], sparse_table[right_block - (size_t(1) << k)][k]);
        res = op(res, res_mid);
        _query_ops++;
    }

    return res;
}