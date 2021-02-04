/*

  == Explanation and rational of testing ==
  I approached the testing in a simple but thorough manner in order to
  increase coverage of all possibilities. I have written tests and have run them during the
  implementation process as to ensure my code was definitely working as intended and so I had a
  better idea of the correctness of my tests so far and also what else I should check for.

  I had to implement around 25 constructors/methods/overloaded operators and so that meant at the
  very least 1 test case for each implementation. Some were simple enough that there was only one
  case to test (which is usually consists of testing any one valid vector). A good bunch of these
  implementations however, required more than 1 test case, more specifically empty vector cases
  and/or exception cases which were given. Due to this, I have been a bit more careful around what
  cases I must test and to read the specs more thoroughly. Therefore, I think I have covered a
  majority if not all possibilities due to my understanding of the limitations of vectors (eg.
  cannot create empty vector but can form one after using move constructor/move assignment so I must
  test these cases).

  Below is the format in which I structured my test cases:

  <CONSTRUCTOR/METHOD/OPERATION>:

  VALID TEST CASE 1
  VALID TEST CASE 2
  VALID TEST CASE 3
  ...

  EXCEPTION TEST CASE 1
  EXCEPTION TEST CASE 2
  ...

  This way, it ensures I don't miss any cases.

  In terms of brittleness, the tests won't need to be changed as I only call functions and
  constructors which are required and specified in the Assignment specs. Regardless of whether I
  change the implementation, the functions used will still exist and should work as expected.

  In terms of clarity, the fact that I break it down to each function and test cases to target that
  particular function makes it easier to figure out what is wrong as the tests are quite simple and
  do not call too many functions (only those necessary). The further down you go in the test cases
  (and functions), the more you will see me use other functions but most of these functions are
  those tested in the previous test cases.

*/

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

/*
 * Default constructor:
 *  EuclideanVector()
 *  EuclideanVector(int)
 */
SCENARIO("Creation of a vector with default constructor") {
  WHEN("You create a new vector without a given size") {
    EuclideanVector a{};

    THEN("A default vector of size 1 and magnitude 0 is created") {
      REQUIRE(a.GetNumDimensions() == 1);
      REQUIRE(a.at(0) == 0);
    }
  }
}

SCENARIO("Creation of a vector with only size given") {
  WHEN("You create a new vector with only a given size") {
    EuclideanVector a{2};

    THEN("A vector with magnitude 0 in each dimension is created") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a.at(0) == 0);
      REQUIRE(a.at(1) == 0);
    }
  }
}

/*
 * Constructor:
 *  EuclideanVector(int, double)
 */
SCENARIO("Creation of a vector with size and magnitude given") {
  WHEN("You create a new vector given a size and magnitude") {
    EuclideanVector a{2, 1};

    THEN("A vector with magnitudes 1 in each dimension is created") {
      REQUIRE(a.GetNumDimensions() == 2);
      REQUIRE(a.at(0) == 1);
      REQUIRE(a.at(1) == 1);
    }
  }
}

/*
 * Constructor:
 *  EuclideanVector(it.begin(), it.end())
 */
SCENARIO("Creation of a vector given start and end of an iterator to a std::vector") {
  WHEN("You create a vector") {
    std::vector<double> v{0, -3, 1, 15};
    EuclideanVector a{v.begin(), v.end()};

    THEN("A vector with magnitudes corresponding to v's values is created") {
      REQUIRE(a.GetNumDimensions() == 4);
      REQUIRE(a.at(0) == 0);
      REQUIRE(a.at(1) == -3);
      REQUIRE(a.at(2) == 1);
      REQUIRE(a.at(3) == 15);
    }
  }
}

/* Copy constructor */
SCENARIO("Copying a vector to new vector") {
  WHEN("You create a vector") {
    std::vector<double> v{3.5, 2.41, 0.67};
    EuclideanVector a{v.begin(), v.end()};

    THEN("You use the copy constructor to create an identical vector") {
      EuclideanVector b{a};
      REQUIRE(b == a);
    }
  }
}

