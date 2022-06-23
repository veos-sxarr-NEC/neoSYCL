#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;

TEST(range, basic_test) {
  range<1> range_1d(1);
  range<2> range_2d(1, 2);
  range<3> range_3d(1, 2, 3);

  EXPECT_EQ(range_1d[0], 1);
  EXPECT_EQ(range_1d.get(0), 1);
  EXPECT_EQ(range_1d.size(), 1);

  EXPECT_EQ(range_2d[0], 1);
  EXPECT_EQ(range_2d[1], 2);
  EXPECT_EQ(range_2d.get(0), 1);
  EXPECT_EQ(range_2d.get(1), 2);
  EXPECT_EQ(range_2d.size(), 2);

  EXPECT_EQ(range_3d[0], 1);
  EXPECT_EQ(range_3d[1], 2);
  EXPECT_EQ(range_3d[2], 3);
  EXPECT_EQ(range_3d.get(0), 1);
  EXPECT_EQ(range_3d.get(1), 2);
  EXPECT_EQ(range_3d.get(2), 3);
  EXPECT_EQ(range_3d.size(), 6);
}

TEST(range, copy_test) {
  range<3> range_3d(1, 2, 3);
  range<3> cpy = range_3d;

  EXPECT_EQ(range_3d.size(), cpy.size());
  EXPECT_EQ(range_3d[0], cpy[0]);
  EXPECT_EQ(range_3d[1], cpy[1]);
  EXPECT_EQ(range_3d[2], cpy[2]);
  EXPECT_EQ(range_3d.get(0), cpy.get(0));
  EXPECT_EQ(range_3d.get(1), cpy.get(1));
  EXPECT_EQ(range_3d.get(2), cpy.get(2));
}