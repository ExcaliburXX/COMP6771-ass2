#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <assert.h>

// Helper function declarations
std::unique_ptr<double[]> CreateMagnitudes(int size, double magnitude) noexcept;
std::unique_ptr<double[]> CreateMagnitudes(std::vector<double>::const_iterator start_it,
                                           std::vector<double>::const_iterator end_it) noexcept;

/* CONSTRUCTORS */
// Takes number of dimensions (size) and sets magnitude in each dimension as 0.0
EuclideanVector::EuclideanVector(int size, double magnitude) noexcept
  : magnitudes_{CreateMagnitudes(size, magnitude)}, size_{size} {}

// Takes start and end of an std::vector iterator and sets magnitudes to the iterated values
EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start_it,
                                 std::vector<double>::const_iterator end_it) noexcept
  : magnitudes_{CreateMagnitudes(start_it, end_it)}, size_{static_cast<int>(
                                                         std::distance(start_it, end_it))} {}

// Copies vector to new vector
EuclideanVector::EuclideanVector(const EuclideanVector& ev) noexcept : size_{ev.size_} {
  magnitudes_ = std::make_unique<double[]>(this->size_);
  for (int i = 0; i < this->size_; ++i) {
    this->magnitudes_[i] = ev.magnitudes_[i];
  }
}

// Moves vector o to current vector
EuclideanVector::EuclideanVector(EuclideanVector&& o) noexcept
  : magnitudes_{std::move(o.magnitudes_)}, size_{o.size_} {
  o.size_ = 0;
}

/* DESTRUCTORS */
// Frees vector
//  Only on-primitives are freed here since primitive objects can be freed by compiler
EuclideanVector::~EuclideanVector() noexcept {
  this->magnitudes_.release();
}

/* METHODS */
// at (getter) - returns value of magnitude in dimension given as function parameter
double EuclideanVector::at(int i) const {
  if (i < 0 || i >= this->size_) {
    throw EuclideanVectorError("Index " + std::to_string(i) +
                               " is not valid for this EuclideanVector object");
  }
  return this->magnitudes_[i];
}

// at (setter) - returns reference of magnitude in dimension given as function parameter
double& EuclideanVector::at(int i) {
  if (i < 0 || i >= this->size_) {
    throw EuclideanVectorError("Index " + std::to_string(i) +
                               " is not valid for this EuclideanVector object");
  }
  return this->magnitudes_[i];
}

// Gets the euclidean norm of the vector as a double
double EuclideanVector::GetEuclideanNorm() const {
  if (this->GetNumDimensions() == 0) {
    throw("EuclideanVector with no dimensions does not have a norm");
  }

  double euclidNorm = 0;
  for (int i = 0; i < this->size_; ++i) {
    euclidNorm += std::pow(this->magnitudes_[i], 2);
  }
  euclidNorm = std::sqrt(euclidNorm);
  return euclidNorm;
}

// Returns a Euclidean vector that is the unit vector of *this vector
EuclideanVector EuclideanVector::CreateUnitVector() const {
  if (this->GetNumDimensions() == 0) {
    throw("EuclideanVector with no dimensions does not have a unit vector");
  }
  if (this->GetEuclideanNorm() == 0) {
    throw("EuclideanVector with euclidean normal of 0 does not have a unit vector");
  }

  EuclideanVector ev(this->size_);
  for (int i = 0; i < this->size_; ++i) {
    ev.magnitudes_[i] = this->magnitudes_[i] / GetEuclideanNorm();
  }
  return ev;
}

/* OPERATIONS */
// Copy assigns ev to *this
EuclideanVector& EuclideanVector::operator=(const EuclideanVector& ev) noexcept {
  *this = EuclideanVector(ev);
  return *this;
}

// Move assigns ev to *this
EuclideanVector& EuclideanVector::operator=(EuclideanVector&& ev) noexcept {
  this->magnitudes_ = std::move(ev.magnitudes_);
  this->size_ = ev.size_;
  ev.size_ = 0;
  return *this;
}