/* Move Constructor */
SCENARIO("Moving a vector to a new vector") {
  WHEN("You create a vector") {
    EuclideanVector a{2, 4.25};

    THEN("You use the move constructor to create a new vector") {
      EuclideanVector b = std::move(a);

      REQUIRE(b.GetNumDimensions() == 2);
      REQUIRE(b.at(0) == 4.25);
      REQUIRE(b.at(1) == 4.25);
      REQUIRE(a.GetNumDimensions() == 0);
    }
  }
}

SCENARIO("Copying a moved vector to new vector") {
  WHEN("You create a moved vector") {
    std::vector<double> v{3.5, 2.41, 0.67};
    EuclideanVector a{v.begin(), v.end()};

    EuclideanVector b{std::move(a)};

    REQUIRE(a.GetNumDimensions() == 0);

    THEN("You use the copy assignment to get an empty vector") {
      EuclideanVector c{a};
      REQUIRE(c.GetNumDimensions() == 0);
    }
  }
}

/*
 * Method:
 *  EuclideanVector CreateUnitVector()
 */
SCENARIO("Creation of two unit vectors") {
  WHEN("You have two identical vectors constructed differently") {
    EuclideanVector a{2};
    a.at(0) = 3;
    a.at(1) = 8;

    std::vector<double> v{3, 8};
    EuclideanVector b{v.begin(), v.end()};

    REQUIRE(a == b);

    THEN("Get their unit vectors") {
      EuclideanVector c{a.CreateUnitVector()};
      EuclideanVector d{b.CreateUnitVector()};

      REQUIRE(c == d);
    }
  }
}

// EXCEPTION - no dimensions nor unit vector
SCENARIO("Creating Unit Vector of vector with no dimensions") {
  WHEN("You create a vector") {
    EuclideanVector a{};
    EuclideanVector b{std::move(a)};

    REQUIRE(a.GetNumDimensions() == 0);

    THEN("Get their unit vectors") {
      REQUIRE_THROWS_WITH(a.CreateUnitVector(),
                          "EuclideanVector with no dimensions does not have a unit vector");
    }
  }
}

// EXCEPTION - euclid norm of 0
SCENARIO("Creating Unit Vector of vector with euclidean norm of 0") {
  WHEN("You create a vector") {
    EuclideanVector a{2};

    REQUIRE(a.GetNumDimensions() == 2);

    THEN("Get their unit vectors") {
      REQUIRE_THROWS_WITH(a.CreateUnitVector(),
                          "EuclideanVector with euclidean normal of 0 does not have a unit vector");
    }
  }
}

/*
 * Method:
 *  GetEuclideanNorm()
 */
SCENARIO("Calculation of two vector's euclidean norms") {
  WHEN("You create two vectors of same size") {
    std::vector<double> v1{6, 3, 6, 1, 2, 14, 10};
    EuclideanVector a{v1.begin(), v1.end()};

    std::vector<double> v2{5, 3, 9, 5, 2, 1, 5};
    EuclideanVector b{v2.begin(), v2.end()};

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());
    REQUIRE(a.at(0) != b.at(0));

    THEN("Get their euclidean norms") { REQUIRE(a.GetEuclideanNorm() != b.GetEuclideanNorm()); }
  }
}

SCENARIO("Calculation of euclidean norms of unit vectors") {
  WHEN("You create 2 different vectors with same dimensions") {
    EuclideanVector a{2, 1};

    EuclideanVector b{2, 0};
    b.at(1) = 15;

    REQUIRE(a != b);
    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());

    THEN("Get their unit vector's euclidean norms") {
      REQUIRE(a.CreateUnitVector().GetEuclideanNorm() != b.CreateUnitVector().GetEuclideanNorm());
    }
  }
}

