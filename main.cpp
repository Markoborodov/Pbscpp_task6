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

    std::optional<int64_t> operator[](int64_t i) const
    {
        if (i < 0) return std::nullopt;
        if (step < 0 && first + step * i < last) return std::nullopt;
        if (step > 0 && first + step * i > last) return std::nullopt;
        return first + step * i;
    }
};

TEST(TRangeTests, Constructor) {
    EXPECT_NO_THROW(TRange(0, 10, 1));
    EXPECT_NO_THROW(TRange(1, -10, -1));
    EXPECT_THROW(TRange(1, -10, 0), std::invalid_argument);
}

TEST(TRangeTests, OperatorIndex) {
    TRange range_0(0, 10, 2);
    EXPECT_EQ(range_0[-1], std::nullopt);
    EXPECT_EQ(range_0[0], 0);
    EXPECT_EQ(range_0[1], 2);
    EXPECT_EQ(range_0[2], 4);
    EXPECT_EQ(range_0[3], 6);
    EXPECT_EQ(range_0[4], 8);
    EXPECT_EQ(range_0[5], 10);
    EXPECT_EQ(range_0[6], std::nullopt);

    TRange range_1(-6, -12, -3);
    EXPECT_EQ(range_1[-1], std::nullopt);
    EXPECT_EQ(range_1[0], -6);
    EXPECT_EQ(range_1[1], -9);
    EXPECT_EQ(range_1[2], -12);
    EXPECT_EQ(range_1[3], std::nullopt);

    TRange range_2(0, 10, 3);
    EXPECT_EQ(range_2[-1], std::nullopt);
    EXPECT_EQ(range_2[0], 0);
    EXPECT_EQ(range_2[1], 3);
    EXPECT_EQ(range_2[2], 6);
    EXPECT_EQ(range_2[3], 9);
    EXPECT_EQ(range_2[4], std::nullopt);

    TRange range_3(1, -10, -5);
    EXPECT_EQ(range_3[-1], std::nullopt);
    EXPECT_EQ(range_3[0], 1);
    EXPECT_EQ(range_3[1], -4);
    EXPECT_EQ(range_3[2], -9);
    EXPECT_EQ(range_3[3], std::nullopt);

    TRange range_4(1, -10, -20);
    EXPECT_EQ(range_4[-1], std::nullopt);
    EXPECT_EQ(range_4[0], 1);
    EXPECT_EQ(range_4[1], std::nullopt);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
