#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <concepts>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>
#include <cmath>
#include <iostream>
#include <glm/vec3.hpp>
#include "Util.hpp"



using uint = unsigned int;

template<class T>
concept Arithmetic =  std::is_integral<T>::value || std::is_floating_point<T>::value;

template <Arithmetic U>
class Matrix{
public:
  Matrix(uint rows = 0,uint cols = 0);
  Matrix(uint rows,uint cols, std::initializer_list<U> l);
  Matrix(uint rows,uint cols, std::vector<U> l);
  Matrix(glm::vec3 v,uint axis = 0);
  

  //operators

  Matrix<U> operator()(uint ele){
    if(ele >= _cols){
      throw std::out_of_range("Matrix acess out of range.");
    }
    return Matrix<U>(_rows,1,_matrix[ele]);
  }

  std::vector<U> & operator[](uint ele){
    if(ele >= _cols){
      throw std::out_of_range("Matrix acess out of range.");
    }
    return _matrix[ele];
  }

  Matrix<U> operator*(Matrix<U> other){
    if(_cols != other.rows()){
      throw std::logic_error("Atempt to multiply uncompatible dimension matrices.");
    }
    Matrix<U> r(_rows,other.cols());
    int i,j,k;
    U sum;
    for(i = 0; i < other.cols(); i++){
      for(j = 0; j < _rows; j++){
        sum = 0;
        for(k = 0; k < _cols; k++){
          sum += other[i][k]*_matrix[k][j];
        }
        r[i][j] = sum;
      }
    }
    return r;
  }

  glm::vec3 operator*(glm::vec3 other){
    if(_cols != 3){
      throw std::logic_error("Atempt to multiply uncompatible dimension matrices and Vec3.");
    }
    glm::vec3 r;
    Matrix<U> otherMatrix(other);
    for(int i = 0; i < 3; i++){
      Matrix<U> row = this->subRow(i).transpose();
      r[i] = (row*otherMatrix)[0][0];
    }
    return r;
  }

  Matrix<U> operator+(Matrix<U> other){
    if(_rows != other.rows() || _cols != other.cols()){
      throw std::logic_error("Atempt to add uncompatible dimension matrices.");
    }
    Matrix<U> r(_rows,_cols);
    for(int i = 0; i < _cols; i++){
      for(int j = 0; j < _rows; j++){
        r[i][j] = _matrix[i][j] + other[i][j];
      }
    }
    return r;
  }
  
  Matrix<U> operator-(Matrix<U> other){
    if(_rows != other.rows() || _cols != other.cols()){
      throw std::logic_error("Atempt to subtract uncompatible dimension matrices.");
    }
    Matrix<U> r(_rows,_cols);
    for(int i = 0; i < _cols; i++){
      for(int j = 0; j < _rows; j++){
        r[i][j] = _matrix[i][j] - other[i][j];
      }
    }
    return r;
  }

  Matrix<U> operator*(U val){
    Matrix<U> r(_rows,_cols);
    for(int i = 0; i < _cols; i++){
      for(int j = 0; j < _rows; j++){
        r[i][j] = _matrix[i][j]*val;
      }
    }
    return r;
  }

  Matrix<U> operator/(U val){
    Matrix<U> r(_rows,_cols);
    for(int i = 0; i < _cols; i++){
      for(int j = 0; j < _rows; j++){
        r[i][j] = _matrix[i][j]/val;
      }
    }
    return r;
  }

  friend Matrix<U> operator*(U val, Matrix<U> A) { 
    Matrix<U> r(A.rows(),A.cols());
    for(int i = 0; i < A.cols(); i++){
      for(int j = 0; j < A.rows(); j++){
        r[i][j] = A[i][j]*val;
      }
    }
    return r;
  }


  //

  //Public Methods