// EXCEPTION: no dimensions nor norm
SCENARIO("Getting Euclidean Norm of a vector with no dimensions nor norm") {
  WHEN("You create a vector") {
    EuclideanVector a{3};
    EuclideanVector b{std::move(a)};

    REQUIRE(a.GetNumDimensions() == 0);

    THEN("You get an exception error") {
      REQUIRE_THROWS_WITH(a.GetEuclideanNorm(),
                          "EuclideanVector with no dimensions does not have a norm");
    }
  }
}

/* at (getter method) */
SCENARIO("Accessing a specific magnitude in a dimension of the vector") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{1, 7, 3, 5, 9, 0};
    EuclideanVector a{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Get the magnitude in the last dimension") {
      double m = a.at(a.GetNumDimensions() - 1);
      REQUIRE(m == 0);
    }
  }
}

// EXCEPTION: invalid index
SCENARIO("Accessing dimension that doesn't exist") {
  WHEN("You create a vector of size 2") {
    EuclideanVector a{5, 12};

    REQUIRE(a.GetNumDimensions() == 5);

    THEN("You get an exception error when accessing non-existent 6th element ") {
      REQUIRE_THROWS_WITH(a.at(5), "Index 5 is not valid for this EuclideanVector object");
    }
  }
}

/* at (setter method) */
SCENARIO("Modifying the value in a dimension of the vector") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{1, 7, 3, 5, 9, 0};
    EuclideanVector a{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Modify the value in the 3rd dimension") {
      a.at(2) = 16;
      REQUIRE(a.at(2) != 3);
    }
  }
}

// EXCEPTION - index X not valid (X < 0)
SCENARIO("Modifying the value in an invalid dimension < 0") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{1, 7, 3.1, 5.23};
    EuclideanVector a{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == 4);

    THEN("Modify the value in the 3rd dimension") {
      REQUIRE_THROWS_WITH(a.at(-1), "Index -1 is not valid for this EuclideanVector object");
    }
  }
}

// EXCEPTION - index X not valid (X >= num. of dims)
SCENARIO("Modifying the value in an invalid bigger dimension") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{1, 7, 3.1, 5.23};
    EuclideanVector a{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == 4);

    THEN("Modify the value in the 3rd dimension") {
      REQUIRE_THROWS_WITH(a.at(5), "Index 5 is not valid for this EuclideanVector object");
    }
  }
}

/* Copy Assignment */
SCENARIO("Copy assignment of a vector") {
  WHEN("You create two vectors") {
    std::vector<double> v1{1, 7, 3, 5, 9, 0};
    EuclideanVector a{v1.begin(), v1.end()};

    std::vector<double> v2{-20, 0, 13};
    EuclideanVector b{v2.begin(), v2.end()};

    THEN("Copy first vector to second vector") {
      b = a;
      REQUIRE(b == a);
    }
  }
}

/* Move Assignment */
SCENARIO("Move assignment of a vector") {
  WHEN("You create two vectors") {
    std::vector<double> v1{1, 7, 3};
    EuclideanVector a{v1.begin(), v1.end()};

    std::vector<double> v2{-20};
    EuclideanVector b{v2.begin(), v2.end()};

    THEN("Move first vector to second vector") {
      b = std::move(a);
      REQUIRE(b.GetNumDimensions() == 3);
      REQUIRE(b[0] == 1);
      REQUIRE(b[1] == 7);
      REQUIRE(b[2] == 3);
      REQUIRE(a.GetNumDimensions() == 0);
    }
  }
}

/* Subscript (getter) */
SCENARIO("Accessing a dimension value with subscript operator") {
  WHEN("You create a vector") {
    std::vector<double> v1{4.8, 1.32, 3.2};
    EuclideanVector a{v1.begin(), v1.end()};

    REQUIRE(a.GetNumDimensions() == 3);

    THEN("Get magnitude in second dimension") {
      double m{a[1]};
      REQUIRE(m == 1.32);
    }
  }
}

