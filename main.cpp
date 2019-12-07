#include <gtest/gtest.h>

class TRange {
    int64_t first, last, step;
public:
    TRange(int64_t N, int64_t M, int64_t s): first(N), last(M), step(s)
    {
        if (0 == s)
            throw std::invalid_argument("zero value of s aka step");
        if (s > 0 && M <= N)
            throw std::invalid_argument("M must be greater N for positive value of s");
        if (s < 0 && M >= N)
            throw std::invalid_argument("M must be less N for negative value of s");
    }
};

TEST(TRangeTests, Constructor) {
    EXPECT_NO_THROW(TRange(0, 10, 1));
    EXPECT_NO_THROW(TRange(1, -10, -1));
    EXPECT_THROW(TRange(1, -10, 0), std::invalid_argument);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