  uint rows() const;
  uint cols() const;
  void swapRows(int r1,int r2,int colIni, int colEnd);
  void swapCols(int c1,int r2,int rowIni, int rowEnd);
  Matrix<U> inverse();
  Matrix<U> transpose();
  U euclidianNorm();
  U dot(Matrix<U> A);
  U det();
  glm::vec3 toVec3();
  
  


  //Static methods
  static Matrix<U> eye(int m);
  static Matrix<U> eye(int m, int n);
  static std::vector<Matrix<U>> LUPdecomp(Matrix<U> A);
  static std::vector<Matrix<U>> QRdecomp(Matrix<U> A);
  static bool checkColOrthogonality(Matrix<U> A);
  static bool checkColUnitary(Matrix<U> A);
  static std::vector<Matrix<U>> symmetricSVD(Matrix<U> A);
  static Matrix<U> columnMean(Matrix<U> A,bool stack = true);
  static Matrix<U> covariance(Matrix<U> A);
  static Matrix<U> stackVectors(std::vector<Matrix<U>> A,uint axis = 0);


  
  //
private:
  //Private static methods
  static void assertSquare(Matrix<U> & A);
  static void assertVector(Matrix<U> & A);
  static Matrix<U> backwardSolver(Matrix<U> upperTriangular,Matrix<U> b);
  static Matrix<U> forwardSolver(Matrix<U> lowerTriangular,Matrix<U> b);
  static Matrix<U> fillBefore(Matrix<U> A, uint n);
  static std::vector<Matrix<U>> bidiagonalize(Matrix<U> A);
  static std::vector<Matrix<U>> QRiteration(Matrix<U> A);
  static std::vector<Matrix<U>> hhZero1(Matrix<U> A);//column hh
  static std::vector<Matrix<U>> hhZero2(Matrix<U> A,uint i, uint j, bool row, int dim);//row hh
  static Matrix<U> roundMatrix(Matrix<U> A);
  //

  //Private methods
  int pivot(int n = 0);
  void sumRows(int r1,int r2,int colIni ,int colEnd , U factor = 1);
  Matrix<U> subCol(uint col,uint rowInit = 0);
  Matrix<U> subRow(uint row,uint colInit = 0);
  Matrix<U> subMatrix(uint rowIni,uint rowEnd, uint colIni, uint colEnd);
  void replaceSubMatrix(Matrix<U> A, uint rowIni,uint rowEnd, uint colIni, uint colEnd);
  bool isSymmetric();
  bool isDiagonal(int tol = 15);
  //
  //Private members
private: 
  uint _rows, _cols;
  std::vector<std::vector<U>> _matrix;
  //
};

//inline bool almostZero(float a){
//  if(std::abs(a) < 10e-15){
//      return true;
//  }
//  return false;
//}

template <Arithmetic U>
inline void assertDivisible(U a, U b){
  bool isZero = Misc::Util::almostZero(b);
  if(isZero){
    throw std::domain_error("Division by zero.\n");
  }
}

////////////////////// Constructors ///////////////////////////

template <Arithmetic U>
Matrix<U>::Matrix(uint rows, uint cols){
  _rows = rows;
  _cols = cols;
  _matrix.resize(cols);
  for(int i = 0;i<_matrix.size();i++){
    _matrix[i].resize(rows);
  }
}

template <Arithmetic U>
Matrix<U>::Matrix(uint rows,uint cols, std::initializer_list<U> l){
  _rows = rows;
  _cols = cols;
  _matrix.resize(cols);
  if(l.size() > rows*cols){throw std::out_of_range("Initializer list size is greater than matrix capacity");}
  for(int i = 0;i<_matrix.size();i++){
    _matrix[i].resize(rows);
  }
  auto list_it = l.begin();
  for(int j = 0;j<l.size();j++){
    _matrix[j%cols][j/cols] = *(list_it++);
  }
}

