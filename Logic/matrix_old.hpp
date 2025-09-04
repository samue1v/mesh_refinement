#ifndef MATRIX_OLD_H
#define MATRIX_OLD_H
#include <iostream>
#include <math.h>
#include <vector>
#include <stdexcept>

template <class T,int m, int n>
class Matrix{
    public:
        Matrix();
        Matrix(T i);
        Matrix(std::vector<T>&& v);
        Matrix(Matrix<T,m,n> && matrix);
        Matrix(const std::vector<T>& v) = delete;
        Matrix(const Matrix<T,m,n> &) = delete;


        Matrix& operator =(const Matrix& other){
            for(int i = 0;i<n*m;i++){
                __matrix[i] = other(i);
            }
            return *this;
        }

        Matrix& operator =(Matrix && v){
            __matrix = v.__matrix;
            return *this;
        }

        T operator () (int i,int j) const {
            return __matrix[i*n+j];
        }

        T& operator ()(int i,int j){
            return __matrix[i*n+j];
        }

        T operator () (int i) const {
            return __matrix[i];
        }

        T& operator ()(int i){
            return __matrix[i];
        }

        Matrix operator +(Matrix other){
            std::vector<T> v;
            for(int i = 0;i< n*m;i++){
                v.push_back(__matrix[i] + other(i));
            }
            return Matrix(std::move(v));
        }

        Matrix operator -(Matrix other){
            std::vector<T> v;
            for(int i = 0;i< n*m;i++){
                v.push_back(__matrix[i] - other(i));
            }
            return Matrix(std::move(v));
        }


        template <int l,int k>
        Matrix<T,m,k> operator *(const Matrix<T,l,k> & other){
            if(n != l){
                throw std::runtime_error("Trying to multiply uncompatible matrixes.\n");
            }
            std::vector<T> nm(m*k,0);
            for(int i =0;i<m;i++){
                for(int j = 0;j<k;j++){
                    T sum = 0;
                    for(int s = 0;s<n;s++){
                        sum = sum + (__matrix[i*n+s]*other(s,j));
                    }
                    std::abs(sum) < 10e-8 ? nm[i*k+j] = 0 : nm[i*k+j] = sum;
                }
            }
            return Matrix<T,m,k>(std::move(nm));
        }

        Matrix operator/(T f){
            std::vector<T> v;
            for(auto i : __matrix){
                auto res = i/f;
                if(std::abs(res) < 10e-8){
                    v.push_back(0);
                }
                else{
                    v.push_back(i/f);
                }

            }
            return Matrix<T,m,n>(std::move(v));
        }

        Matrix operator*(T f){
            std::vector<T> v;
            for(auto i : __matrix){
                v.push_back(i*f);
            }
            return Matrix<T,m,n>(std::move(v));
        }

        friend std::ostream& operator<<(std::ostream &out,const Matrix& ma) {
            int line,col;
            for(int i = 0;i<n*m;i++){
                if(i%n == 0){
                    out << "\n";
                }
                out<<ma(i)<<" ";
            }
            out<<"\n";
            return out;
        }

        //Methods signature


        Matrix<T,n,m> transpose () const ;

        Matrix<T,m,1> getIthColumn(int i) const;

        Matrix<T,m,1> getIthColumnZeroAbove(int i) const;

        Matrix<T,m,n> completeDiagonal (int i) const;

        Matrix inverse  () const ;

        double det () const;

        Matrix cof () const;

        int countLines () const;
         
        int countCols () const;

        int size () const;

        double norm () const;

        bool isDiagonal() const;

        

        Matrix<T,m,n> normalize () const;

        Matrix<T,m,n> scale () const;

        //Matrix<T,m,n> map (double (*func) (double)) const;

    private:
        std::vector<T> __matrix;
        
    
};

template <class T,int m, int n>
inline Matrix<T,m,n>::Matrix(){
    __matrix.reserve(n*m);
    __matrix.resize(m*n);
    int l,c;
    for(int i = 0;i<m*n;i++){
        l = i/n;
        c = i%n;
        __matrix[l*n+c] = l==c ? 1 : 0;
    }
}

template <class T,int m, int n>
inline Matrix<T,m,n>::Matrix(T i){
    __matrix.reserve(n*m);
    __matrix.resize(m*n);
    int l,c;
    std::fill(__matrix.begin(),__matrix.end(),i);
}

template <class T,int m,int n>
inline Matrix<T,m,n>::Matrix(std::vector<T>&& v){
    __matrix = v;
}

template <class T,int m,int n>
inline Matrix<T,m,n>::Matrix(Matrix<T,m,n>&& ma){
    __matrix = ma.__matrix;
}

template <class T,int m, int n>
inline int Matrix<T,m,n>::countLines() const{
    return m;
}

template <class T,int m, int n>
inline int Matrix<T,m,n>::countCols() const{
    return n;
}

template <class T,int m, int n>
inline int Matrix<T,m,n>::size() const{
    return __matrix.size();
}

