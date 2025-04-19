#include <iostream>
#include "fixed_point.hpp"

int main()
{
    using FP_S32_14 = fp::Number<std::int32_t, 14>;
    using FP_U32_14 = fp::Number<std::uint32_t, 14>;

    // type aliases for convenience
    using FPS = FP_S32_14;
    using FPU = FP_U32_14;

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