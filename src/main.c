#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// representing the number in fp_int_t
#define FP_INT_TYPE         int32_t
#define FP_INT_XL_TYPE      int64_t

typedef FP_INT_TYPE         fp_int_t;
typedef FP_INT_XL_TYPE      fp_int_xl_t;

// FP_INT_BITS lets us represent values in range:
// - (1 << FP_INT_BITS) for unsigned values
// - (1 << FP_INT_BITS) / 2 for signed values
#define FP_NUM_BITS         (sizeof(fp_int_t) * 8)
#define FP_INT_BITS         (14)
#define FP_FRAC_BITS        (FP_NUM_BITS - FP_INT_BITS)

#define FP_SCALE            (FP_FRAC_BITS)
#define FP_SCALE_FACTOR     (1 << FP_SCALE)

#define FP_INT_MASK         (((1 << FP_INT_BITS) - 1) << FP_FRAC_BITS)
#define FP_FRAC_MASK        ((1 << FP_FRAC_BITS) - 1)

#define FP_FROM_INT(a)      (a << FP_SCALE)
#define FP_VAL_ZERO         0
#define FP_VAL_HALF         (1 << (FP_FRAC_BITS - 1))
#define FP_VAL_POS_ONE      FP_FROM_INT(1)
#define FP_VAL_NEG_ONE      FP_FROM_INT(-1)

// returns 1 if positive, -1 if negative number
#define FP_SIGN_BIT(a)      ((a >> (FP_NUM_BITS - 1)) & 1)
#define FP_SIGN(a)          (FP_SIGN_BIT(a) == FP_VAL_ZERO ? FP_VAL_POS_ONE : FP_VAL_NEG_ONE)

float fp_to_float(fp_int_t a)
{
    return (float) a / FP_SCALE_FACTOR;
}

fp_int_t float_to_fp(float a)
{
    return (fp_int_t)(a * FP_SCALE_FACTOR);
}

fp_int_t fp_mul(fp_int_t a, fp_int_t b)
{
    return ((fp_int_xl_t)a * (fp_int_xl_t)b) >> FP_SCALE;
}

fp_int_t fp_div(fp_int_t a, fp_int_t b)
{
    // first we scale a to avoid losing precision
    return ((fp_int_xl_t)(a) << FP_SCALE) / b;
}

fp_int_t fp_abs(fp_int_t a)
{
    return (FP_SIGN_BIT(a) == 0) ? a : (~a + 1);
}

fp_int_t fp_frac(fp_int_t a)
{
    return fp_abs(a) & FP_FRAC_MASK;
}

fp_int_t fp_floor(fp_int_t a)
{
    fp_int_t frac = fp_frac(a);
    if (frac == 0)
    {
        return a;
    }

    return (FP_SIGN_BIT(a) == 0) ? (FP_INT_MASK & a) : ((FP_INT_MASK & a) + FP_VAL_POS_ONE);
}

fp_int_t fp_ceil(fp_int_t a)
{
    fp_int_t frac = fp_frac(a);
    return (frac == 0) ? a : (fp_floor(a) + FP_SIGN(a));
}

fp_int_t fp_round(fp_int_t a)
{
    fp_int_t frac = fp_frac(a);
    return (frac >= FP_VAL_HALF) ? fp_ceil(a) : fp_floor(a);
}


int main()
{
    fp_int_t a = float_to_fp(42.5F);
    printf("a = %f\n", fp_to_float(a));

    fp_int_t b = FP_FROM_INT(-42);
    printf("b = %f\n", fp_to_float(b));

    fp_int_t c = FP_FROM_INT(45);
    printf("c = %f\n", fp_to_float(c));


    fp_int_t d = float_to_fp(42.5F);
    fp_int_t e = float_to_fp(-18.2F);
    fp_int_t g = d + e;
    printf("g = %f\n", fp_to_float(g));

    fp_int_t h = d - e;
    printf("h = %f\n", fp_to_float(h));

    fp_int_t i = fp_mul(d, e);
    printf("i = %f\n", fp_to_float(i));

    fp_int_t j = fp_div(d, e);
    printf("j = %f\n", fp_to_float(j));

    fp_int_t k = fp_abs(e);
    printf("k = %f\n", fp_to_float(k));

    fp_int_t m = fp_abs(-e);
    printf("m = %f\n", fp_to_float(m));

    fp_int_t n = fp_floor(d);
    fp_int_t o = fp_floor(e);
    printf("n = %f\n", fp_to_float(n));
    printf("o = %f\n", fp_to_float(o));

    fp_int_t p = fp_frac(d);
    fp_int_t q = fp_frac(e);
    printf("p = %f\n", fp_to_float(p));
    printf("q = %f\n", fp_to_float(q));

    fp_int_t r = fp_ceil(d);
    fp_int_t s = fp_ceil(e);
    printf("r = %f\n", fp_to_float(r));
    printf("s = %f\n", fp_to_float(s));

    fp_int_t t = fp_ceil(float_to_fp(42.0F));
    fp_int_t u = fp_ceil(float_to_fp(18.0f));
    printf("t = %f\n", fp_to_float(t));
    printf("u = %f\n", fp_to_float(u));

    fp_int_t v = fp_round(float_to_fp(-18.0F));
    fp_int_t w = fp_round(float_to_fp(-18.2F));
    fp_int_t x = fp_round(float_to_fp(-18.5F));
    fp_int_t y = fp_round(float_to_fp(-18.6F));
    printf("v = %f\n", fp_to_float(v));
    printf("w = %f\n", fp_to_float(w));
    printf("x = %f\n", fp_to_float(x));
    printf("y = %f\n", fp_to_float(y));
    
    fp_int_t aa = fp_round(float_to_fp(18.0F));
    fp_int_t bb = fp_round(float_to_fp(18.2F));
    fp_int_t cc = fp_round(float_to_fp(18.5F));
    fp_int_t dd = fp_round(float_to_fp(18.6F));
    printf("aa = %f\n", fp_to_float(aa));
    printf("bb = %f\n", fp_to_float(bb));
    printf("cc = %f\n", fp_to_float(cc));
    printf("dd = %f\n", fp_to_float(dd));

    // ADC 12 bits (0-16383) - 16384 total values
    // analog reference voltage is 3.3 V. meaning 0 V -> 0, 3.3V -> 16383
    const fp_int_t ref_voltage = float_to_fp(3.3F);
    const fp_int_t adc_max = FP_FROM_INT(4095);
    int32_t adc_reading = 3185;

    fp_int_t adc_fp = FP_FROM_INT(adc_reading);
    fp_int_t real_voltage = fp_mul(fp_div(adc_fp, adc_max), ref_voltage);
    printf("real voltage = %f V \n", fp_to_float(real_voltage));
    return 0;
}