// main.cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <limits>
#include <cassert>
#include "./include/basic.h"          // RSQ1D, RSQ2D, RMQ1D
#include "./include/sqrt_decomp.h"    // SqrtDecomposition
#include "./include/segment_tree.h"   // SegmentTree

using namespace std;

template <typename T>
void print_result(const string& test_name, T expected, T got) {
    cout << test_name << ": ";
    if (expected == got) {
        cout << "OK (got " << got << ")\n";
    } else {
        cout << "FAIL! expected " << expected << ", got " << got << "\n";
    }
}

void test_rsq1d() {
    cout << "\n=== RSQ1D (префиксные суммы) ===\n";
    vector<int> arr = {1, 2, 3, 4, 5};
    RSQ1D<int> rsq(arr);

    print_result("Сумма [0,4]", 15, rsq.query(0, 4));
    print_result("Сумма [1,3]", 9, rsq.query(1, 3));
    print_result("Сумма [2,2]", 3, rsq.query(2, 2));
    print_result("Некорректный l>r", 0, rsq.query(3, 1));
    print_result("Выход за r", 0, rsq.query(0, 10));

    vector<int> empty;
    RSQ1D<int> rsq_empty(empty);
    print_result("Пустой массив", 0, rsq_empty.query(0, 0));
}

void test_rsq2d() {
    cout << "\n=== RSQ2D (2D префиксные суммы) ===\n";
    vector<vector<int>> mat = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    RSQ2D<int> rsq2d(mat);
    print_result("Весь массив", 45, rsq2d.query(0, 0, 2, 2));
    print_result("Элемент (1,1)", 5, rsq2d.query(1, 1, 1, 1));
    print_result("Прямоугольник 2x2", 16, rsq2d.query(0, 1, 1, 2));
    print_result("x1>x2", 0, rsq2d.query(2, 0, 1, 2));
    print_result("Выход за границы", 0, rsq2d.query(0, 0, 3, 2));

    vector<vector<int>> empty_mat;
    RSQ2D<int> rsq2d_empty(empty_mat);
    print_result("Пустая матрица", 0, rsq2d_empty.query(0, 0, 0, 0));
}

void test_rmq1d() {
    cout << "\n=== RMQ1D (предпросчёт всех отрезков) ===\n";
    vector<int> arr = {5, 2, 8, 1, 9};
    RMQ1D<int> rmq(arr);
    print_result("Минимум [0,4]", 1, rmq.query(0, 4));
    print_result("Минимум [1,3]", 1, rmq.query(1, 3));
    print_result("Минимум [0,1]", 2, rmq.query(0, 1));
    print_result("Минимум [3,3]", 1, rmq.query(3, 3));
    print_result("Некорректный запрос", 0, rmq.query(4, 2));

    vector<int> empty;
    RMQ1D<int> rmq_empty(empty);
    print_result("Пустой массив", 0, rmq_empty.query(0, 0));
}

void test_sqrt_decomposition() {
    cout << "\n=== SqrtDecomposition (корневая декомпозиция) ===\n";

    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto sum_op = [](int a, int b) { return a + b; };
    SqrtDecomposition<int, decltype(sum_op)> sqrt_sum(arr, sum_op, 0);

    print_result("Сумма [0,9]", 55, sqrt_sum.query(0, 9));
    print_result("Сумма [2,5]", 18, sqrt_sum.query(2, 5));
    print_result("Сумма [0,0]", 1, sqrt_sum.query(0, 0));
    print_result("Сумма [9,9]", 10, sqrt_sum.query(9, 9));
    print_result("Сумма вне границ", 0, sqrt_sum.query(5, 15));

    auto min_op = [](int a, int b) { return std::min(a, b); };
    int inf = numeric_limits<int>::max();
    SqrtDecomposition<int, decltype(min_op)> sqrt_min(arr, min_op, inf);

    print_result("Минимум [0,9]", 1, sqrt_min.query(0, 9));
    print_result("Минимум [2,5]", 3, sqrt_min.query(2, 5));
    print_result("Минимум [4,7]", 5, sqrt_min.query(4, 7));
    print_result("Минимум [0,0]", 1, sqrt_min.query(0, 0));
    print_result("l>r", inf, sqrt_min.query(5, 2));

    vector<int> empty;
    SqrtDecomposition<int, decltype(sum_op)> sqrt_empty(empty, sum_op, 0);
    print_result("Пустой массив", 0, sqrt_empty.query(0, 0));
}

void test_segment_tree() {
    cout << "\n=== SegmentTree (дерево отрезков) ===\n";

    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // RSQ через дерево отрезков
    auto sum_op = [](int a, int b) { return a + b; };
    SegmentTree<int, decltype(sum_op)> seg_sum(arr, sum_op, 0);

    print_result("Сумма [0,9]", 55, seg_sum.query(0, 9));
    print_result("Сумма [2,5]", 18, seg_sum.query(2, 5));
    print_result("Сумма [0,0]", 1, seg_sum.query(0, 0));
    print_result("Сумма [9,9]", 10, seg_sum.query(9, 9));
    print_result("Сумма l>r", 0, seg_sum.query(5, 2));
    print_result("Сумма вне границ", 0, seg_sum.query(0, 15));

    // RMQ через дерево отрезков (минимум)
    auto min_op = [](int a, int b) { return std::min(a, b); };
    int inf = numeric_limits<int>::max();
    SegmentTree<int, decltype(min_op)> seg_min(arr, min_op, inf);

    print_result("Минимум [0,9]", 1, seg_min.query(0, 9));
    print_result("Минимум [2,5]", 3, seg_min.query(2, 5));
    print_result("Минимум [4,7]", 5, seg_min.query(4, 7));
    print_result("Минимум [0,0]", 1, seg_min.query(0, 0));
    print_result("Минимум l>r", inf, seg_min.query(5, 2));

    // Пустой массив
    vector<int> empty;
    SegmentTree<int, decltype(sum_op)> seg_empty(empty, sum_op, 0);
    print_result("Пустой массив (сумма)", 0, seg_empty.query(0, 0));
}

int main() {
    test_rsq1d();
    test_rsq2d();
    test_rmq1d();
    test_sqrt_decomposition();
    test_segment_tree();
    return 0;
}