/* Subscript (setter) */
SCENARIO("Modifying a dimension value with subscript operator") {
  WHEN("You create a vector of size 3") {
    std::vector<double> v1{4.8, 1.32, 3.2};
    EuclideanVector a{v1.begin(), v1.end()};

    REQUIRE(a.GetNumDimensions() == 3);

    THEN("Modify magnitude to 0 in second dimension") {
      a[1] = 0;
      REQUIRE(a.at(1) == 0);
    }
  }
}

/* Addition (a += b) */
SCENARIO("Inline adding one vector to another") {
  WHEN("You create two non-empty vectors of same size") {
    std::vector<double> vec{6, 2};
    EuclideanVector a{vec.begin(), vec.end()};

    EuclideanVector b{2};
    b[0] = 3;
    b[1] = -5;

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());
    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Inline add b to a modifies a and keeps b the same") {
      a += b;

      REQUIRE(a[0] == 9);
      REQUIRE(a[1] == -3);
      REQUIRE(b[0] == 3);
      REQUIRE(b[1] == -5);
    }
  }
}

// EXCEPTION - dimensions do not match
SCENARIO("Inline adding vector with different dimension") {
  WHEN("You create two vectors of different size") {
    std::vector<double> vec{6, 2};
    EuclideanVector a{vec.begin(), vec.end()};

    EuclideanVector b{1, 3};

    REQUIRE(a.GetNumDimensions() != b.GetNumDimensions());

    THEN("Inline add b to a returns exception error") {
      REQUIRE_THROWS_WITH(a += b, "Dimensions of LHS(2) and RHS(1) do not match");
    }
  }
}

/* Subtraction (a -= b) */
SCENARIO("Inline subtracting vector from another") {
  WHEN("You create two vectors of different size") {
    std::vector<double> v1{-2, 4, 8};
    EuclideanVector a{v1.begin(), v1.end()};

    std::vector<double> v2{-5, 10, 2};
    EuclideanVector b{v2.begin(), v2.end()};

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());

    THEN("Inline subtract b from a modifies a but keeps b the same") {
      a -= b;

      REQUIRE(a[0] == 3);
      REQUIRE(a[1] == -6);
      REQUIRE(a[2] == 6);

      REQUIRE(b[0] == -5);
      REQUIRE(b[1] == 10);
      REQUIRE(b[2] == 2);
    }
  }
}

// EXCEPTION - dimensions do not match
SCENARIO("Inline subtracting vector with different dimension") {
  WHEN("You create two vectors of different size") {
    std::vector<double> v1{-2, 4, 8};
    EuclideanVector a{v1.begin(), v1.end()};

    std::vector<double> v2{-5, 2};
    EuclideanVector b{v2.begin(), v2.end()};

    REQUIRE(a.GetNumDimensions() != b.GetNumDimensions());

    THEN("Inline subtract b returns exception error") {
      REQUIRE_THROWS_WITH(a -= b, "Dimensions of LHS(3) and RHS(2) do not match");
    }
  }
}

/* Multiplication (a *= 3) */
SCENARIO("Inline multiplying a vector by a double") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v1{6, -2.5};
    EuclideanVector a{v1.begin(), v1.end()};

    double n{3.1};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Inline divide a by given double") {
      a *= n;

      REQUIRE(a[0] == 18.6);
      REQUIRE(a[1] == -7.75);
    }
  }
}

/* Division (a *= 3) */
SCENARIO("Inline dividing a vector by a double") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v1{-3, 1.5};
    EuclideanVector a{v1.begin(), v1.end()};

    double n{1.5};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Inline divide a by given double") {
      a /= n;

      REQUIRE(a[0] == -2);
      REQUIRE(a[1] == 1);
    }
  }
}

