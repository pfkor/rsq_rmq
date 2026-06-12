// main.cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <limits>
#include <cmath>
#include <algorithm>
#include "./include/basic.h"
#include "./include/sqrt_decomp.h"
#include "./include/segment_tree.h"
#include "./include/fenwick.h"
#include "./include/sparse_table.h"
// #include "./include/hybrid_rmq.h"

using namespace std;

// Утилита для вывода результатов теста
template <typename T>
void print_test(const string& name, T expected, T got) {
    cout << name << ": ";
    if (expected == got)
        cout << "OK (got " << got << ")\n";
    else
        cout << "FAIL! expected " << expected << ", got " << got << "\n";
}

// Прямой перебор для проверки RMQ
int brute_min(const vector<int>& arr, size_t l, size_t r) {
    int res = numeric_limits<int>::max();
    for (size_t i = l; i <= r; ++i) res = min(res, arr[i]);
    return res;
}

int brute_sum(const vector<int>& arr, size_t l, size_t r) {
    int res = 0;
    for (size_t i = l; i <= r; ++i) res += arr[i];
    return res;
}

// --------------------------------------------------------------
// Тесты для RSQ1D (префиксные суммы) с обновлением
void test_rsq1d() {
    cout << "\n=== RSQ1D (префиксные суммы) ===\n";
    vector<int> arr = {1, 2, 3, 4, 5};
    RSQ1D<int> rsq(arr);

    print_test("Сумма [0,4] до обновления", 15, rsq.query(0,4));
    print_test("Сумма [1,3]", 9, rsq.query(1,3));

    rsq.update(2, 10);  // стало {1,2,10,4,5}
    print_test("Сумма [0,4] после update(2,10)", 1+2+10+4+5, rsq.query(0,4));
    print_test("Сумма [2,4] после update", 10+4+5, rsq.query(2,4));

    rsq.update(0, 100);
    print_test("Сумма [0,0] после update(0,100)", 100, rsq.query(0,0));
}

// --------------------------------------------------------------
// Тесты для RSQ2D
void test_rsq2d() {
    cout << "\n=== RSQ2D (2D префиксные суммы) ===\n";
    vector<vector<int>> mat = {{1,2,3},{4,5,6},{7,8,9}};
    RSQ2D<int> rsq2d(mat);
    print_test("Сумма всей матрицы", 45, rsq2d.query(0,0,2,2));

    rsq2d.update(1,1, 50);  // заменяем 5 на 50
    // новая матрица: {{1,2,3},{4,50,6},{7,8,9}} сумма = 1+2+3+4+50+6+7+8+9 = 90
    print_test("Сумма всей после update(1,1,50)", 90, rsq2d.query(0,0,2,2));
    print_test("Подматрица (1,1)-(2,2) после update", 50+6+8+9, rsq2d.query(1,1,2,2));
}

// --------------------------------------------------------------
// Тесты для RMQ1D (предпросчёт всех отрезков)
void test_rmq1d() {
    cout << "\n=== RMQ1D (таблица всех отрезков) ===\n";
    vector<int> arr = {5,2,8,1,9};
    RMQ1D<int> rmq(arr);
    print_test("Минимум [0,4]", 1, rmq.query(0,4));
    print_test("Минимум [1,3]", 1, rmq.query(1,3));

    rmq.update(3, 10);  // стало {5,2,8,10,9}, минимум на [0,4] = 2
    print_test("Минимум [0,4] после update(3,10)", 2, rmq.query(0,4));
    print_test("Минимум [2,4] после update", min({8,10,9}), rmq.query(2,4));
}

// --------------------------------------------------------------
// Тесты для SqrtDecomposition (с обновлением)
void test_sqrt_decomp() {
    cout << "\n=== SqrtDecomposition (корневая декомпозиция) ===\n";
    vector<int> arr = {1,2,3,4,5,6,7,8,9,10};
    auto sum_op = [](int a,int b){ return a+b; };
    SqrtDecomposition<int, decltype(sum_op)> sqrt_sum(arr, sum_op, 0);
    print_test("Сумма [0,9] до обновления", 55, sqrt_sum.query(0,9));

    sqrt_sum.update(5, 100); // arr[5] было 6, стало 100
    print_test("Сумма [0,9] после update(5,100)", 55 -6 +100, sqrt_sum.query(0,9));
    print_test("Сумма [5,5] после update", 100, sqrt_sum.query(5,5));

    // Тест минимума
    auto min_op = [](int a,int b){ return min(a,b); };
    int inf = numeric_limits<int>::max();
    SqrtDecomposition<int, decltype(min_op)> sqrt_min(arr, min_op, inf);
    print_test("Минимум [0,9] до update", 1, sqrt_min.query(0,9));
    sqrt_min.update(3, -5);
    print_test("Минимум [0,9] после update(3,-5)", -5, sqrt_min.query(0,9));
}

