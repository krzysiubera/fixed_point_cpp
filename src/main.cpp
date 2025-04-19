#include <cstdint>
#include <iostream>
#include <type_traits>
#include <concepts>
#include <format>

namespace fp
{

/// @brief Concept for integer types.
template<typename T>
concept Integral = std::is_integral_v<T>;

/**
 * @brief A fixed-point number class template. Supports both signed and unsigned integer types.
 * 
 * @tparam IntType The base integer type to use (e.g., std::int32_t, std::uint32_t).
 * @tparam IntBits Number of bits to use for the integer part.
 */
template<Integral IntType, std::size_t IntBits>
class Number
{
public:
    // for now assume that std::int32_t/std::uint32_t are max possible IntType
    using XLType = std::conditional_t<std::is_signed_v<IntType>, std::int64_t, std::uint64_t>;

    // variables describing the fixed point number representation
    static constexpr bool kIsSigned {std::is_signed_v<IntType>};
    static constexpr std::size_t kNumBits {sizeof(IntType) * 8};
    static constexpr std::size_t kFracBits {kNumBits - IntBits};
    static constexpr IntType kScaleFactor {static_cast<IntType>(static_cast<XLType>(1) << kFracBits)};
    static constexpr IntType kIntMask {((static_cast<IntType>(1) << IntBits) - 1) << kFracBits};
    static constexpr IntType kFracMask {(static_cast<IntType>(1) << kFracBits) - 1};

    // factory method for number construction
    [[nodiscard]] static constexpr Number FromRaw(IntType raw) noexcept
    {
        Number result;
        result.value_ = raw;
        return result;
    }

    // constants
    static constexpr Number Zero() noexcept
    {
        return FromRaw(0);
    }

    static constexpr Number Half() noexcept
    {
        return FromRaw(static_cast<IntType>(1) << (kFracBits - 1));
    }

    static constexpr Number PosOne() noexcept
    {
        return FromRaw(kScaleFactor);
    }

    static constexpr Number NegOne() noexcept
    {
        static_assert(kIsSigned, "NegOne() doesn't exist for unsigned fixed point types");
        return FromRaw(-kScaleFactor);
    }

    // default constructor
    constexpr explicit Number() noexcept : value_{0} {};

    // constructor from float
    constexpr explicit Number(float f) noexcept: value_{static_cast<IntType>(f * kScaleFactor)} {};

    // constructor from double
    constexpr explicit Number(double d) noexcept: value_{static_cast<IntType>(d * kScaleFactor)} {};

    // constructor from int
    template<std::integral T>
    constexpr Number(T i) noexcept : value_{static_cast<IntType>(i) << kFracBits} {}

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
        return FromRaw(-value_);
    }

    // addition operator
    [[nodiscard]] constexpr Number operator+(const Number & other) const noexcept
    {
        return FromRaw(value_ + other.value_);
    }

    // subtraction operator
    [[nodiscard]] constexpr Number operator-(const Number & other) const noexcept
    {
        return FromRaw(value_ - other.value_);
    }

    // multiplication operator
    [[nodiscard]] constexpr Number operator*(const Number & other) const noexcept
    {
        const auto this_val = static_cast<XLType>(value_);
        const auto other_val = static_cast<XLType>(other.value_);
        return FromRaw(static_cast<IntType>((this_val * other_val) >> kFracBits));
    }

    // division operator
    [[nodiscard]] constexpr Number operator/(const Number& other) const noexcept 
    {
        const auto this_xl_val = static_cast<XLType>(value_) << kFracBits;
        const auto other_xl_val = static_cast<XLType>(other.value_);
        return FromRaw(static_cast<IntType>(this_xl_val / other_xl_val));
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
        const auto this_xl_val = static_cast<XLType>(value_);
        const auto other_xl_val = static_cast<XLType>(other.value_);
        value_ = static_cast<IntType>((this_xl_val * other_xl_val) >> kFracBits);
        return *this;
    }

    // division assignment operator
    constexpr Number& operator/=(const Number & other) noexcept
    {
        const auto this_xl_val = static_cast<XLType>(value_) << kFracBits;
        const auto other_xl_val = static_cast<XLType>(other.value_);
        value_ = static_cast<IntType>(this_xl_val / other_xl_val);
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
            return FromRaw(SignBit(a) ? -a.value_ : a.value_);
        }
        else
        {
            // for unsigned types abs is identity
            return a;
        }
    }

    // fractional part
    friend constexpr auto Frac(const Number& a) noexcept
    {
        return FromRaw(Abs(a).value_ & kFracMask);
    }

private:
    IntType value_;
};

using S32_14 = Number<std::int32_t, 14>;
using U32_14 = Number<std::uint32_t, 14>;

// Stream operator for convenient printing
template <Integral IntType, size_t IntBits>
std::ostream& operator<<(std::ostream& os, const Number<IntType, IntBits>& fp) 
{
    os << static_cast<double>(fp);
    return os;
}

};  // namespace FP

int main()
{
    // test signed fixed-point
    using FPS = fp::S32_14;

    FPS a(42.5F);
    std::cout << "signed a = " << a << '\n';

    FPS b(-42);
    std::cout << "signed b = " << b << '\n';

    // basic arithmetic with signed values
    FPS c = a + b;
    std::cout << "signed a + b = " << c << '\n';

    FPS d = Abs(b);
    std::cout << "signed abs(b) = " << d << '\n';

    // test unsigned fixed point
    using FPU = fp::U32_14;
    FPU e(42.5F);
    std::cout << "unsigned e = " << e << '\n';

    // test multiplication
    FPU g = e * FPU(2.5f);
    std::cout << "Unsigned e * 2.5 = " << g << '\n';
    
    // ADC example, with unsigned type
    const FPU ref_voltage {3.3f};           // Reference voltage
    const FPU adc_max = 4095;               // 12-bit ADC (0-4095)
    uint32_t adc_reading = 3185;
    
    FPU adc_fp = adc_reading;               // ADC reading as fixed point
    FPU real_voltage = (adc_fp / adc_max) * ref_voltage;
    std::cout << "ADC reading = " << adc_reading << std::endl;
    std::cout << "Real voltage = " << real_voltage << " V" << std::endl;

    return 0;
}