// [Subscript getter] Gets the value in a given dimension of vector
double EuclideanVector::operator[](int i) const noexcept {
  assert(i >= 0 || i < this->size_);
  return at(i);
}

// [Subscript setter] Gets reference to value in a given dimension of vector
double& EuclideanVector::operator[](int i) noexcept {
  assert(i >= 0 || i < this->size_);
  return at(i);
}

// Adds vector's magnitude values by ev's corresponding magnitude values
EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& ev) {
  if (this->GetNumDimensions() != ev.GetNumDimensions()) {
    std::string l_dims = std::to_string(this->GetNumDimensions());
    std::string r_dims = std::to_string(ev.GetNumDimensions());
    throw("Dimensions of LHS(" + l_dims + ") and RHS(" + r_dims + ") do not match");
  }

  for (int i = 0; i < this->size_; ++i) {
    this->magnitudes_[i] += ev.magnitudes_[i];
  }
  return *this;
}

// Subtracts vector's magnitude values by ev's corresponding magnitude values
EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& ev) {
  if (this->GetNumDimensions() != ev.GetNumDimensions()) {
    std::string l_dims = std::to_string(this->GetNumDimensions());
    std::string r_dims = std::to_string(ev.GetNumDimensions());
    throw("Dimensions of LHS(" + l_dims + ") and RHS(" + r_dims + ") do not match");
  }

  for (int i = 0; i < this->size_; ++i) {
    this->magnitudes_[i] -= ev.magnitudes_[i];
  }
  return *this;
}

// Multiplies vector's magnitude values by n
EuclideanVector& EuclideanVector::operator*=(const double n) noexcept {
  for (int i = 0; i < this->size_; ++i) {
    this->magnitudes_[i] *= n;
  }
  return *this;
}

// Divides vector's magnitude values by n
EuclideanVector& EuclideanVector::operator/=(const double n) {
  if (n == 0) {
    throw("Invalid vector division by 0");
  }

  for (int i = 0; i < this->size_; ++i) {
    this->magnitudes_[i] /= n;
  }
  return *this;
}

// Operator for type casting vector to a std::vector object
EuclideanVector::operator std::vector<double>() const noexcept {
  std::vector<double> vec;
  vec.reserve(this->size_);

  for (int i = 0; i < this->size_; ++i) {
    vec.emplace_back(at(i));
  }
  return vec;
}

// Operator for type casting vector to a std::list object
EuclideanVector::operator std::list<double>() const noexcept {
  std::list<double> lt;

  for (int i = 0; i < this->size_; ++i) {
    lt.emplace_back(at(i));
  }
  return lt;
}

/* FRIENDS */
// Returns true if the two vectors are equal in num. of dims. and magnitude values
bool operator==(const EuclideanVector& o1, const EuclideanVector& o2) noexcept {
  if (o1.GetNumDimensions() != o2.GetNumDimensions())
    return false;

  for (int i = 0; i < o1.GetNumDimensions(); ++i) {
    if (o1.at(i) != o2.at(i))
      return false;
  }
  return true;
}

// Returns true if the two vectors are not equal in num. of dims. or magnitude values
bool operator!=(const EuclideanVector& o1, const EuclideanVector& o2) noexcept {
  if (o1.GetNumDimensions() != o2.GetNumDimensions())
    return true;

  for (int i = 0; i < o1.GetNumDimensions(); ++i) {
    if (o1.at(i) != o2.at(i))
      return true;
  }
  return false;
}

