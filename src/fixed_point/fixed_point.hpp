#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <iostream>
#include <limits>

namespace fp
{

/// @brief Concept for integer types.
template<typename T>
concept Integral = std::is_integral_v<T>;

/// @brief Concept: T1 and T2 have the same signedness
template<typename T1, typename T2>
concept SameSignedness = (std::is_signed_v<T1> == std::is_signed_v<T2>);

/// @brief Concept: T1 is strictly larger in size than T2
template<typename T1, typename T2>
concept LargerThan = (sizeof(T1) > sizeof(T2));

/// @brief Concept: NumIntBits must be less than the number of value bits in T
template<typename T, std::size_t NumIntBits>
concept ValidIntBits = (NumIntBits < std::numeric_limits<T>::digits);

/// @brief Concept: T must not exceed 64 bits
template<typename T>
concept Max64Bits = (std::numeric_limits<T>::digits <= 64);

/**
 * @brief A fixed-point number class template. Supports both signed and unsigned integer types.
 * 
 * @tparam IntType The base integer type to use (e.g., std::int32_t, std::uint32_t).
 * @tparam WideType The integer type to perform intermediate calculations.
 * @tparam NumIntBits Number of bits to use for the integer part.
 */
template<Integral IntType, Integral WideType, std::size_t NumIntBits>
requires SameSignedness<IntType, WideType> && LargerThan<WideType, IntType> && ValidIntBits<IntType, NumIntBits> && Max64Bits<WideType>
class Number
{
public:
    // variables describing the fixed point number representation
    static constexpr bool kIsSigned {std::is_signed_v<IntType>};
    static constexpr std::size_t kNumBits {sizeof(IntType) * 8};
    static constexpr std::size_t kNumFracBits {kNumBits - NumIntBits};
    static constexpr IntType kScaleFactor {static_cast<IntType>(static_cast<WideType>(1) << kNumFracBits)};

    // getter for integer part bits
    [[nodiscard]] static constexpr IntType IntMask() noexcept
    {
        return ((static_cast<IntType>(1) << NumIntBits) - 1) << kNumFracBits;
    }

    // getter for fractional part bits
    [[nodiscard]] static constexpr IntType FracMask() noexcept
    {
        return (static_cast<IntType>(1) << kNumFracBits) - 1;
    }

    // factory method for number construction
    [[nodiscard]] static constexpr Number FromBits(IntType raw) noexcept
    {
        Number result;
        result.value_ = raw;
        return result;
    }

    // constants
    static constexpr Number Zero() noexcept
    {
        return FromBits(0);
    }

    static constexpr Number Half() noexcept
    {
        return FromBits(static_cast<IntType>(1) << (kNumFracBits - 1));
    }

    static constexpr Number PosOne() noexcept
    {
        return FromBits(kScaleFactor);
    }

    static constexpr Number NegOne() noexcept
    {
        static_assert(kIsSigned, "NegOne() doesn't exist for unsigned fixed point types");
        return FromBits(-kScaleFactor);
    }

    // default constructor
    constexpr explicit Number() noexcept : value_{0} {};

    // constructor from float
    constexpr explicit Number(float f) noexcept: value_{static_cast<IntType>(f * kScaleFactor)} {};

    // constructor from double
    constexpr explicit Number(double d) noexcept: value_{static_cast<IntType>(d * kScaleFactor)} {};

    // constructor from int
    template<std::integral T>
    constexpr explicit Number(T i) noexcept : value_{static_cast<IntType>(static_cast<WideType>(i) << kNumFracBits)} {}

    // getter for integer part
    [[nodiscard]] constexpr IntType IntPart() const noexcept
    {
        return value_ >> kNumFracBits;
    }

    // getter for fractional part
    [[nodiscard]] friend constexpr auto FracPart(const Number& a) noexcept
    {
        return FromBits(Abs(a).value_ & FracMask());
    }

    // conversion to float 
    [[nodiscard]] constexpr explicit operator float() const noexcept
    {
        return static_cast<float>(value_) / kScaleFactor;
    }

