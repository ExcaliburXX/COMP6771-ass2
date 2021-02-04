#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept { return what_.c_str(); }

 private:
  std::string what_;
};

class EuclideanVector {
 public:
  /* CONSTRUCTORS */
  explicit EuclideanVector(int size = 1) noexcept
    : EuclideanVector(size, 0.0) {}  // default constructor
  ~EuclideanVector() noexcept;       // destructor

  EuclideanVector(int size, double magnitude) noexcept;
  EuclideanVector(std::vector<double>::const_iterator start_it,
                  std::vector<double>::const_iterator end_it) noexcept;
  EuclideanVector(const EuclideanVector& ev) noexcept;  // copy constructor
  EuclideanVector(EuclideanVector&& ev) noexcept;       // move constructor

  /* METHODS */
  int GetNumDimensions() const noexcept { return this->size_; }
  double at(int i) const;  // getter at index i
  double& at(int i);       // setter at index i
  double GetEuclideanNorm() const;
  EuclideanVector CreateUnitVector() const;

  /* OPERATIONS */
  EuclideanVector& operator=(const EuclideanVector& ev) noexcept;  // copy assignment
  EuclideanVector& operator=(EuclideanVector&& ev) noexcept;       // move assignment

  // Subscript assignment
  double operator[](int i) const noexcept;
  double& operator[](int i) noexcept;

  // Mathematical operators on vectors
  EuclideanVector& operator+=(const EuclideanVector& ev);
  EuclideanVector& operator-=(const EuclideanVector& ev);
  EuclideanVector& operator*=(const double n) noexcept;
  EuclideanVector& operator/=(const double n);

  // Vector and List Type Conversion
  explicit operator std::vector<double>() const noexcept;
  explicit operator std::list<double>() const noexcept;

  /* FRIENDS */
  // Equality and Inequality operators
  friend bool operator==(const EuclideanVector& o1, const EuclideanVector& o2) noexcept;
  friend bool operator!=(const EuclideanVector& o1, const EuclideanVector& o2) noexcept;

  // Mathematical operations on two vectors (add, subtract, multiply)
  friend EuclideanVector operator+(const EuclideanVector& o1, const EuclideanVector& o2);
  friend EuclideanVector operator-(const EuclideanVector& o1, const EuclideanVector& o2);
  friend double operator*(const EuclideanVector& o1, const EuclideanVector& o2);

  // Inline operations (multiply and divide)
  friend EuclideanVector operator*(const EuclideanVector& o, double n) noexcept;  // vector * scalar
  friend EuclideanVector operator*(double n, const EuclideanVector& o) noexcept;  // scalar * vector
  friend EuclideanVector operator/(const EuclideanVector& o, double n);           // vector / scalar

  // Output stream to display vector details#include <sstream>  // used to check output from output
  // stream
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) noexcept;

 private:
  std::unique_ptr<double[]> magnitudes_;
  int size_;  // size of magnitudes_ and dimension of vector
};

#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
