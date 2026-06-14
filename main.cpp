// main.cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <limits>
#include <cmath>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>
#include "./include/basic.h"
#include "./include/sqrt_decomp.h"
#include "./include/segment_tree.h"
#include "./include/fenwick.h"
#include "./include/sparse_table.h"
#include "./include/hybrid_rmq.h"
#include <fstream>

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

// --------------------------------------------------------------
void test_hybrid_rmq() {
    cout << "\n=== HybridRMQ (корневая + разреженная таблица) ===\n";
    vector<int> arr = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0, 11, 15, 3};
    auto min_op = [](int a, int b) { return min(a, b); };
    int inf = numeric_limits<int>::max();

    HybridRMQ<int, decltype(min_op)> h_rmq(arr, min_op, inf);

    print_test("Минимум на всем массиве [0,12]", brute_min(arr, 0, 12), h_rmq.query(0, 12));
    print_test("Минимум внутри одного первого блока [0,2]", brute_min(arr, 0, 2), h_rmq.query(0, 2));
    print_test("Минимум внутри одного среднего блока [4,5]", brute_min(arr, 4, 5), h_rmq.query(4, 5));
    print_test("Минимум на стыке двух блоков [2,4]", brute_min(arr, 2, 4), h_rmq.query(2, 4));
    print_test("Минимум через несколько блоков [1,8]", brute_min(arr, 1, 8), h_rmq.query(1, 8));
    print_test("Запрос из одного элемента [5,5]", brute_min(arr, 5, 5), h_rmq.query(5, 5));
    print_test("Минимум на хвосте массива [9,12]", brute_min(arr, 9, 12), h_rmq.query(9, 12));

    auto sum_op = [](int a, int b) { return a + b; };
    HybridRMQ<int, decltype(sum_op)> h_rsq(arr, sum_op, 0);
    print_test("Сумма на отрезке [3,7] (проверка универсальности)", brute_sum(arr, 3, 7), h_rsq.query(3, 7));
}

vector<int> generate_array(int n) {
    vector<int> arr(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}


int main() {

   ofstream out("data.txt");

    vector<int> sizes = {2000, 5000, 8000, 11000, 14000, 17000, 20000};

    for (int n : sizes) {
        auto arr = generate_array(n);

        out << n << " elements:\n";

        // RSQ1D
        RSQ1D<int> rsq(arr);
        rsq.reset_query_ops();
        int l = rand() % n;
        int r = l + rand() % (n - l);
        rsq.query(l, r);
        out << "  RSQ1D (Prefix Sum) - build: " << rsq.get_build_ops() << ", query: " << rsq.get_query_ops() << "\n";

        // RSQ2D
        vector<vector<int>> mat(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = rand() % 100;
            }
        }
        RSQ2D<int> rsq2d(mat);
        rsq2d.reset_query_ops();
        int x1 = rand() % n;
        int y1 = rand() % n;
        int x2 = x1 + rand() % (n - x1);
        int y2 = y1 + rand() % (n - y1);
        rsq2d.query(x1, y1, x2, y2);
        out << "  RSQ2D (2D Prefix Sum) - build: " << rsq2d.get_build_ops() << ", query: " << rsq2d.get_query_ops() << "\n";

        // RMQ1D
        RMQ1D<int> rmq(arr);
        rmq.reset_query_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        rmq.query(l, r);
        out << "  RMQ1D (Naive O(n^2) table) - build: " << rmq.get_build_ops() << ", query: " << rmq.get_query_ops() << "\n";

        // Fenwick
        FenwickTree<int> fw(arr);
        fw.reset_query_ops();
        fw.reset_update_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        fw.query(l, r);
        fw.update(rand() % n, rand() % 100);
        out << "  Fenwick Tree - build: " << fw.get_build_ops() << ", query: " << fw.get_query_ops() << ", update: " << fw.get_update_ops() << "\n";

        // SegmentTree
        auto sum_op = [](int a, int b) { return a + b; };
        SegmentTree<int, decltype(sum_op)> seg(arr, sum_op, 0);
        seg.reset_query_ops();
        seg.reset_update_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        seg.query(l, r);
        seg.update(rand() % n, rand() % 100);
        out << "  Segment Tree (sum) - build: " << seg.get_build_ops() << ", query: " << seg.get_query_ops() << ", update: " << seg.get_update_ops() << "\n";

        // SparseTable
        auto min_op = [](int a, int b) { return min(a, b); };
        SparseTable<int, decltype(min_op)> st(arr, min_op, numeric_limits<int>::max());
        st.reset_query_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        st.query(l, r);
        out << "  Sparse Table (RMQ) - build: " << st.get_build_ops() << ", query: " << st.get_query_ops() << "\n";

        // SqrtDecomp
        SqrtDecomposition<int, decltype(sum_op)> sqrtd(arr, sum_op, 0);
        sqrtd.reset_query_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        sqrtd.query(l, r);
        out << "  Sqrt Decomposition (sum) - build: " << sqrtd.get_build_ops() << ", query: " << sqrtd.get_query_ops() << "\n";

        // HybridRMQ
        HybridRMQ<int, decltype(min_op)> hrmq(arr, min_op, numeric_limits<int>::max());
        hrmq.reset_query_ops();
        l = rand() % n;
        r = l + rand() % (n - l);
        hrmq.query(l, r);
        out << "  Hybrid RMQ - build: " << hrmq.get_build_ops() << ", query: " << hrmq.get_query_ops() << "\n";

        out << "\n";
    }
    out.close();




    test_rsq1d();
    test_rsq2d();
    test_rmq1d();
    test_sqrt_decomp();
    test_segment_tree();
    test_fenwick();
    test_sparse_table();
    test_hybrid_rmq();

    cout << "\nВсе тесты завершены.\n";
    return 0;
}