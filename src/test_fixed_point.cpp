#include "fixed_point.hpp"

using FP_S32_16 = fp::Number<std::int32_t, std::int64_t, 16>;
using FP_U32_16 = fp::Number<std::uint32_t, std::uint64_t, 16>;

// compile-time test cases
constexpr bool TestConstructionSignedFromInt()
{
    auto a = FP_S32_16(3);
    return a.IntPart() == 3;
}

constexpr bool TestConstructionUnsignedFromInt()
{
    auto a = FP_U32_16(3);
    return a.IntPart() == 3;
}

constexpr bool TestConstructionSignedFromFloat()
{
    auto a = FP_S32_16(-1.5f);
    return static_cast<float>(a) > -1.6f && static_cast<float>(a) < -1.4f;
}

constexpr bool TestConstructionUnsignedFromFloat()
{
    auto a = FP_U32_16(1.5f);
    return static_cast<float>(a) > 1.4f && static_cast<float>(a) < 1.6f;
}

constexpr bool TestNegationSigned()
{
    auto a = FP_S32_16(5);
    auto b = -a;
    return b.IntPart() == -5;
}

constexpr bool TestAddition()
{
    auto a = FP_S32_16(2.5);
    auto b = FP_S32_16(1.25);
    auto c = a + b;
    return static_cast<float>(c) > 3.7f && static_cast<float>(c) < 3.8f;
}

constexpr bool TestSubtraction()
{
    auto a = FP_S32_16(2.5);
    auto b = FP_S32_16(1.25);
    auto c = a - b;
    return static_cast<float>(c) > 1.2f && static_cast<float>(c) < 1.3f;
}

constexpr bool TestMultiplication()
{
    auto a = FP_S32_16(2.5);
    auto b = FP_S32_16(1.25);
    auto c = a * b;
    return static_cast<float>(c) > 3.1f && static_cast<float>(c) < 3.2f;
}

constexpr bool TestDivision()
{
    auto a = FP_S32_16(2.5);
    auto b = FP_S32_16(1.25);
    auto c = a / b;
    return static_cast<float>(c) > 1.9f && static_cast<float>(c) < 2.1f;
}

constexpr bool TestFractionalPart()
{
    auto a = FP_S32_16(3.75);
    auto f = FracPart(a);
    return static_cast<float>(f) > 0.74f && static_cast<float>(f) < 0.76f;
}

constexpr bool TestAbsSigned()
{
    auto x = FP_S32_16(-10);
    return Abs(x) == FP_S32_16(10);
}

constexpr bool TestAbsUnsigned()
{
    auto x = FP_U32_16(10);
    return Abs(x) == x;
}

constexpr bool TestSignBitSignedPositive()
{
    return FP_S32_16::SignBit(FP_S32_16(1)) == false;
}

constexpr bool TestSignBitSignedNegative()
{
    return FP_S32_16::SignBit(FP_S32_16(-1)) == true;
}

constexpr bool TestSignBitUnsigned()
{
    return FP_U32_16::SignBit(FP_U32_16(1)) == false;
}

constexpr bool TestSignUnsignedPositive()
{
    return FP_U32_16::Sign(FP_U32_16(1)) == FP_U32_16::PosOne();
}

constexpr bool TestSignUnsignedZero()
{
    return FP_U32_16::Sign(FP_U32_16(0)) == FP_U32_16::Zero();
}

constexpr bool TestSignSignedPositive()
{
    return FP_S32_16::Sign(FP_S32_16(123)) == FP_S32_16::PosOne();
}

constexpr bool TestSignSignedNegative()
{
    return FP_S32_16::Sign(FP_S32_16(-123)) == FP_S32_16::NegOne();
}

// compile-time execution
static_assert(TestConstructionSignedFromInt(), "Construction of signed FP number from int failed");
static_assert(TestConstructionUnsignedFromInt(), "Construction of unsigned FP number from int failed");
static_assert(TestConstructionSignedFromFloat(), "Construction of signed FP number from float failed");
static_assert(TestConstructionUnsignedFromFloat(), "Construction of unsigned FP number from float failed");
static_assert(TestNegationSigned(), "Signed negation failed");
static_assert(TestAddition(), "Addition failed");
static_assert(TestSubtraction(), "Subtraction failed");
static_assert(TestMultiplication(), "Multiplication failed");
static_assert(TestDivision(), "Division failed");
static_assert(TestFractionalPart(), "FracPart() failed");
static_assert(TestAbsSigned(), "Signed Abs() failed");
static_assert(TestAbsUnsigned(), "Unsigned Abs() failed");
static_assert(TestSignBitSignedPositive(), "SignBit() with a signed positive integer failed");
static_assert(TestSignBitSignedNegative(), "SignBit() with a signed negative integer failed");
static_assert(TestSignBitUnsigned(), "SignBit() with an unsigned integer failed");
static_assert(TestSignUnsignedPositive(), "Sign() with a positive unsigned integer failed");
static_assert(TestSignUnsignedZero(), "Sign() with a zero unsigned integer failed");
static_assert(TestSignSignedPositive(), "Sign() with a positive signed integer failed");
static_assert(TestSignSignedNegative(), "Sign() with a negative signed integer failed");

int main()
{
    return 0;
}