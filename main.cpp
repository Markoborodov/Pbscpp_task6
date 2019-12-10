#include <gtest/gtest.h>
#include <limits>
#include <unordered_set>

class TRange {
    int64_t first, last, step;
    uint64_t size;
public:
    TRange(int64_t N, int64_t M, int64_t s): first(N), last(M), step(s)
    {
        if (0 == s)
            throw std::invalid_argument("Zero value of s");
        if (s > 0 && M <= N)
            throw std::invalid_argument("M must be greater N for positive value of s");
        if (s < 0 && M >= N)
            throw std::invalid_argument("M must be less N for negative value of s");

        if (s > 0)
            size = static_cast<uint64_t>(last - first) / static_cast<uint64_t>(step) + 1;
        else
            size = static_cast<uint64_t>(first - last) / static_cast<uint64_t>(-step) + 1;

        if (0 == size) throw std::invalid_argument("Range too large");
    }

    std::optional<int64_t> operator[](uint64_t i) const
    {
        if (i >= size) return std::nullopt;
        return first + static_cast<int64_t>(step * i);
    }

    uint64_t GetSize() const
    {
        return size;
    }

    class const_iterator {
        const TRange* range;
        uint64_t current_index;
        uint64_t size;

        explicit
        const_iterator(const TRange* _range, uint64_t i = 0)
        :range(_range), current_index(i)
        {
            size = _range->GetSize();
            if (current_index > size)
                current_index = size;
        }

    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef int64_t value_type;
        typedef uint64_t difference_type;
        typedef int64_t* pointer;
        typedef int64_t& reference;

        const_iterator(): range(nullptr), current_index(0), size(0) {}

        const_iterator(const const_iterator& it)
        {
            range = it.range;
            current_index = it.current_index;
            size = it.size;
        }

        const_iterator& operator=(const const_iterator& it)
        {
            if (this == &it) return *this;
            range = it.range;
            current_index = it.current_index;
            size = it.size;
            return *this;
        }

        bool operator==(const const_iterator& rhs) const
        {
            return range == rhs.range
                   && current_index == rhs.current_index
                   && size == rhs.size;
        }

        bool operator!=(const const_iterator& rhs) const
        {
            return !(rhs == *this);
        }

        std::optional<int64_t> operator*() const
        {
            if (size == current_index) return std::nullopt;
            return (*range)[current_index];
        }

        friend const const_iterator& operator++(const_iterator& it)
        {
            if (it.current_index == it.size)
                throw std::overflow_error("Incrementing iterator of end");
            it.current_index++;
            return it;
        }

        friend const const_iterator operator++(const_iterator& it, int)
        {
            const_iterator tmp(it);
            if (it.current_index == it.size)
                throw std::overflow_error("Incrementing iterator of end");
            it.current_index++;
            return tmp;
        }

        friend class TRange;
    };

    const_iterator begin() const
    {
        return const_iterator(this);
    }

    const_iterator end() const
    {
        return const_iterator(this, size);
    }

    bool operator==(const TRange& rhs) const
    {
        return first == rhs.first
               && last == rhs.last
               && step == rhs.step;
    }

    friend class std::hash<TRange>;
};

template <>
class std::hash<TRange> {
public:
    size_t operator()(const TRange& range) const
    {
        size_t result = 2166136261;

        result = (result * 16777619) ^ static_cast<size_t>(range.first);
        result = (result * 16777619) ^ static_cast<size_t>(range.last);
        result = (result * 16777619) ^ static_cast<size_t>(range.step);

        return result;
    }
};

TEST(TRangeTests, Constructor) {
    EXPECT_NO_THROW(TRange(0, 10, 1));
    EXPECT_NO_THROW(TRange(1, -10, -1));

    EXPECT_THROW(TRange(1, -10, 0), std::invalid_argument);
    EXPECT_THROW(TRange(10, 0, 1), std::invalid_argument);
    EXPECT_THROW(TRange(-10, 1, -1), std::invalid_argument);
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

TEST(TRangeTests, GetSize) {
    TRange range_0(0, 10, 2);
    EXPECT_EQ(range_0.GetSize(), 6);

    TRange range_1(-6, -12, -3);
    EXPECT_EQ(range_1.GetSize(), 3);

    TRange range_2(0, 10, 3);
    EXPECT_EQ(range_2.GetSize(), 4);

    TRange range_3(1, -10, -5);
    EXPECT_EQ(range_3.GetSize(), 3);

    TRange range_4(1, -10, -20);
    EXPECT_EQ(range_4.GetSize(), 1);

    TRange range_5(1, 10, 20);
    EXPECT_EQ(range_5.GetSize(), 1);
}

TEST(TRangeTests, WorstCase) {
    EXPECT_THROW(TRange(std::numeric_limits<int64_t>::min(),
            std::numeric_limits<int64_t>::max(), 1), std::invalid_argument);
    TRange range(std::numeric_limits<int64_t>::min() + 1,
            std::numeric_limits<int64_t>::max(), 1);

    EXPECT_EQ(range[0], std::numeric_limits<int64_t>::min() + 1);
    EXPECT_EQ(range[std::numeric_limits<int64_t>::max()], 0);

    EXPECT_EQ(range.GetSize(), std::numeric_limits<uint64_t>::max());
}

TEST(TRangeConstIteratorTests, Constructor) {
    TRange range_0(0, 10, 2);
    TRange range_1(0, 5, 2);

    TRange::const_iterator it_0;
    EXPECT_EQ(TRange::const_iterator(), it_0);

    TRange::const_iterator it_1(it_0);
    EXPECT_EQ(it_0, it_1);

    TRange::const_iterator it_2 = it_0;
    EXPECT_TRUE(it_0 == it_2);
    EXPECT_FALSE(it_0 != it_2);

    EXPECT_EQ(*it_0, std::nullopt);

    EXPECT_THROW(++it_0, std::overflow_error);
    EXPECT_THROW(it_0++, std::overflow_error);
}

TEST(TRangeTests, Iterator) {
    TRange range_0(0, 10, 3);
    TRange range_1(1, -10, -3);

    int64_t array_0[] = {0, 3, 6, 9};
    int64_t array_1[] = {1, -2, -5, -8};

    int i = 0;
    for (auto it = range_0.begin(); it != range_0.end(); it++) {
        EXPECT_EQ(*it, array_0[i]) << "i = " << i;
        i++;
    }

    i = 0;
    for (auto&& it : range_1) {
        EXPECT_EQ(it, array_1[i]) << "i = " << i;
        i++;
    }
}

TEST(TRangeTests, UnorderedSet) {
    std::unordered_set<TRange> set;

    TRange range_0(0, 10, 3);
    TRange range_1(1, -10, -3);

    set.insert(range_0);
    set.insert(range_1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