// EXCEPTION - given double == 0
SCENARIO("Inline dividing a vector by 0") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v1{-3, 1.5};
    EuclideanVector a{v1.begin(), v1.end()};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Inline divide a by 0 returns exception error") {
      REQUIRE_THROWS_WITH(a / 0, "Invalid vector division by 0");
    }
  }
}

/* Vector Type Conversion */
SCENARIO("Conversion of vector to a std::vector<double>") {
  WHEN("You create a non-empty vector") {
    std::vector<double> vec{1, 6, 3};
    EuclideanVector a{vec.begin(), vec.end()};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Typecast the vector to a std::vector<double>") {
      auto v = std::vector<double>{a};

      REQUIRE(v[0] == 1);
      REQUIRE(v[1] == 6);
      REQUIRE(v[2] == 3);
    }
  }
}

/* List Type Conversion */
SCENARIO("Conversion of vector to a std::list<double>") {
  WHEN("You create a non-empty vector") {
    std::vector<double> vec{1, 7};
    EuclideanVector a{vec.begin(), vec.end()};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Typecast the euclidean vector to a std::list<double>") {
      auto v = std::list<double>{a};

      REQUIRE(v.front() == 1);
      REQUIRE(v.back() == 7);
    }
  }
}

/* Addition (a = b + c) */
SCENARIO("Addition of vectors of the same dimension") {
  WHEN("You create two non-empty vectors of same size") {
    EuclideanVector a{3};
    a.at(0) = 5;
    a.at(1) = 8;
    a.at(2) = 1;

    std::vector<double> v{4, 0, -12};
    EuclideanVector b{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());
    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Add the two vectors") {
      EuclideanVector c = a + b;

      REQUIRE(c[0] == 9);
      REQUIRE(c[1] == 8);
      REQUIRE(c[2] == -11);
    }
  }
}

// EXCEPTION - dimensions do not match
SCENARIO("Addition of vectors of different dimensions") {
  WHEN("You create two vectors of different size") {
    EuclideanVector a{3};
    a.at(0) = 5;
    a.at(1) = 8;
    a.at(2) = 1;

    EuclideanVector b{1, 4};

    REQUIRE(a.GetNumDimensions() != b.GetNumDimensions());

    THEN("Adding the vectors will return exception error") {
      REQUIRE_THROWS_WITH(a + b, "Dimensions of LHS(3) and RHS(1) do not match");
    }
  }
}

/* Subtraction (a = b - c) */
SCENARIO("Subtraction of vectors of the same dimension") {
  WHEN("You create two non-empty vectors of same size") {
    EuclideanVector a{3};
    a.at(0) = -5;
    a.at(1) = 8;
    a.at(2) = 11;

    std::vector<double> v{-4, 10, 2};
    EuclideanVector b{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());
    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Subtract the two vectors") {
      EuclideanVector c = a - b;

      REQUIRE(c[0] == -1);
      REQUIRE(c[1] == -2);
      REQUIRE(c[2] == 9);
    }
  }
}

// EXCEPTION - dimensions do not match
SCENARIO("Subtraction of vectors of different dimension") {
  WHEN("You create two vectors of different size") {
    EuclideanVector a{3};
    a.at(0) = -5;
    a.at(1) = 8;
    a.at(2) = 11;

    EuclideanVector b{1, 2.59};

    REQUIRE(a.GetNumDimensions() != b.GetNumDimensions());

    THEN("Subtracting the two vectors returns exception error") {
      REQUIRE_THROWS_WITH(a - b, "Dimensions of LHS(3) and RHS(1) do not match");
    }
  }
}

/* Multiplication (c {a * b}) */
SCENARIO("Dot product multiplication of vectors in the same dimension") {
  WHEN("You create two non-empty vectors of same size") {
    EuclideanVector a{3};
    a.at(0) = -3;
    a.at(1) = -8;
    a.at(2) = 0;

    std::vector<double> v{-4, 2, 2};
    EuclideanVector b{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == b.GetNumDimensions());
    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Get dot product multiplication of the two vectors") {
      double c{a * b};

      REQUIRE(c == -4);
    }
  }
}