template <class T,int m, int n>
inline bool Matrix<T,m,n>::isDiagonal() const{
    int line,column;
    double sum = 0;
    for(int i = 0;i<m*n;i++){
        line = i/n;
        column = i%n;
        sum = line!=column ? sum + std::abs(__matrix[i]) : sum+0;
    }
    std::cout<<sum<<std::endl;
    if(sum<1e-10){
        return true;
    }
    return false;
}

template <class T,int m, int n>
inline double Matrix<T,m,n>::norm () const{
    double sum = 0;
    for(int i = 0;i<m*n;i++){
        sum+=__matrix[i]*__matrix[i];
    }
    return sqrt(sum);
}

template <class T,int m, int n>
inline Matrix<T,m,n> Matrix<T,m,n>::normalize () const{
    return this/this->norm();
}

template <class T,int m, int n>
inline Matrix<T,m,n> Matrix<T,m,n>::scale () const{
    Matrix<T,m,n> scaled;
    scaled = *this;
    T max = -INFINITY;
    for(int i=0;i<m*n;i++){
        max = std::abs(__matrix[i]) > max ? std::abs(__matrix[i]) : max;
    }
    return scaled/max;

}

template <class T,int m, int n>
inline Matrix<T,m,n> Matrix<T,m,n>::completeDiagonal(int i) const{
    Matrix<T,m,n> temp;
    temp = *this;
    for(int j=0;j<i;j++){
        temp(j*n+j) = 1;
    }
    return temp;
}

template <class T,int m,int n>
inline Matrix<T,n,m> Matrix<T,m,n>::transpose() const {

    if(m==1 || n==1){
        std::vector<T> v;
        v.reserve(n*m);
        v.resize(n*m);
        v = __matrix;
        return Matrix<T,n,m>(std::move(v));
    }

    
    std::vector<T> v;
    v.reserve(n*m);
    v.resize(n*m);
    int line,col;
    for(int i = 0; i < n*m;i++){
        line = i/n;
        col = (i)%n;
        v[(col*m+line)] = __matrix[line*n+col];
    }
    return Matrix<T,n,m>(std::move(v));
}

 template<class T,int m,int n>
 inline Matrix<T,m,1> Matrix<T,m,n>::getIthColumn(int i) const{
    Matrix<T,m,1> columnMatrix;
    int count=0;
    for(int j = i;j<m*n;j+=n){
        columnMatrix(count) = __matrix[j];
        count++;
    }
    return columnMatrix;
 }

  template<class T,int m,int n>
 inline Matrix<T,m,1> Matrix<T,m,n>::getIthColumnZeroAbove(int i) const{
    Matrix<T,m,1> columnMatrix(0);
    int count=i;
    for(int j = i+n*i;j<m*n;j+=n){
        columnMatrix(count) = __matrix[j];
        count++;
    }
    return columnMatrix;
 }


template <class T,int n,int m>
inline double Matrix<T,n,m>::det() const{
    if(m!=n){
        throw std::runtime_error("This Matrix does not support determinant.\n");
    }
    double result = 0;
    int sign = 1;
    for(int i = 0; i < m; i++) {

        //Submatrix construction
        Matrix<double,n-1,m-1> subm(std::vector<double>((n-1)*(m-1),0));
        for(int l = 1; l < m; l++) {
            int z = 0;
            for(int k = 0; k < n; k++) {
                if(k != i) {
                    subm(l-1,z) = __matrix[l*m+k];
                    z++;
                }
            }
        }

        //recursive call
        result = result + sign * __matrix[i] * subm.det();
        sign = -sign;
    }

    return result;
}

template <class T,int n,int m>
inline Matrix<T,n,m> Matrix<T,n,m>::cof() const{
    std::vector<double>v(m*n,0);
    int line,column,z;
    for(int i=0;i<m*n;i++){
        line = i/m;
        column = i%n;
        Matrix<double,m-1,n-1> sub;
        z=0;
        //Constructing submatrix
        for(int j=0;j<m*n;j++){
            if(j/m!=line && j%n!=column){
                sub(z++) = (__matrix[j]);
            }
        }

        v[i] = sub.det()*pow(-1,line+1+column+1);
    }
    return Matrix<double,m,n>(std::move(v));
}

template <class T,int n,int m>
inline Matrix<T,n,m> Matrix<T,n,m>::inverse() const{
    if(m!=n){
        throw std::runtime_error("This Matrix does not support determinant.\n");
    }
    double d = this->det();
    if (std::abs(d) < 10e-8){
        throw std::runtime_error("Determinant 0 on inverse");
    }
    Matrix<double,m,n> t = this->transpose();
    Matrix<double,m,n> c = t.cof();
    
    return c/d;
}


/*

template specialization for determinants and cofactor for double point numbers

*/
template<>
inline double Matrix<double,0,0>::det() const{
    return 0;
}

