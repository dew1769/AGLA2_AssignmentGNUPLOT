//Roman Pakhomov
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>

using namespace std;

class Matrix {
public:
    int rows = 0;
    int columns = 0;
    double **arr{};
    Matrix() = default;
    Matrix(int row, int column);
    friend istream& operator>>(istream& f1, Matrix& M);
    friend ostream& operator<<(ostream& f2, const Matrix& M);
    Matrix operator=(const Matrix& M);
    Matrix operator+(const Matrix& second);
    Matrix operator-(const Matrix& second);
    Matrix operator*(const Matrix& second);
    Matrix transpose();
};
//create matrix
Matrix::Matrix(int row, int column) {
    rows = row;
    columns = column;
    arr = new double*[row];
    for (int i = 0; i < row; i++) {
        arr[i] = new double[column];
    }
}

// Overloaded operator for reading matrix from input
istream& operator>>(istream& f1, Matrix& M) {
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            f1 >> M.arr[i][j];
        }
    }
    return f1;
}

// Overloaded operator for printing matrix
ostream& operator<<(ostream& f2, const Matrix& M) {
    f2 << fixed;
    f2.precision(4);
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.columns; j++) {
            f2 << M.arr[i][j] << " ";
        }
        f2 << endl;
    }
    return f2;
}

//Overloaded operator for assign one matrix to another
Matrix Matrix::operator=(const Matrix& M) {
    rows = M.rows;
    columns = M.columns;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            this->arr[i][j] = M.arr[i][j];
        }
    }
    return (*this);
}

// Overloaded operator for matrix addition
Matrix Matrix::operator+(const Matrix& second) {
    if (rows!=second.rows || columns!=second.columns) {
        cout << "Error: the dimensional problem occurred" << endl;
        return Matrix(0, 0);
    }
    Matrix addition(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            addition.arr[i][j] = arr[i][j] + second.arr[i][j];
        }
    }
    return addition;
}

// Overloaded operator for matrix subtraction
Matrix Matrix::operator-(const Matrix& second) {
    if (rows!=second.rows || columns!=second.columns) {
        cout << "Error: the dimensional problem occurred" << endl;
        return Matrix(0, 0);
    }
    Matrix sub(rows, columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            sub.arr[i][j] = arr[i][j] - second.arr[i][j];
        }
    }
    return sub;
}

// Overloaded operator for matrix multiplication
Matrix Matrix::operator*(const Matrix& second) {
    if (columns!=second.rows) {
        cout << "Error: the dimensional problem occurred" << endl;
        return Matrix(0, 0);
    }
    Matrix product(rows, second.columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < second.columns; j++) {
            product.arr[i][j] = 0;
            for (int temp = 0; temp<columns; temp++) {
                product.arr[i][j] += arr[i][temp]*second.arr[temp][j];
            }
        }
    }
    return product;
}

//Transposed matrix
Matrix Matrix::transpose() {
    Matrix transposed(columns, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            transposed.arr[j][i] = arr[i][j];
        }
    }
    return transposed;
}

class SquareMatrix : public Matrix {
public:
    SquareMatrix() {};
    SquareMatrix(int n) {
        rows = n;
        columns = n;
        arr = new double*[n];
        for (int i = 0; i < n; i++) {
            arr[i] = new double[n];
        }
    }
    friend istream& operator>>(istream& f1, SquareMatrix& M) {
        for (int i = 0; i < M.rows; i++) {
            for (int j = 0; j < M.columns; j++) {
                f1 >> M.arr[i][j];
            }
        }
        return f1;
    }
    friend ostream& operator<<(ostream& f2, const SquareMatrix& M) {
        f2 << fixed;
        f2.precision(4);
        for (int i = 0; i < M.rows; i++) {
            for (int j = 0; j < M.columns; j++) {
                f2 << M.arr[i][j] << " ";
            }
            f2 << endl;
        }
        return f2;
    }
    SquareMatrix operator+(const SquareMatrix &x) {
        Matrix addition = *((Matrix*)this) + *((Matrix*)&x);

        return *(SquareMatrix*)&addition;
    }
    SquareMatrix operator-(const SquareMatrix &x) {
        Matrix subtraction = *((Matrix*)this) - *((Matrix*)&x);

        return *(SquareMatrix*)&subtraction;
    }
    SquareMatrix operator*(const SquareMatrix &x) {
        Matrix multiplication = *((Matrix*)this) * *((Matrix*)&x);

        return *(SquareMatrix*)&multiplication;
    }
    SquareMatrix transpose() {
        Matrix transposed = ((Matrix*)this)->transpose();

        return *(SquareMatrix*)&transposed;
    }
};