// EXCEPTION - dimensions do not match
SCENARIO("Dot product multiplication of vectors of different dimension") {
  WHEN("You create two vectors of different size") {
    EuclideanVector a{2};
    a.at(0) = -3;
    a.at(1) = -8;

    EuclideanVector b{3, 2.3};
    b[2] = 11.1;

    REQUIRE(a.GetNumDimensions() != b.GetNumDimensions());

    THEN("Getting dot product multiplication returns exception error") {
      REQUIRE_THROWS_WITH(a * b, "Dimensions of LHS(2) and RHS(3) do not match");
    }
  }
}

/* Multiply */
// Scalar on left of vector
SCENARIO("Scalar multiplication of a vector with scalar on the left") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{-4, 2, 2};
    EuclideanVector a{v.begin(), v.end()};

    double n{2.5};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Get scalar multiplied vector where scalar is on the left") {
      EuclideanVector b = a * n;

      REQUIRE(b.GetNumDimensions() == a.GetNumDimensions());
      REQUIRE(b[0] == -10);
      REQUIRE(b[1] == 5);
      REQUIRE(b[2] == 5);
    }
  }
}

// Scalar on right of vector
SCENARIO("Scalar multiplication of a vector with scalar on the right") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{-4, 2, 2};
    EuclideanVector a{v.begin(), v.end()};

    double n{2.5};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Get scalar multiplied vector where scalar is on the right") {
      EuclideanVector b = n * a;

      REQUIRE(b.GetNumDimensions() == a.GetNumDimensions());
      REQUIRE(b[0] == -10);
      REQUIRE(b[1] == 5);
      REQUIRE(b[2] == 5);
    }
  }
}

/* Divide */
SCENARIO("Scalar division of a vector") {
  WHEN("You create a non-empty vector") {
    std::vector<double> v{-15, 0, 9};
    EuclideanVector a{v.begin(), v.end()};

    double n{2};

    REQUIRE(a.GetNumDimensions() > 0);

    THEN("Get scalar divided vector") {
      EuclideanVector b = a / n;

      REQUIRE(b.GetNumDimensions() == a.GetNumDimensions());
      REQUIRE(b[0] == -7.5);
      REQUIRE(b[1] == 0);
      REQUIRE(b[2] == 4.5);
    }
  }
}

/* Output Stream */
SCENARIO("Displaying vectors in text form") {
  WHEN("You have an empty vector") {
    EuclideanVector a{};
    EuclideanVector b{std::move(a)};

    REQUIRE(a.GetNumDimensions() == 0);

    THEN("You print the vector as []") {
      std::cout << a;

      std::stringstream ss;
      ss << a;
      REQUIRE(ss.str() == "[]");
    }
  }

  WHEN("You create a non-empty vector") {
    std::vector<double> v{-2.5, 1.3, 0.9};
    EuclideanVector a{v.begin(), v.end()};

    REQUIRE(a.GetNumDimensions() == 3);

    THEN("You print the vector as its values surrounded by [ ]") {
      std::cout << a;

      std::stringstream ss;
      ss << a;
      REQUIRE(ss.str() == "[-2.5 1.3 0.9]");
    }
  }
}

/*
 * Combination cases:
 *  Multiple methods/operators are used in each of these scenarios
 */
SCENARIO("Comparing identical and different vectors") {
  WHEN("You create three different vectors with different constructors") {
    std::vector<double> vec{3, 7, 5, 8};
    EuclideanVector a{vec.begin(), vec.end()};

    EuclideanVector b{};

    EuclideanVector c{3, 99};
    c[0] = -4;
    c.at(2) = 61;

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(b != c);

    THEN("You use the copy assignment and constructor to make them identical") {
      b = a;
      EuclideanVector a_copy{a};
      c = a_copy;

      REQUIRE(b == a);
      REQUIRE(c == a);
    }
  }
}
