#include "Eigen/Dense"
#include "gtest/gtest.h"

namespace Eigen {

TEST(EigenTest, CanCompile) {
  MatrixXd m(2, 2);
  m(0, 0) = 3;
  m(1, 0) = 2.5;
  m(0, 1) = -1;
  m(1, 1) = m(1, 0) + m(0, 1);

  EXPECT_DOUBLE_EQ(1.5, m(1, 1));
}

TEST(EigenTest, CanUseVectorOfFloats) {
  Vector2f v{1.f, 2.f};

  EXPECT_FLOAT_EQ(1.f, v[0]);
}

}  // namespace Eigen