class IdentityMatrix : public SquareMatrix {
public:
    IdentityMatrix(SquareMatrix M) {
        rows = M.rows;
        columns = M.columns;
        arr = new double*[M.rows];
        for (int i = 0; i < M.rows; i++) {
            arr[i] = new double[M.rows];
        }
        for (int i = 0; i < M.rows; i++) {
            for (int j = 0; j < M.columns; j++) {
                arr[i][j] = 0.0;
            }
        }
        for (int i = 0; i < M.rows; i++) {
            arr[i][i] = 1.0;
        }
    }
};

class EliminationMatrix : public SquareMatrix {
public:
    EliminationMatrix(SquareMatrix M, int p1, int p2) {
        rows = M.rows;
        columns = M.columns;
        arr = new double *[M.rows];
        for (int i = 0; i < M.rows; i++) {
            arr[i] = new double[M.rows];
        }
        for (int i = 0; i < M.rows; i++) {
            for (int j = 0; j < M.columns; j++) {
                if (p1 == i && p2 == j) {
                    arr[i][j] = -(M.arr[i][j]/M.arr[j][j]);
                } else if (i==j){
                    arr[i][j] = 1.0;
                } else {
                    arr[i][j] = 0.0;
                }
            }
        }
    }
};

class PermutationMatrix : public SquareMatrix {
public:
    PermutationMatrix(int n, int p1, int p2) {
        rows = n;
        columns = n;
        arr = new double *[n];
        for (int i = 0; i < n; i++) {
            arr[i] = new double[n];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                arr[i][j] = 0.0;
            }
        }
        for (int i = 0; i < n; i++) {
            arr[i][i] = 1.0;
        }
        swap(arr[p1], arr[p2]);
    }
};


void inverse(SquareMatrix M, SquareMatrix I) {
    cout << fixed;
    cout.precision(4);

    for (int i = 0; i < M.rows; i++) {
        double max = M.arr[i][i];
        int pos = i;
        for (int j = i+1; j < M.rows; j++) {
            if (abs(M.arr[j][i]) > abs(max)) {
                max = M.arr[j][i];
                pos = j;
            }
        }
        if (pos != i) {
            PermutationMatrix P(M.rows, i, pos);
            M = P*M;
            I = P*I;
        }
        for (int j = i+1; j < M.rows; j++) {
            if (M.arr[j][i] == 0) {
                continue;
            }
            EliminationMatrix E(M, j, i);
            M = E*M;
            I = E*I;
        }
    }

    for (int i = M.rows - 1; i > 0; i--) {
        if (M.arr[i][i] == 0)
            continue;
        for (int j = i-1; j >= 0; j--) {
            if (M.arr[j][j] == 0 || M.arr[j][i] == 0)
                continue;
            EliminationMatrix E(M, j, i);
            M = E*M;
            I = E*I;
        }
    }

    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.rows; j++) {
            I.arr[i][j]/=M.arr[i][i];
        }
        M.arr[i][i]=1;
    }
    cout << "(A_T*A)^-1:" << endl << I;
}

#ifdef WIN32
#define plot "C:\\gnuplot\\bin\\gnuplot -persist"
#else
#define plot "gnuplot -persist"
#endif

int main() {

    FILE* pipe = _popen(plot, "w");

    int n, m = 0;
    cin >> m;
    Matrix input(m, 2);
    cin >> input;
    cin >> n;
    Matrix A(m, n+1);
    Matrix b(m, 1);
    for (int i = 0; i < m; i++) {
        A.arr[i][0] = 1;
        for (int j = 0; j <n; j++) {
            A.arr[i][j+1] = pow(input.arr[i][0], j+1);
        }
        b.arr[i][0] = input.arr[i][1];
    }
    cout << "A:" << endl << A;

    Matrix As = A.transpose()*A;
    SquareMatrix Anew = *(SquareMatrix*)&As;
    cout << "A_T*A:" << endl << Anew;

    SquareMatrix I = IdentityMatrix(Anew.rows);
    inverse(Anew, I);

    Matrix Atb = A.transpose()*b;

    cout << "A_T*b:" << endl << Atb;

    Matrix f = *((Matrix*)&I)*Atb;

    cout << "x~:" << endl << f;

    fprintf(pipe, "plot [-5 : 10] [0 : 30] %lf*x**3 + %lf*x**2 + %lf*x**1 + %lf*x**0 , '-' using 1:2 with points\n", f.arr[3][0], f.arr[2][0], f.arr[1][0], f.arr[0][0]);
    fprintf(pipe, "plot '-' w p ls 1, '-' w p ls 2, '-' w p ls 3, '-' w p ls 4, '-' w p ls 5, '-' w p ls 6, '-' w p ls 7, '-' w p ls 8, '-' w p ls 9, '-' w p ls 10\n");
    for (int i = 0; i < m; i++) {
        fprintf(pipe, "%f\t%f\n", input.arr[i][0], b.arr[i][0]);
    }

    _pclose(pipe);

    return 0;
}