    // conversion to double
    [[nodiscard]] constexpr explicit operator double() const noexcept
    {
        return static_cast<double>(value_) / kScaleFactor;
    }
    
    // negation operator
    [[nodiscard]] constexpr Number operator-() const noexcept
    {
        static_assert(kIsSigned, "Negation operator is not supported for unsigned fixed point types");
        return FromBits(-value_);
    }

    // addition operator
    [[nodiscard]] constexpr Number operator+(const Number & other) const noexcept
    {
        return FromBits(value_ + other.value_);
    }

    // subtraction operator
    [[nodiscard]] constexpr Number operator-(const Number & other) const noexcept
    {
        return FromBits(value_ - other.value_);
    }

    // multiplication operator
    [[nodiscard]] constexpr Number operator*(const Number & other) const noexcept
    {
        const auto this_val = static_cast<WideType>(value_);
        const auto other_val = static_cast<WideType>(other.value_);
        return FromBits(static_cast<IntType>((this_val * other_val) >> kNumFracBits));
    }

    // division operator
    [[nodiscard]] constexpr Number operator/(const Number& other) const noexcept 
    {
        const auto this_wide_val = static_cast<WideType>(value_) << kNumFracBits;
        const auto other_wide_val = static_cast<WideType>(other.value_);
        return FromBits(static_cast<IntType>(this_wide_val / other_wide_val));
    }

    // increment operator
    constexpr Number& operator+=(const Number & other) noexcept
    {
        value_ += other.value_;
        return *this;
    }

    // decrement operator
    constexpr Number& operator-=(const Number & other) noexcept
    {
        value_ -= other.value_;
        return *this;
    }

    // multiplication assignment operator
    constexpr Number& operator*=(const Number & other) noexcept
    {
        const auto this_wide_val = static_cast<WideType>(value_);
        const auto other_wide_val = static_cast<WideType>(other.value_);
        value_ = static_cast<IntType>((this_wide_val * other_wide_val) >> kNumFracBits);
        return *this;
    }

    // division assignment operator
    constexpr Number& operator/=(const Number & other) noexcept
    {
        const auto this_wide_val = static_cast<WideType>(value_) << kNumFracBits;
        const auto other_wide_val = static_cast<WideType>(other.value_);
        value_ = static_cast<IntType>(this_wide_val / other_wide_val);
        return *this;
    }

    // spaceship operator handles all operations
    [[nodiscard]] constexpr auto operator<=>(const Number & other) const noexcept = default;

    // equality comparison
    [[nodiscard]] constexpr bool operator==(const Number & other) const noexcept = default;

    // sign bit
    [[nodiscard]] static constexpr bool SignBit(const Number & a) noexcept
    {
        if constexpr (kIsSigned)
        {
            return (a.value_ >> (kNumBits - 1)) & 1;
        }
        else
        {
            // unsigned types are always positive
            return false;
        }
    }

    // sign
    [[nodiscard]] static constexpr Number Sign(const Number & a) noexcept
    {
        if constexpr (kIsSigned)
        {
            return SignBit(a) ? NegOne() : PosOne();
        }
        else
        {
            // unsigned types are always positive (Except zero)
            return a.value_ == 0 ? Zero() : PosOne();
        }
    }

    // abs function
    friend constexpr auto Abs(const Number& a) noexcept
    {
        if constexpr (kIsSigned)
        {
            const auto raw_val = static_cast<WideType>(a.value_);
            const auto abs_val = SignBit(a) ? -raw_val : raw_val;
            return FromBits(static_cast<IntType>(abs_val));
        }
        else
        {
            // for unsigned types abs is identity
            return a;
        }
    }

private:
    IntType value_;
};

// Stream operator for convenient printing
template <Integral IntType, Integral WideType, size_t NumIntBits>
std::ostream& operator<<(std::ostream& os, const Number<IntType, WideType, NumIntBits>& fp) 
{
    os << static_cast<double>(fp);
    return os;
}

};  // namespace FP