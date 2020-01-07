#include <gtest/gtest.h> // заголовочный файл Google Test
#include "myproject/4/Matrix.h"

struct ExpectSymmetric{
	Matrix<int, 3, 3> input;
	bool exOutput;
};

class symm_test: public ::testing::TestWithParam<ExpectSymmetric> {};

struct ExpectWhere{
	Matrix<int, 3, 3> input1;
    Matrix<int, 3, 3> input2;
	Matrix<int, 3, 3> exOutput;
};

class where_test: public ::testing::TestWithParam<ExpectWhere> {};

TEST_P(symm_test, _){
	const ExpectSymmetric& param = GetParam();
    auto ans = param.input.is_symmetric();
	EXPECT_EQ(param.exOutput, ans);
}

INSTANTIATE_TEST_CASE_P(
        _,
        symm_test,
        ::testing::Values(
                ExpectSymmetric{
                    {1,1,1, 1,1,1, 1,1,1},
                    1
                },
                ExpectSymmetric{
                    {1,2,3, 4,5,6, 7,8,9},
                    0
                }
                )
);

TEST_P(where_test, _){
    const ExpectWhere& param = GetParam();
    EXPECT_EQ(param.exOutput, where(param.input1 < param.input2, param.input1, param.input2));
}

INSTANTIATE_TEST_CASE_P(
        _,
        where_test,
        ::testing::Values(
        ExpectWhere{
                {1,1,1, 1,1,1, 1,1,1},
                {0,2,0, 2,0,2, 0,2,0},
                {0,1,0, 1,0,1, 0,1,0}
        },
        ExpectWhere{
                {1,2,3, 4,5,6, 7,8,9},
                {9,8,7, 6,5,4, 3,2,1},
                {1,2,3, 4,5,4, 3,2,1}
        }
)
);