// --------------------------------------------------------------
// Тесты для SegmentTree
void test_segment_tree() {
    cout << "\n=== SegmentTree (дерево отрезков) ===\n";
    vector<int> arr = {1,2,3,4,5,6,7,8,9,10};
    auto sum_op = [](int a,int b){ return a+b; };
    SegmentTree<int, decltype(sum_op)> seg_sum(arr, sum_op, 0);
    print_test("Сумма [2,7] до обновления", 3+4+5+6+7+8, seg_sum.query(2,7));

    seg_sum.update(4, 50);
    print_test("Сумма [2,7] после update(4,50)", 3+4+50+6+7+8, seg_sum.query(2,7));

    auto min_op = [](int a,int b){ return min(a,b); };
    int inf = numeric_limits<int>::max();
    SegmentTree<int, decltype(min_op)> seg_min(arr, min_op, inf);
    print_test("Минимум [0,9] до update", 1, seg_min.query(0,9));
    seg_min.update(0, -10);
    print_test("Минимум [0,9] после update(0,-10)", -10, seg_min.query(0,9));
}

// --------------------------------------------------------------
// Тесты для FenwickTree (RSQ)
void test_fenwick() {
    cout << "\n=== FenwickTree (дерево Фенвика) ===\n";
    vector<int> arr = {1,2,3,4,5};
    FenwickTree<int> fw(arr);
    print_test("Сумма [1,3]", 2+3+4, fw.query(1,3));
    print_test("Сумма [0,4]", 15, fw.query(0,4));

    fw.update(2, 10); // {1,2,10,4,5}
    print_test("Сумма [0,4] после update(2,10)", 1+2+10+4+5, fw.query(0,4));
    fw.update(4, 0);
    print_test("Сумма [3,4] после update(4,0)", 4+0, fw.query(3,4));
}

// --------------------------------------------------------------
// Тесты для SparseTable (RMQ) – обновление перестраивает таблицу
void test_sparse_table() {
    cout << "\n=== SparseTable (разреженная таблица) ===\n";
    vector<int> arr = {5,3,8,1,9,2};
    auto min_op = [](int a,int b){ return min(a,b); };
    int inf = numeric_limits<int>::max();
    SparseTable<int, decltype(min_op)> st(arr, min_op, inf);
    print_test("Минимум [0,5]", 1, st.query(0,5));
    print_test("Минимум [2,4]", min({8,1,9}), st.query(2,4));

    st.update(3, 100);
    print_test("Минимум [0,5] после update(3,100)", 2, st.query(0,5));
    print_test("Минимум [2,4] после update", min({8,100,9}), st.query(2,4));
}

// // --------------------------------------------------------------
// // Тесты для HybridRMQ (корневая + sparse table)
// void test_hybrid_rmq() {
//     cout << "\n=== HybridRMQ (гибрид) ===\n";
//     vector<int> arr = {10, 20, 5, 7, 30, 1, 25, 3};
//     auto min_op = [](int a,int b){ return min(a,b); };
//     int inf = numeric_limits<int>::max();
//     // размер блока выберем 3 для наглядности
//     HybridRMQ<int, decltype(min_op)> hybrid(arr, min_op, inf, 3);
//     print_test("Минимум [1,6]", min({20,5,7,30,1,25}), hybrid.query(1,6));
//     print_test("Минимум [0,7]", 1, hybrid.query(0,7));

//     hybrid.update(5, 100); // arr[5] было 1, стало 100
//     print_test("Минимум [0,7] после update(5,100)", min({10,20,5,7,30,100,25,3}), hybrid.query(0,7));
//     print_test("Минимум [4,6] после update", min(30,100,25), hybrid.query(4,6));

//     hybrid.update(2, -10);
//     print_test("Минимум [0,7] после update(2,-10)", -10, hybrid.query(0,7));
// }

// --------------------------------------------------------------
int main() {
    test_rsq1d();
    test_rsq2d();
    test_rmq1d();
    test_sqrt_decomp();
    test_segment_tree();
    test_fenwick();
    test_sparse_table();
    // test_hybrid_rmq();

    cout << "\nВсе тесты завершены.\n";
    return 0;
}