template <Arithmetic U>
Matrix<U>::Matrix(uint rows,uint cols, std::vector<U> l){
  _rows = rows;
  _cols = cols;
  _matrix.resize(cols);
  if(l.size() > rows*cols){throw std::out_of_range("Vector list size is greater than matrix capacity");}
  for(int i = 0;i<_matrix.size();i++){
    _matrix[i].resize(rows);
  }
  auto list_it = l.begin();
  for(int j = 0;j<l.size();j++){
    _matrix[j%cols][j/cols] = *(list_it++);
  }
}


template <Arithmetic U>
Matrix<U>::Matrix(glm::vec3 v, uint axis){
  if(axis == 0){
    _rows = 3;
    _cols = 1;
    _matrix.resize(1);
    _matrix[0].resize(3);
    _matrix[0][0] = v.x;
    _matrix[0][1] = v.y;
    _matrix[0][2] = v.z;
  }
  else{
    _rows = 1;
    _cols = 3;
    _matrix.resize(3);
    _matrix[0].resize(1);
    _matrix[1].resize(1);
    _matrix[2].resize(1);
    _matrix[0][0] = v.x;
    _matrix[1][0] = v.y;
    _matrix[2][0] = v.z;
  }
}

//////////////////////              ///////////////////////////

template <Arithmetic U>
uint Matrix<U>::rows() const{
  return _rows;
}

template <Arithmetic U>
uint Matrix<U>::cols() const{
  return _cols;
}

template <Arithmetic U>
void printMatrix(Matrix<U> m){
  for(int i = 0;i<m.rows();i++){
    for(int j = 0;j<m.cols();j++){
      if(!Misc::Util::almostZero(m[j][i])){
        std::cout<< m[j][i] << " ";
      }
      else{
        std::cout<< 0.0 << " ";
      }
    }
    std::cout<<"\n";
  }
}

