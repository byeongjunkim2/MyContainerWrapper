#pragma once
#include <iterator>
#include <vector>
#include <span>
#include <concepts>


namespace mc
{
    // concepts

    // checks whether + operator exists
    template<typename T>
    concept IsAddable = requires(const T & a, const T & b) { a + b; };

    // checks whether * operator exists
    template<typename T>
    concept IsMultipliable = requires(const T & a, const T & b) { a* b; };

    // checks whether / operator exists
    template<typename T>
    concept IsDividable = requires(const T & a, const T & b) { a / b; };

    ///////////////



    template <typename T>
    class MyContainerWrapper {
    public:
        template <typename Container>
        MyContainerWrapper(Container& container) : data_observer{ std::data(container), std::size(container) }
        {
            Pre_Calculate();
        }

        // for raw pointer
        MyContainerWrapper(T* data, std::size_t n) : data_observer{ data, n }
        {
            Pre_Calculate();
        }

        template<typename U = T>
            requires IsAddable<U>
        U RangeSum(std::size_t start, std::size_t end) const
        {
            // start > end
            if (start > end)
            {
                throw std::out_of_range("start cannot be bigger than end");
            }

            size_t const containerSize{ data_observer.size() };
            if (containerSize == 0)
            {
                return 0;
            }

            // out of range > size
            if (start >= containerSize || end >= containerSize)
            {
                throw std::invalid_argument("start or end should be bigger than the array\'s size");
            }

            if (start == 0)
            {
                return result_sum[end];
            }
            else
            {
                return result_sum[end] - result_sum[start - 1];
            }
        }

        template<typename U = T>
            requires IsMultipliable<U>&& IsDividable<U>
        U RangeMult(std::size_t start, std::size_t end) const
        {
            // start > end
            if (start > end)
            {
                throw std::out_of_range("start cannot be bigger than end");
            }

            size_t const containerSize{ data_observer.size() };
            if (containerSize == 0)
            {
                return 0;
            }

            // out of range > size
            if (start >= containerSize || end >= containerSize)
            {
                throw std::invalid_argument("start or end should be bigger than the array\'s size");
            }

            if (start == 0)
            {
                return result_multi[end];
            }
            else
            {
                if(isZeroExist == true)
                {
                    return U{ 0 };
                }
                return result_multi[end] / result_multi[start - 1];
            }
        }

        T& operator[](size_t index)
        {
            // do not care about the out of range, since this operator just provides random access.
            // out of range error warning's responsibility is due to for the user.
            return data_observer[index];
        }

        void Refresh()
        {
            Pre_Calculate();
        }

    private:
        void Pre_Calculate()
        {
            const size_t spanSize{ data_observer.size() };
            isZeroExist = false;

            // has add operator?
            if (IsAddable<T>) {
                result_sum.resize(spanSize);
                result_sum[0] = data_observer[0];
                for (size_t i = 1; i < spanSize; i++)
                {
                    result_sum[i] = result_sum[i - 1] + data_observer[i];
                }
            }

            // has multiply & divide operator?
            if (IsMultipliable<T> && IsDividable<T>) {
                result_multi.resize(spanSize);
                result_multi[0] = data_observer[0];

                // zero exist?
                if(std::is_arithmetic_v<T>)
                {
	                if(data_observer[0] == T{0})
	                {
                        isZeroExist = true;
	                }
                }
                for (size_t i = 1; i < spanSize; i++)
                {
                    result_multi[i] = result_multi[i - 1] * data_observer[i];

                    if (std::is_arithmetic_v<T>)
                    {
                        if (data_observer[i] == T{ 0 })
                        {
                            isZeroExist = true;
                        }
                    }
                }
            }
        }

        std::span<T> data_observer; // container viewer using std::span
        std::vector<T> result_sum; // pre-calculated sums
        std::vector<T> result_multi; // pre-calculated multiplies
        bool isZeroExist = false;
    };


    // CTAD guide
    // for contiguous container such as vector, std::array..
    template<std::ranges::contiguous_range Range>
    MyContainerWrapper(const Range&) -> MyContainerWrapper<std::ranges::range_value_t<Range>>;

    // others
    template<typename T>
    MyContainerWrapper(const T*, std::size_t) -> MyContainerWrapper<T>;
}