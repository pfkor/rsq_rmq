#include "./include/basic.h"

#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

void printSeparator() {
    cout << "\n========================================\n";
}

void testRSQ1D() {
    cout << "\n=== Testing RSQ1D (Range Sum Query 1D) ===\n";
    
    size_t n;
    cout << "Enter array size: ";
    cin >> n;
    
    vector<int> arr(n);
    cout << "Enter " << n << " integers: ";
    for (size_t i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    
    RSQ1D<int> rsq(arr);
    
    cout << "\nArray: ";
    for (size_t i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << "\n";
    
    int choice;
    do {
        cout << "\n1. Query sum on range [l, r]\n";
        cout << "2. Change array (create new RSQ)\n";
        cout << "3. Exit to main menu\n";
        cout << "Choose: ";
        cin >> choice;
        
        if (choice == 1) {
            size_t l, r;
            cout << "Enter l and r (0-indexed, l <= r, r < " << n << "): ";
            cin >> l >> r;
            
            int sum = rsq.query(l, r);
            cout << "Sum on [" << l << ", " << r << "] = " << sum << "\n";
        }
        else if (choice == 2) {
            cout << "Enter new size: ";
            cin >> n;
            arr.resize(n);
            cout << "Enter " << n << " integers: ";
            for (size_t i = 0; i < n; ++i) {
                cin >> arr[i];
            }
            RSQ1D<int> newRsq(arr);
            rsq = newRsq;
            cout << "Array updated!\n";
        }
        
    } while (choice != 3);
}

void testRSQ2D() {
    cout << "\n=== Testing RSQ2D (Range Sum Query 2D) ===\n";
    
    size_t n, m;
    cout << "Enter matrix dimensions (rows cols): ";
    cin >> n >> m;
    
    vector<vector<int>> matrix(n, vector<int>(m));
    cout << "Enter matrix " << n << " x " << m << ":\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            cin >> matrix[i][j];
        }
    }
    
    RSQ2D<int> rsq2d(matrix);
    
    cout << "\nMatrix:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
    
    int choice;
    do {
        cout << "\n1. Query sum on rectangle [(x1,y1), (x2,y2)]\n";
        cout << "2. Exit to main menu\n";
        cout << "Choose: ";
        cin >> choice;
        
        if (choice == 1) {
            size_t x1, y1, x2, y2;
            cout << "Enter x1, y1, x2, y2 (0-indexed, x1<=x2, y1<=y2): ";
            cin >> x1 >> y1 >> x2 >> y2;
            
            if (x1 <= x2 && y1 <= y2 && x2 < n && y2 < m) {
                int sum = rsq2d.query(x1, y1, x2, y2);
                cout << "Sum on rectangle [(" << x1 << "," << y1 << ") - (" 
                     << x2 << "," << y2 << ")] = " << sum << "\n";
            } else {
                cout << "Invalid coordinates!\n";
            }
        }
        
    } while (choice != 2);
}

void testRMQ1D() {
    cout << "\n=== Testing RMQ1D (Range Minimum Query 1D) ===\n";
    
    size_t n;
    cout << "Enter array size: ";
    cin >> n;
    
    vector<int> arr(n);
    cout << "Enter " << n << " integers: ";
    for (size_t i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    
    RMQ1D<int> rmq(arr);
    
    cout << "\nArray: ";
    for (size_t i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << "\n";
    
    int choice;
    do {
        cout << "\n1. Query minimum on range [l, r]\n";
        cout << "2. Exit to main menu\n";
        cout << "Choose: ";
        cin >> choice;
        
        if (choice == 1) {
            size_t l, r;
            cout << "Enter l and r (0-indexed, l <= r, r < " << n << "): ";
            cin >> l >> r;
            
            int min_val = rmq.query(l, r);
            cout << "Minimum on [" << l << ", " << r << "] = " << min_val << "\n";
        }
        
    } while (choice != 2);
}

int main() {
    cout << "========================================\n";
    cout << "   Range Query Data Structures Test\n";
    cout << "========================================\n";
    
    int choice;
    do {
        printSeparator();
        cout << "1. Test RSQ1D (Range Sum Query 1D)\n";
        cout << "2. Test RSQ2D (Range Sum Query 2D)\n";
        cout << "3. Test RMQ1D (Range Minimum Query 1D)\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                testRSQ1D();
                break;
            case 2:
                testRSQ2D();
                break;
            case 3:
                testRMQ1D();
                break;
            case 4:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
        
    } while (choice != 4);
    
    return 0;
}