template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::LUPdecomp(Matrix<U> A){
  assertSquare(A);
  int dim = A.rows();
  Matrix<U> P = Matrix<U>::eye(dim);
  Matrix<U> L = Matrix<U>::eye(dim);
  Matrix<U> Up= A;
  for(int i = 0;i< dim - 1;i++){
    int pivotRow = Up.pivot(i);
    if(pivotRow != i){Up.swapRows(pivotRow,i,i,Up.cols());P.swapRows(pivotRow,i,0,P.cols());if(i>0){L.swapRows(pivotRow,i,0,i);}}
    for(int j = i+1;j<dim;j++){
      U factor = Up[i][j]/Up[i][i];
      Up.sumRows(i,j,0,dim,-factor);
      L[i][j] = factor;
    }
  }
  std::vector<Matrix<U>> ret(3);
  ret[0]=P;
  ret[1]=L;
  ret[2]=Up;
  return ret;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::eye(int n){
  Matrix<U> id(n,n);
  for(int i = 0;i < n;i++){
    id[i][i] = 1;
  }
  return id;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::eye(int m, int n){
  Matrix<U> id(m,n);
  int k = std::min(m,n);
  for(int i = 0;i < k;i++){
    id[i][i] = 1;
  }
  return id;
}

template <Arithmetic U>
int Matrix<U>::pivot(int n){
  int dim = _rows;
  int maxVal = std::abs(_matrix[n][n]);
  int maxIdx = n;
  for(int i = n + 1 ;i < dim;i++){
    if(std::abs(_matrix[n][i]) > maxVal ){
      maxVal = std::abs(_matrix[n][i]);
      maxIdx = i;
    }
  }
  return maxIdx;
}


template <Arithmetic U>
void Matrix<U>::swapRows(int r1, int r2,int colIni, int colEnd){
  if(r1 > _rows || r2 > _rows){throw std::invalid_argument("Row index out of range.");}
  U temp;
  for(int i = colIni; i<colEnd;i++){
    temp = _matrix[i][r1];
    _matrix[i][r1] = _matrix[i][r2];
    _matrix[i][r2] = temp;
  }
}

template <Arithmetic U>
void Matrix<U>::swapCols(int c1, int c2,int rowIni, int rowEnd){
  if(c1 > _cols || c2 > _cols){throw std::invalid_argument("Column index out of range.");}
  if(rowIni == 0 && rowEnd == _rows){
    std::vector<U> temp = _matrix[c1];
    _matrix[c1] = _matrix[c2];
    _matrix[c2] = temp;
    return;
  }
  U temp;
  for(int i = rowIni; i<rowEnd;i++){
    temp = _matrix[c1][i];
    _matrix[c1][i] = _matrix[c2][i];
    _matrix[c2][i] = temp;
  }

}

template <Arithmetic U>
void Matrix<U>::sumRows(int r1,int r2, int colIni, int colEnd,U factor){
  int dim = _cols;
  for(int i = colIni;i<colEnd;i++){
    _matrix[i][r2] = _matrix[i][r2] + (_matrix[i][r1] * factor);
  }
}

template <Arithmetic U>
Matrix<U> Matrix<U>::backwardSolver(Matrix<U> upperTriangular,Matrix<U> b){
  int dim = upperTriangular.cols();
  std::vector<U> solutionVector(dim);
  assertDivisible<U>(b[0][dim-1],upperTriangular[dim-1][dim-1]);
  solutionVector[dim-1] = b[0][dim-1]/upperTriangular[dim-1][dim-1];
  for(int i = dim - 2;i>=0;i--){
    U sum = 0;
    for(int j = i+1;j<dim;j++){
      sum += upperTriangular[j][i]*solutionVector[j];
    }
    assertDivisible<U>((b[0][i]-sum),upperTriangular[i][i]);
    solutionVector[i] = (b[0][i]-sum)/upperTriangular[i][i];
  }
  return Matrix<U>(dim,1,solutionVector);
}


template <Arithmetic U>
Matrix<U> Matrix<U>::forwardSolver(Matrix<U> lowerTriangular,Matrix<U> b){
  int dim = lowerTriangular.cols();
  std::vector<U> solutionVector(dim);
  assertDivisible<U>(b[0][0],lowerTriangular[0][0]);
  solutionVector[0] = b[0][0]/lowerTriangular[0][0];
  for(int i = 1;i < dim;i++){
    U sum = 0;
    for(int j = 0;j<i;j++){
      sum += lowerTriangular[j][i]*solutionVector[j];
    }
    assertDivisible<U>((b[0][i]-sum),lowerTriangular[i][i]);
    solutionVector[i] = (b[0][i]-sum)/lowerTriangular[i][i];
  }
  return Matrix<U>(dim,1,solutionVector);
}

template <Arithmetic U>
Matrix<U> Matrix<U>::inverse(){
  if(this->isDiagonal(5)){//minimum acceptable tolerance for now
    Matrix<U> res = Matrix<U>::eye(_rows);
    for(int j = 0;j<_cols;j++){
      if(_matrix[j][j] != 0 ){
        res[j][j] = res[j][j]/_matrix[j][j];
      }
      else{
        res[j][j] = 0;
      }
    }
    return res;
  }
  std::vector<Matrix<U>> lup = LUPdecomp(*this);
  Matrix<U> uInverse(_rows,_cols);
  Matrix<U> lInverse(_rows,_cols);
  Matrix<U> id1(_rows,1);
  id1[0][0] = 1;
  for(int i = 1;i<=_cols;i++){
    uInverse[i-1] = Matrix<U>::backwardSolver(lup[2],id1)[0];
    lInverse[i-1] = Matrix<U>::forwardSolver(lup[1],id1)[0];
    id1.swapRows(i-1,i,0,1);
  }
  Matrix<U> res = uInverse * lInverse * lup[0];
  return res;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::transpose(){
  Matrix<U> ret(_cols,_rows);
  for(int i = 0;i<_cols;i++){
    for(int j = 0; j < _rows;j++){
      ret[j][i] = _matrix[i][j];
    }
  }
  return ret;
}


template <Arithmetic U>
Matrix<U> Matrix<U>::fillBefore(Matrix<U> A, uint n){
  Matrix<U> ret = Matrix<U>::eye(A.rows()+n,A.cols()+n);
  for(int i = n;i<ret.cols();i++){
    for(int j = n; j < ret.rows();j++){
      ret[i][j] = A[i-n][j-n];
    }
  }
  return ret;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::subCol(uint col,uint rowInit){
  int dim = _rows-rowInit;
  Matrix<U> ret(dim,1);
  for(int i = 0;i<dim;i++){
    ret[0][i] = _matrix[col][i+rowInit];
  }
  return ret;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::subRow(uint row,uint colInit){
  int dim = _cols-colInit;
  Matrix<U> ret(dim,1);
  for(int i = 0;i<dim;i++){
    ret[0][i] = _matrix[i+colInit][row];
  }
  return ret;
}

template <Arithmetic U>
U Matrix<U>::euclidianNorm(){
  assertVector(*this);
  U res = 0;
  for(int i = 0;i<_rows;i++){
    res += _matrix[0][i]*_matrix[0][i];
  }
  return std::sqrt(res);
}

template <Arithmetic U>
U Matrix<U>::dot(Matrix<U> A){
  assertVector(*this);
  if(A.rows() != _rows){
    throw std::domain_error("Trying to dot product two vectors of different length." );
  }
  U res = 0;
  for(int i = 0;i<_rows;i++){
    res += _matrix[0][i]*A[0][i];
  }
  return res;
}

template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::hhZero1(Matrix<U> A){

  std::vector<Matrix<U>> ret(2);
  Matrix<U> x = A.subCol(0);
  Matrix<U> hu;
  U beta;
  uint pivotIdx = x.pivot(0);
  U maxVal = std::abs(x[0][pivotIdx]);
  x = x/maxVal;
  U colNorm = x.euclidianNorm();
  hu = x;
  if(hu[0][0]>=0){
    hu[0][0] = hu[0][0] + colNorm;
  }
  else{
    hu[0][0] = hu[0][0] - colNorm;
  }

  U huNorm = hu.euclidianNorm(); 

  if(!Misc::Util::almostZero(huNorm)){
    beta = 2.f/(huNorm*huNorm);
  }
  else{
    beta = 0.f;
  }
  A = A - (beta*hu)*(hu.transpose()*A);
  ret[0] = A;
  ret[1] = hu;
  return ret;
}



template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::hhZero2(Matrix<U> A,uint i, uint j, bool row, int dim){
  int m = A.rows();
  int n = A.cols();
  std::vector<Matrix<U>> ret(3);
  Matrix<U> H = Matrix<U>::eye(dim);
  Matrix<U> u;
  if(row == false){
    u = A.subCol(j,i);
  }
  else{
    u = A.subRow(i,j);
  }

  uint maxPos = u.pivot(0);
  U max = std::abs(u[0][maxPos]);
  U uNorm; 

  if(!Misc::Util::almostZero(max)){
    u = u/max;
    uNorm = u.euclidianNorm();
  }
  else{
    for(int k = 0;k<u.rows();k++){
      u[0][k] = INFINITY;
      
    }
    uNorm = INFINITY;
  }
  if(u[0][0] >= 0){
    u[0][0] = u[0][0] + uNorm;
  }
  else{
    u[0][0] = u[0][0] - uNorm;
  }
  U beta;
  U uNorm2 = u.euclidianNorm();
  if(!Misc::Util::almostZero(uNorm2)){
    beta = 2./(uNorm2*uNorm2);
  }
  else{
    beta = 0;
  }
  Matrix<U> subA = A.subMatrix(i,m-1,j,n-1);
  Matrix<U> subH;
  
  if(row == false){
    subA =  subA - beta*u*(u.transpose()*subA);
    subH = H.subMatrix(i,dim-1,i,dim-1);
    subH = Matrix<U>::eye(dim-i) - beta*u*u.transpose();
    H.replaceSubMatrix(subH,i,dim-1,i,dim-1);
  }
  else{
    u = u.transpose();
    subA =  subA - beta*subA*u.transpose()*u;
    subH = H.subMatrix(j,dim-1,j,dim-1);
    subH = Matrix<U>::eye(dim-j) - beta*u.transpose()*u;
    H.replaceSubMatrix(subH,j,dim-1,j,dim-1);
  }
  A.replaceSubMatrix(subA,i,m-1,j,n-1);
  ret[0] = A;
  ret[1] = u;
  ret[2] = H;
  return ret;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::roundMatrix(Matrix<U> A){
  Matrix<U> ret = A;
  for(int i = 0; i< A.cols();i++){
    for(int j = 0;j<A.rows();j++){
      U val = A[i][j];
      if(Misc::Util::almostZero(val) || isnanf(val)){
        ret[i][j] = .0f;
      }
    }
  }
  return ret;
}

template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::QRdecomp(Matrix<U> A){
  std::vector<Matrix<U>> res(2);
  Matrix<U> R = A;
  Matrix<U> Q = Matrix<U>::eye(A.rows());
  uint k = std::min(A.rows()-1,A.cols());
  for(int i = 0;i<k;i++){
    std::vector<Matrix<U>> ru = hhZero1(R.subMatrix(i,R.rows()-1,i,R.cols()-1));
    R.replaceSubMatrix(ru[0],i,R.rows()-1,i,R.cols()-1);
    Matrix<U> uh = ru[1];
    U uhNorm = uh.euclidianNorm();
    Matrix<U> qsub = Q.subMatrix(0,Q.rows()-1,i,Q.cols()-1);
    Matrix<U> qres = qsub - ((2.f/(uhNorm*uhNorm))*qsub)*(uh*uh.transpose());   //qsub - qumul;
    Q.replaceSubMatrix(qres,0,Q.rows()-1,i,Q.cols()-1);
  }
  res[0] = Q;
  res[1] = R;
  return res;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::subMatrix(uint rowIni,uint rowEnd, uint colIni, uint colEnd){
  if(rowEnd  > _rows || colEnd > _cols){
    throw std::invalid_argument("Row or Col out  of range.");
  }
  
  Matrix<U> ret(rowEnd - rowIni+1,colEnd-colIni+1);
  for(int i = colIni;i<=colEnd;i++){
    for(int j = rowIni;j<=rowEnd;j++){
      ret[i-colIni][j-rowIni] = _matrix[i][j];
    }
  }
  return ret;
}

template <Arithmetic U>
void Matrix<U>::replaceSubMatrix(Matrix<U> A, uint rowIni,uint rowEnd, uint colIni, uint colEnd){
  if(rowEnd  > _rows || colEnd > _cols){
    throw std::invalid_argument("Row or Col out  of range.");
  }
  for(int i = colIni;i<=colEnd;i++){
    for(int j = rowIni;j<=rowEnd;j++){
      _matrix[i][j] = A[i-colIni][j-rowIni];
    }
  }
}

template <Arithmetic U>
bool Matrix<U>::checkColOrthogonality(Matrix<U> A){
  for(int i = 0;i<A.cols();i++){
    for(int j = i+1;j<A.cols();j++){
      if(!Misc::Util::almostZero(A.subCol(i).dot(A.subCol(j)))){
        return false;
      }
    }
  }
  return true;
}

template <Arithmetic U>
bool Matrix<U>::checkColUnitary(Matrix<U> A){
  for(int i = 0;i<A.cols();i++){
    Matrix<U> col = A.subCol(i);
    if(!Misc::Util::almostZero(1.-col.euclidianNorm())){
      return false;
    }
  }
  return true;
}


template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::bidiagonalize(Matrix<U> A){
  std::vector<Matrix<U>> ret(3);
  Matrix<U> u;
  Matrix<U> H;
  int m = A.rows();
  int n1 = A.cols();
  int n = std::min(m-1,n1);
  Matrix<U> H1 = Matrix<U>::eye(m);
  Matrix<U> H2 = Matrix<U>::eye(n1);
  for(int i = 0; i<n;i++){
    ret = hhZero2(A,i,i,0,m);
    A = ret[0];
    u = ret[1];
    H = ret[2];
    H1 = H*H1;
    if(i<n-1){
      ret = hhZero2(A,i,i+1,1,n1);
      A = ret[0];
      u = ret[1];
      H = ret[2];
      H2 = H2*H;
    }
  }
  ret[0] = A;
  ret[1] = H1;
  ret[2] = H2;
  return ret;
}

template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::QRiteration(Matrix<U> A){
  bool done = false;
  int tolIt = 200;
  int tolExp = 15;
  int l = 0;
  std::vector<Matrix<U>> ret(3);
  std::vector<Matrix<U>> temp;
  Matrix<U> Qk1;
  Matrix<U> Rk1;
  Matrix<U> Uk = Matrix<U>::eye(A.rows());
  Matrix<U> Vk = Matrix<U>::eye(A.rows());
  Matrix<U> Ak = A;
  while(!done){
    Uk = Matrix<U>::eye(A.rows());
    Vk = Matrix<U>::eye(A.rows());
    Ak = A;
    while(!Ak.isDiagonal(tolExp)){
      temp = Matrix<U>::QRdecomp(Ak);
      Qk1 = temp[0];
      Rk1 = temp[1];

      Ak = Rk1*Qk1;
      Uk = Uk*Qk1;
      if(l>=tolIt){
        break;
      }
      else{
        ++l;
      }
    }
    if(l>=tolIt){
      --tolExp;
      l = 0;
    }
    else{
      done = true;
    }
  }
  ret[0] = Ak;
  ret[1] = Uk;
  ret[2] = Uk.transpose();
  return ret;

}

template <Arithmetic U>
std::vector<Matrix<U>> Matrix<U>::symmetricSVD(Matrix<U> A){
  if(!A.isSymmetric()){
    throw std::invalid_argument("Trying to compute symmetric SVD of non symmetric matrix.");
  }
  std::vector<Matrix<U>> ret(3);
  std::vector<Matrix<U>> svdDecomp = Matrix<U>::QRiteration(A);
  ret[0] = svdDecomp[1];
  ret[1] = svdDecomp[0];
  ret[2] = svdDecomp[2];
  return ret;
}

template <Arithmetic U>
bool Matrix<U>::isSymmetric(){
  bool ret = true;
  assertSquare(*this);
  for(int i = 0; i< _cols;i++){
    for(int j = 0; j<_rows;j++){
      if(_matrix[i][j] != _matrix[j][i]){
        return false;
      }
    }
  }
  return ret;
}

template <Arithmetic U>
bool Matrix<U>::isDiagonal(int tol){
  bool ret = true;
  for(int i = 0; i< _cols;i++){
    for(int j = 0; j<_rows;j++){
      if(i!=j){
        if(!Misc::Util::almostZero(_matrix[i][j],tol)){
          return false;
        }
      }
    }
  }
  return ret;
}

template <Arithmetic U>
U Matrix<U>::det(){
    if(_rows!=_cols){
        throw std::runtime_error("This Matrix does not support determinant.\n");
    }
    if(_rows == 0){
      return 0;
    }
    else if(_rows == 1){
      return _matrix[0][0];
    }
    else if(_rows == 2){
      return _matrix[0][0] * _matrix[1][1] - _matrix[1][0] * _matrix[0][1];
    }
    else if(_rows == 3){
    return (_matrix[0][0]*_matrix[1][1]*_matrix[2][2] + 
            _matrix[1][0]*_matrix[2][1]*_matrix[0][2] +
            _matrix[2][0]*_matrix[0][1]*_matrix[1][2] -
            _matrix[2][0]*_matrix[1][1]*_matrix[0][2] -
            _matrix[0][0]*_matrix[2][1]*_matrix[1][2] -
            _matrix[1][0]*_matrix[0][1]*_matrix[2][2]
            );
    }
    else if(_rows == 4){
      U result = 0;
      int sign = 1;
      for(int i = 0; i < 4; i++) {

          //Submatrix construction
          Matrix<U> subm(3,3);
          for(int l = 1; l < 4; l++) {
              int z = 0;
              for(int k = 0; k < 4; k++) {
                  if(k != i) {
                      subm[z][l-1] = _matrix[k][l];
                      z++;
                  }
              }
          }

          //recursive call
          result = result + sign * _matrix[i][0] * subm.det();
          sign = -sign;
      }

      return result;
    }
    else{
      U result = 0;
      int sign = 1;
      for(int i = 0; i < _cols; i++) {

          //Submatrix construction
          Matrix<U> subm(_rows,_cols);
          for(int l = 1; l < _rows; l++) {
              int z = 0;
              for(int k = 0; k < _cols; k++) {
                  if(k != i) {
                      subm[z][l-1] = _matrix[k][l];
                      z++;
                  }
              }
          }

          //recursive call
          result = result + sign * _matrix[i][0] * subm.det();
          sign = -sign;
      }

      return result;
    }
}

template <Arithmetic U>
Matrix<U> Matrix<U>::stackVectors(std::vector<Matrix<U>> A,uint axis){
  int m = A[0].rows();
  int n = A[0].cols();
  //check if all vectors have the same dim.
  for(Matrix<U> mat : A){
    if(mat.rows() != m || mat.cols() != n){
      throw std::invalid_argument("Trying to stack vectors of different magnitude.");
    }
  }
  //
  Matrix<U> ret;
  if(axis == 0){
    ret = Matrix<U>(m,n*A.size());
    for(int i = 0; i < n*A.size();i++){
      for(int j = 0;j<m;j++){
        ret[i][j] = A[i/n][i%n][j];
      }
    }
  }

  else{
    ret = Matrix<U>(m*A.size(),n);
    for(int i = 0; i < n;i++){
      for(int j = 0;j<m*A.size();j++){
        ret[i][j] = A[j/m][i][j%m];
      }
    }
  }
  return ret;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::covariance(Matrix<U> A){
  Matrix<U> meanMatrix = Matrix<U>::columnMean(A);
  Matrix<U> covMatrix = (A - meanMatrix).transpose()*(A-meanMatrix)/(A.rows());
  return covMatrix;
}

template <Arithmetic U>
Matrix<U> Matrix<U>::columnMean(Matrix<U> A, bool stack){
  uint m = A.rows();
  uint n = A.cols();
  Matrix<U> means(m,n);
  for(int i = 0; i < n;i++){
    U mean = 0;
    for(int j = 0; j< m;j++){
      mean+=A[i][j];
    }
    means[i][0] = mean/(m);
  }
  if(!stack){
    return means.subRow(0).transpose();
  }
  //expanding 
  for(int i = 0; i < n;i++){
    for(int j = 1; j< m;j++){
      means[i][j] = means[i][0];
    }
  }
  //printMatrix(means);
  return means;
}

template <Arithmetic U>
void Matrix<U>::assertSquare(Matrix<U> & A){
  if(A.rows() != A.cols()){
    throw std::invalid_argument("Matrix is not square.");
  }
}

template <Arithmetic U>
void Matrix<U>::assertVector(Matrix<U> & A){
  if(A.cols() != 1 && A.rows() != 1){
    throw std::invalid_argument("Matrix is not a vector.");
  }
}

template <Arithmetic U>
glm::vec3 Matrix<U>::toVec3(){
  assertVector(*this);
  glm::vec3 r;
  if(_rows > 1){//column vector
    r[0] = _matrix[0][0];
    r[1] = _matrix[0][1];
    r[2] = _matrix[0][2];
    return r;
  }
  r[0] = _matrix[0][0];
  r[1] = _matrix[1][0];
  r[2] = _matrix[2][0];
  return r;
}



#endif //MATRIX_HPP
