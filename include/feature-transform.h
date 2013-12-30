#ifndef _FEATURE_TRANSFORM_H_
#define _FEATURE_TRANSFORM_H_

#include <dnn-utility.h>

template <typename T>
device_matrix<T> operator & (const device_matrix<T>& A, const device_matrix<T>& B) {
  assert(A.getRows() == B.getRows() && A.getCols() == B.getCols());

  device_matrix<T> C(A.getRows(), A.getCols());

  thrust::device_ptr<T> aPtr(A.getData());
  thrust::device_ptr<T> bPtr(B.getData());
  thrust::device_ptr<T> cPtr(C.getData());

  thrust::transform(aPtr, aPtr + A.size(), bPtr, cPtr, thrust::multiplies<T>());

  return C;
}

void substractMaxPerRow(mat& x);

/*class FeatureTransform {
public:
  virtual void feedForward(mat& fout, const mat& fin, size_t offset, size_t nData) { }
  virtual void backPropagate(mat& delta, mat& fin) { }
};*/

class AffineTransform /*: protected FeatureTransform*/ {
public:
  AffineTransform();
  AffineTransform(const AffineTransform& source);
  AffineTransform(const mat& w);
  AffineTransform(size_t rows, size_t cols);

  AffineTransform& operator = (AffineTransform rhs);
  void setOutputLayer(bool flag);

  mat& getW();
  const mat& getW() const;
  mat& getDw();
  const mat& getDw() const;

  void update(float learning_rate);
  void resize(size_t rows, size_t cols);

  virtual string toString() const;
  virtual void feedForward(mat& fout, const mat& fin, size_t offset, size_t nData);
  virtual void backPropagate(const mat& fin, const mat& fout, mat& error);

  friend void swap(AffineTransform& lhs, AffineTransform& rhs);

protected:
  bool _isOutputLayer;
  mat _w;
  mat _dw;
};

class Softmax : public AffineTransform {
public:
  Softmax(const mat& w);
  Softmax(size_t rows, size_t cols);

  Softmax& operator = (Softmax rhs);
  
  virtual string toString() const;
  virtual void feedForward(mat& fout, const mat& fin, size_t offset, size_t nData);
  virtual void backPropagate(const mat& fin, const mat& fout, mat& error);

  friend void swap(Softmax& lhs, Softmax& rhs);
};


#endif // _FEATURE_TRANSFORM_H_