template<>
inline double Matrix<double,1,1>::det() const{
    return __matrix[0];
}

template<>
inline double Matrix<double,2,2>::det() const{
    return __matrix[0] * __matrix[3] - __matrix[1] * __matrix[2];
}

template <>
inline double Matrix<double,3,3>::det() const{

    return (__matrix[0]*__matrix[4]*__matrix[8] + 
            __matrix[1]*__matrix[5]*__matrix[6] +
            __matrix[2]*__matrix[3]*__matrix[7] -
            __matrix[2]*__matrix[4]*__matrix[6] -
            __matrix[0]*__matrix[5]*__matrix[7] -
            __matrix[1]*__matrix[3]*__matrix[8]
            );

}

template <>
inline double Matrix<double,4,4>::det() const{

    double result = 0;
    int sign = 1;
    for(int i = 0; i < 4; i++) {

        //Submatrix construction
        Matrix<double,3,3> subm(std::vector<double>(9,0));
        for(int l = 1; l < 4; l++) {
            int z = 0;
            for(int k = 0; k < 4; k++) {
                if(k != i) {
                    subm(l-1,z) = __matrix[l*4+k];
                    z++;
                }
            }
        }

        //recursive call
        result = result + sign * __matrix[i] * subm.det();
        sign = -sign;
    }

    return result;
}

template <>
inline Matrix<double,0,0> Matrix<double,0,0>::cof() const{
    return Matrix<double,0,0>();
}

template <>
inline Matrix<double,1,1> Matrix<double,1,1>::cof() const{
    return Matrix<double,1,1>(std::vector<double>(__matrix));
}

template <>
inline Matrix<double,2,2> Matrix<double,2,2>::cof() const{
    
    std::vector<double> v({__matrix[3], -(__matrix[2]), -(__matrix[1]), __matrix[0]});
    return Matrix<double,2,2>(std::move(v));
}

template <>
inline Matrix<double,3,3> Matrix<double,3,3>::cof() const{
    std::vector<double>v(9,0);
    int line,column,z;
    for(int i=0;i<9;i++){
        line = i/3;
        column = i%3;
        Matrix<double,2,2> sub;
        z=0;
        //Constructing submatrix
        for(int j=0;j<9;j++){
            if(j/3!=line && j%3!=column){
                sub(z++) = (__matrix[j]);
            }
        }

        v[i] = sub.det()*pow(-1,line+column);
    }
    return Matrix<double,3,3>(std::move(v));
}

template <>
inline  Matrix<double,4,4> Matrix<double,4,4>::cof() const{
    std::vector<double>v(16,0);
    int line,column,z;
    for(int i=0;i<16;i++){
        line = i/4;
        column = i%4;
        Matrix<double,3,3> sub;
        z=0;
        //Constructing submatrix
        for(int j=0;j<16;j++){
            if(j/4!=line && j%4!=column){
                sub(z++) = (__matrix[j]);
            }
        }

        v[i] = sub.det()*pow(-1,line+1+column+1);
    }
    return Matrix<double,4,4>(std::move(v));
}

template<>
inline Matrix<double,3,3> Matrix<double,3,3>::inverse() const{
    Matrix<double,3,3> t = this->transpose();
    Matrix<double,3,3> c = t.cof();
    double d = this->det();
    return c/d;
}

template<>
inline Matrix<double,4,4> Matrix<double,4,4>::inverse() const{
    double d = this->det();
    if (std::abs(d) < 10e-8){
        throw std::runtime_error("Determinant 0 on inverse");
    }
    Matrix<double,4,4> t = this->transpose();
    Matrix<double,4,4> c = t.cof();
    
    return c/d;
}

//Useful functions that use Matrix but are not part of it

template<class T,int m>
 Matrix<T,m,1> getCanonicalEi(int i){
    Matrix<T,m,1> ei(0);
    ei(i) = 1;
    return ei;
 }

template<class T,int m,int n>
 Matrix<T,m,n> getIdentity(){
    Matrix<T,m,n> i;
    return i;
 }

 template<class T,int m,int n>
 Matrix<T,m,n> getIdentitySubMatrix(int i){
    Matrix<T,m,n> sub(0);
    for(int j = i*n+i;j<m*n;j+=n+1){
        sub(j) = 1;
    }
    return sub;
 }

//  template<class T,int m,int n>
//  inline Matrix<T,m,n> Matrix<T,m,n>::map(double (*func) (double))const {
//     std::vector<double> v = __matrix;
//     //v.reserve(m*n);
//     //v.resize(m*n);
//     for(int i=0;i<m*n;i++){
//         v[i] = func(v[i]);
//     }
//     return Matrix<T,m,n>(std::move(v));
//  }

using Matrix3d = Matrix<double,3,3>;
using Matrix4d = Matrix<double,4,4>;
using RowMatrix3d = Matrix<double,1,3>;
using ColMatrix3d = Matrix<double,3,1>;

#endif