// Returns result of addition of vectors o1 and o2
EuclideanVector operator+(const EuclideanVector& o1, const EuclideanVector& o2) {
  if (o1.GetNumDimensions() != o2.GetNumDimensions()) {
    std::string l_dims = std::to_string(o1.GetNumDimensions());
    std::string r_dims = std::to_string(o2.GetNumDimensions());
    throw("Dimensions of LHS(" + l_dims + ") and RHS(" + r_dims + ") do not match");
  }

  EuclideanVector ev(o1.GetNumDimensions());
  for (int i = 0; i < ev.size_; ++i) {
    ev.at(i) = o1.at(i) + o2.at(i);
  }
  return ev;
}

// Returns result of subtraction of vectors o1 and o2
EuclideanVector operator-(const EuclideanVector& o1, const EuclideanVector& o2) {
  if (o1.GetNumDimensions() != o2.GetNumDimensions()) {
    std::string l_dims = std::to_string(o1.GetNumDimensions());
    std::string r_dims = std::to_string(o2.GetNumDimensions());
    throw("Dimensions of LHS(" + l_dims + ") and RHS(" + r_dims + ") do not match");
  }

  EuclideanVector ev(o1.GetNumDimensions());
  for (int i = 0; i < ev.size_; ++i) {
    ev.at(i) = o1.at(i) - o2.at(i);
  }
  return ev;
}

// Returns result of dot-product multiplication of vectors o1 and o2
double operator*(const EuclideanVector& o1, const EuclideanVector& o2) {
  if (o1.GetNumDimensions() != o2.GetNumDimensions()) {
    std::string l_dims = std::to_string(o1.GetNumDimensions());
    std::string r_dims = std::to_string(o2.GetNumDimensions());
    throw("Dimensions of LHS(" + l_dims + ") and RHS(" + r_dims + ") do not match");
  }

  double res = 0;
  for (int i = 0; i < o1.GetNumDimensions(); ++i) {
    res += o1.at(i) * o2.at(i);
  }
  return res;
}

// Multiplies vector's magnitude values by n
//  Format: EuclideanVector * double
EuclideanVector operator*(const EuclideanVector& o, double n) noexcept {
  EuclideanVector ev(o.GetNumDimensions());

  for (int i = 0; i < ev.size_; ++i) {
    ev.at(i) = n * o.at(i);
  }
  return ev;
}

//  Format: double * EuclideanVector
EuclideanVector operator*(double n, const EuclideanVector& o) noexcept {
  EuclideanVector ev = o * n;
  return ev;
}

// Divides vector's magnitude values by double 'n'
EuclideanVector operator/(const EuclideanVector& o, double n) {
  if (n == 0) {
    throw("Invalid vector division by 0");
  }

  EuclideanVector ev(o.GetNumDimensions());
  for (int i = 0; i < ev.size_; ++i) {
    ev.at(i) = o.at(i) / n;
  }
  return ev;
}

// Outputs EuclideanVector in string format in output stream os
std::ostream& operator<<(std::ostream& os, const EuclideanVector& ev) noexcept {
  int size = ev.GetNumDimensions();
  os << "[";
  for (int i = 0; i < size; ++i) {
    if (i == size - 1)
      os << ev.magnitudes_[i];
    else
      os << ev.magnitudes_[i] << " ";
  }
  os << "]";
  return os;
}

/* HELPER FUNCTIONS */
std::unique_ptr<double[]> CreateMagnitudes(int size, double magnitude) noexcept {
  std::unique_ptr<double[]> magnitudes = std::make_unique<double[]>(size);
  for (int i = 0; i < size; ++i) {
    magnitudes[i] = magnitude;
  }
  return magnitudes;
}

std::unique_ptr<double[]> CreateMagnitudes(std::vector<double>::const_iterator start_it,
                                           std::vector<double>::const_iterator end_it) noexcept {
  int size = static_cast<int>(std::distance(start_it, end_it));
  std::unique_ptr<double[]> magnitudes = std::make_unique<double[]>(size);
  int i = 0;
  for (auto it = start_it; it != end_it; ++it) {
    magnitudes[i] = *it;
    ++i;
  }
  return magnitudes;
}
