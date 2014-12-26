/*****************************************************************************

    fft.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2007 Aleksey Bader

    The Arageli Library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    The Arageli Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

    We are also open for dual licensing for the whole library or
    for its particular part. If you are interested to get the library
    in this way, i.e. not under the GNU General Public License,
    please contact Arageli Support Service support.arageli@gmail.com.

*****************************************************************************/

/**
    \file fft.hpp
    \brief Fast Fourier Transformation over finite fild also called Number Theoretic Transform.


*/

#ifndef _ARAGELI_fft_hpp_
#define _ARAGELI_fft_hpp_

#include "config.hpp"
#include "bigar.hpp"
#include "exception.hpp"

namespace Arageli
{
// TODO: clarify if this approach speed up the algorithm
//#define BARRETT
#ifdef BARRETT
// precomputed constants for Barrett reduction
// p_inv = floor(4^k / p), where k --- number of bits for p number representation
const _Internal::digit p1_inv = 2454267021ul;
const _Internal::digit p2_inv = 429496727ul;
const _Internal::digit p3_inv = 1527099480ul;

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4244)
#endif

// Barrett reduction function return reduction of x modulo p
// here n = 4^{k+1}, where k --- number of bits for p number representation
template<_Internal::digit p, _Internal::digit p_inv, int n>
unsigned long barrett_reduction(unsigned long long x)
{
    unsigned long q = ((x >> (n-4))*p_inv)>> n;
    long long res = (x & ((1ll<<n)-1)) - (((unsigned long long)(q)*p) & ((1ll<<n)-1));
    if (res < 0) res += 1ll<<n;
    while(res >= p) res -= p;
    return res;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

// TODO: old code --- should be deleted!
#ifdef TEMPL
unsigned long barrett_reduction_p1(unsigned long long x)
{
    unsigned long q = ((x >> 28)*p1_inv)>>32;
    long long res = (x & 0xFFFFFFFF) - (((unsigned long long)(q)*pollard_p1) & 0xFFFFFFFF);
    if (res < 0) res += 0x100000000ll;
    while(res >= pollard_p1) res -= pollard_p1;
    return res;
}
unsigned long barrett_reduction_p2(unsigned long long x)
{
    unsigned long q = ((x >> 26)*p2_inv)>>30;
    long res = (x & 0x3FFFFFFF) - (((unsigned long long)(q)*pollard_p2) & 0x3FFFFFFF);
    if (res < 0) res += 0x40000000;
    while(res >= pollard_p2) res -= pollard_p2;
    return res;
}
unsigned long barrett_reduction_p3(unsigned long long x)
{
    unsigned long q = ((x >> 28)*p3_inv)>>32;
    long long res = (x & 0xFFFFFFFF) - (((unsigned long long)(q)*pollard_p3) & 0xFFFFFFFF);
    if (res < 0) res += 0x100000000ll;
    while(res >= pollard_p3) res -= pollard_p3;
    return res;
}
#endif
#endif

template <typename T>
struct doubled_type;

template <>
struct doubled_type<_Internal::digit>
{
    typedef _Internal::doubledigit d_value;
};

// WARNING!!! here we suppose that 32-bit computations are supported!!!
// TODO: make this code more portable.
const unsigned long k_max = ARAGELI_POLLARD_MAX_POWER;
const unsigned long n_max = ARAGELI_POLLARD_UPPER_BOUND;   // max length of (input vector ~ polynom)

/**
* \breif Class contains methods for FFT and inverse FFT calculation.
* \param ET --- type of elements for which we compute FFT
* \param MT --- modulo type in Z/p fied and type of vector length
*/
#ifdef BARRETT
template<typename ET, typename MT, _Internal::digit p_num, _Internal::digit p_inv, int b_n>
#else
template<typename ET, typename MT>
#endif
class CFFT
{
protected:
    MT n;                                   //< The maximal length of a vector for which we can calculate FFT.
    MT p;                                   //< The module of a finite field.
private:
    MT alpha;                               //< The primitive root of 2^k_Max power in finite field.
    MT **alpha_powers;                      //< The precomputed powers of alpha for FFT.
    MT *binary_reverse;                     //< Binary reverse indexes to calculate regular power of alpha.
    static const MT num_of_alpha_powers = k_max;

    typedef typename doubled_type<ET>::d_value doubled_value;

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4244)
#endif

    /// Find primitive root of 2^k_Max power.
    void alpha_determination(const MT p, const MT root)
    {
        alpha = 1;
        unsigned long j;
        for (j=1; j <= (p - 1) / n; ++j)
        {
#ifdef BARRETT
            //alpha = barrett_reduction<p_num, p_inv, b_n>(doubled_value(alpha)*root);
            alpha = doubled_value(alpha)*root > p ? barrett_reduction<p_num, p_inv, b_n>(doubled_value(alpha)*root): doubled_value(alpha)*root;
#else
            alpha = MT((doubled_value(alpha)*root) % p);
#endif
        }

        // Look at this code more thoroughly
        // It was invented for optimization reasons, but contains bugs!
        /*
        MT curr_root = root;
        MT curr_power = (p - 1) / n;
        while(curr_power)
        {
#ifdef BARRETT
                alpha = (doubled_value(alpha)*curr_root > p) ? barrett_reduction<p_num,p_inv,b_n>(doubled_value(alpha)*curr_root): doubled_value(alpha)*curr_root;
#else
                alpha = (doubled_value(alpha)*curr_root > p) ? (doubled_value(alpha)*curr_root) % p: doubled_value(alpha)*curr_root;
#endif
            curr_power >>= 1;
#ifdef BARRETT
            curr_root = (doubled_value(curr_root)*curr_root > p) ? barrett_reduction<p_num,p_inv,b_n>(doubled_value(curr_root)*curr_root) : doubled_value(curr_root)*curr_root;
#else
            curr_root = (doubled_value(curr_root)*curr_root > p) ? (doubled_value(curr_root)*curr_root) % p : doubled_value(curr_root)*curr_root;
#endif
        }
        */
    };

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

    /// Compute need powers of alpha.
    void alpha_power_computation(const MT p)
    {
        MT *curr_alpha_power;
        MT *prev_alpha_power;
        MT current_power = 1;
        long array_size, half_array_size;
        long j, i;
        // Size of array is (2^j + 1)
        array_size = (1 << num_of_alpha_powers) + 1;

        // allocate (2 ^ num_of_alpha_powers) + 1 bytes memory for current tabulation
        curr_alpha_power = alpha_powers[num_of_alpha_powers] = new MT[array_size];
        ARAGELI_ASSERT_3(curr_alpha_power != NULL);

        half_array_size = array_size / 2;

        // tabulate alpha powers
        for (i = 0; i <= (array_size / 2); i++)
        {
            curr_alpha_power[i] = current_power;
            curr_alpha_power[i + half_array_size] = p - current_power;
#ifdef BARRETT
            current_power = barrett_reduction<p_num,p_inv,b_n>(doubled_value(current_power) * alpha);
#else
            current_power = MT((doubled_value(current_power) * alpha) % p);
#endif
        }

        for (j = num_of_alpha_powers - 1; j >= 0; j--)
        {
            // Size of array is (2^j + 1)
            array_size = (1 << j) + 1;

            // allocate (2 ^ j) + 1 bytes memory for current tabulation
            curr_alpha_power = alpha_powers[j] = new MT[array_size];
            ARAGELI_ASSERT_3(curr_alpha_power != NULL);

            // get source tabulation
            prev_alpha_power = alpha_powers[j+1];

            // make tabulation from previous one
            for (i = 0; i < array_size; i++)
            {
                curr_alpha_power[i] = prev_alpha_power[i * 2];
            }
        }
    }

    void binary_reverse_tabulation()
    {
        MT reverse_i = 0;
        unsigned long i, j;
        long *digits_of_reverse_i = new long[num_of_alpha_powers];
        for (j=0; j<num_of_alpha_powers; j++)
            digits_of_reverse_i[j] = 0;

        for (i=0; i<n; i++)
        {
            binary_reverse[i] = reverse_i;

            for (j=num_of_alpha_powers; j>0; j--)
            {
                if (digits_of_reverse_i[j-1] == 0)
                {
                    digits_of_reverse_i[j-1] = 1;
                    reverse_i = ((1<<(j-1)) + reverse_i) % (1 << j);
                    break;
                }
                else
                {
                    digits_of_reverse_i[j-1] = 0;
                }
            }
        }
        delete []digits_of_reverse_i;
    };
public:
    CFFT(const MT p_module = 469762049, const MT root = 3) // \xi 3 is primitive root in Z_{469762049} field
    {
        p = p_module;               // base
        n = n_max;
        // allocate memory storage for powers of alpha.
        // Note that we compute powers for each iteration of FFT.
        // So we duplicate data in exchange for opportunity to have
        // a sequential access that speed up FFT calculation.
        alpha_powers = (MT**)malloc(sizeof(MT*)*(num_of_alpha_powers+1));
        binary_reverse = new MT[n];
        /* calculate alpha value */
        alpha_determination(p, root);
        alpha_power_computation(p);
        binary_reverse_tabulation();
    }

    virtual ~CFFT()
    {
        unsigned int i;
        /* delete alpha powers */
        for (i = 0; i < num_of_alpha_powers + 1; i++)
        {
            if (alpha_powers[i] != NULL)
            {
                delete [] alpha_powers[i];
            }
        }
        free(alpha_powers);
        delete [] binary_reverse;
    };

    // This optimized for polynom multiplication FFT.
    // Actually we already know that a half of proimage values is zero.
    // So on the first iteratiion we reduce amount of computation.
    void FFT_h(const ET* proimage, const MT len, ET* image) const
    {
        ET *F_Hat = image;
        doubled_value tmp;
        MT *curr_alpha_power;

        long m = 0;
        MT Two_Power_k_m_1 = len / 2;
        MT Two_Power_k_Max_m_1 = n / 2;
        MT Two_Power_m = 1;
        MT i_0, i_1, i_0_j, i_1_j;
        unsigned long F_Hat_0, F_Hat_1;
        MT i, j;

        for (i=0;i < len; i++)
        {
            F_Hat[i] = proimage[binary_reverse[i * (n / len)]];
        }

        curr_alpha_power = alpha_powers[m + 1];

        for (i=0; i < Two_Power_k_m_1;  i++)
        {
            i_0 = 2 * i * Two_Power_m;
            i_1 = i_0 + Two_Power_m;
            for (j=0; j < Two_Power_m; j++)
            {
                i_0_j = i_0 + j;
                i_1_j = i_1 + j;

                F_Hat_0 = F_Hat[i_0_j];

                F_Hat[i_0_j] = F_Hat_0;
                F_Hat[i_1_j] = F_Hat_0;
            }
        }

        m++;
        Two_Power_m *= 2;
        Two_Power_k_m_1 /= 2;
        Two_Power_k_Max_m_1 /= 2;

        while (1) //  Loop_On_m : loop
        {
            curr_alpha_power = alpha_powers[m + 1];

            for (i=0; i < Two_Power_k_m_1;  i++)
            {
                i_0 = 2 * i * Two_Power_m;
                i_1 = i_0 + Two_Power_m;
                for (j=0; j < Two_Power_m; j++)
                {
                    i_0_j = i_0 + j;
                    i_1_j = i_1 + j;

                    F_Hat_0 = F_Hat[i_0_j];

                    tmp = curr_alpha_power[j];
#ifdef BARRETT
                    F_Hat_1 = ((tmp * F_Hat[i_1_j]) > p) ? barrett_reduction<p_num,p_inv,b_n>(tmp * F_Hat[i_1_j]) : ET(tmp * F_Hat[i_1_j]);
#else
                    F_Hat_1 = ((tmp * F_Hat[i_1_j]) > p) ? ET((tmp * F_Hat[i_1_j]) % p) : ET(tmp * F_Hat[i_1_j]);
#endif
                    F_Hat[i_0_j] = ((F_Hat_0  + F_Hat_1) > p) ? (F_Hat_0  + F_Hat_1) - p : F_Hat_0  + F_Hat_1;
                    if (F_Hat_0 >= F_Hat_1)
                    {
                        F_Hat[i_1_j] = F_Hat_0 - F_Hat_1;
                    }
                    else
                    {
                        F_Hat[i_1_j] = p - (F_Hat_1 - F_Hat_0);
                    }
                }
            }

            if (Two_Power_k_m_1 == 1) break;

            m++;
            Two_Power_m *= 2;
            Two_Power_k_m_1 /= 2;
            Two_Power_k_Max_m_1 /= 2;
        }
    };

    void FFT(const ET* proimage, const MT len, ET* image) const        // Fast Fourie Transform
    {
        ET *F_Hat = image;
        doubled_value tmp;
        MT *curr_alpha_power;

        long m = 0;
        MT Two_Power_k_m_1 = len >> 1;  //len / 2
        MT Two_Power_k_Max_m_1 = n >> 1;//n / 2
        MT Two_Power_m = 1;
        MT i_0, i_1, i_0_j, i_1_j;
        unsigned long F_Hat_0, F_Hat_1;
        MT i, j;

        for (i=0;i < len; i++)
        {
            F_Hat[i] = proimage[binary_reverse[i * (n / len)]];
        }

        while (1) //  Loop_On_m : loop
        {
            curr_alpha_power = alpha_powers[m + 1];

            for (i=0; i < Two_Power_k_m_1;  i++)
            {
                i_0 = (i * Two_Power_m) << 1;
                i_1 = i_0 + Two_Power_m;
                for (j=0; j < Two_Power_m; j++)
                {
                    i_0_j = i_0 + j;
                    i_1_j = i_1 + j;

                    F_Hat_0 = F_Hat[i_0_j];

                    tmp = curr_alpha_power[j];
                    F_Hat_1 = ((tmp * F_Hat[i_1_j]) > p) ? ET((tmp * F_Hat[i_1_j]) % p) : ET(tmp * F_Hat[i_1_j]);
                    F_Hat[i_0_j] = ((F_Hat_0  + F_Hat_1) > p) ? (F_Hat_0  + F_Hat_1) - p : F_Hat_0  + F_Hat_1;
                    if (F_Hat_0 >= F_Hat_1)
                    {
                        F_Hat[i_1_j] = F_Hat_0 - F_Hat_1;
                    }
                    else
                    {
                        F_Hat[i_1_j] = p - (F_Hat_1 - F_Hat_0);
                    }
                }
            }

            if (Two_Power_k_m_1 == 1) break;

            m++;
            Two_Power_m <<= 1;
            Two_Power_k_m_1 >>= 1;
            Two_Power_k_Max_m_1 >>= 1;
        }
    };

    void FFT_inverse(const ET* proimage, const MT len, ET* image) const
    {
        ET *F_Hat = image;
        MT *curr_alpha_power;

        long m = 0;
        MT Two_Power_k_m_1 = len >> 1;
        MT Two_Power_k_Max_m_1 = n >> 1;
        MT Two_Power_m = 1;
        MT i_0, i_1, i_0_j, i_1_j;
        unsigned long F_Hat_0, F_Hat_1;
        MT i, j;

        MT inverse_of_length = p - (p - 1) / len; // here must be double digit???

        for (i=0;i < len; i++)
        {
            F_Hat[i] = proimage[binary_reverse[i * (n / len)]];
        }

        while (1)
        {
            curr_alpha_power = alpha_powers[m + 1];

            for (i=0; i< Two_Power_k_m_1; i++)
            {
                i_0 = (i * Two_Power_m) << 1;
                i_1 = i_0 + Two_Power_m;
                for (j=0; j < Two_Power_m; j++)
                {
                    i_0_j = i_0 + j;
                    i_1_j = i_1 + j;
                    F_Hat_0 = F_Hat[i_0_j];

                    F_Hat_1 = ((doubled_value(curr_alpha_power[Two_Power_m*2 - j]) * F_Hat[i_1_j]) > p) ?
#ifdef BARRETT
                        barrett_reduction<p_num,p_inv,b_n>(doubled_value(curr_alpha_power[Two_Power_m*2 - j]) * F_Hat[i_1_j]) :
#else
                        ET((doubled_value(curr_alpha_power[Two_Power_m*2 - j]) * F_Hat[i_1_j]) % p) :
#endif
                        ET((doubled_value(curr_alpha_power[Two_Power_m*2 - j]) * F_Hat[i_1_j]));
                    F_Hat[i_0_j] = ((F_Hat_0  + F_Hat_1) > p) ? (F_Hat_0  + F_Hat_1) - p : F_Hat_0  + F_Hat_1;

                    if (F_Hat_0 >= F_Hat_1)
                        F_Hat[i_1_j] = F_Hat_0 - F_Hat_1;
                    else
                        F_Hat[i_1_j] = p - (F_Hat_1 - F_Hat_0);

                }
            }

            if (Two_Power_k_m_1 == 1) break;

            m++;
            Two_Power_m <<= 1;
            Two_Power_k_m_1 >>= 1;
            Two_Power_k_Max_m_1 >>= 1;
        }

        for (i=0;i<len;i++)
        {
            F_Hat[i] = ((doubled_value(F_Hat[i]) * inverse_of_length) > p) ?
#ifdef BARRETT
                barrett_reduction<p_num,p_inv,b_n>(doubled_value(F_Hat[i]) * inverse_of_length) :
#else
                ET((doubled_value(F_Hat[i]) * inverse_of_length) % p) :
#endif
                ET((doubled_value(F_Hat[i]) * inverse_of_length));
        }
    }
};

#ifdef BARRETT
template<typename ET, typename MT, _Internal::digit p_num, _Internal::digit p_inv, int b_n>
class CFFT_mult :
    private CFFT<ET,MT,p_num,p_inv,b_n>
#else
template<typename ET, typename MT>
class CFFT_mult :
    private CFFT<ET,MT>
#endif
{
public:

    /* constructor */
#ifdef BARRETT
    CFFT_mult(MT p_module = 469762049, MT root = 3) :
        CFFT<ET,MT,p_num,p_inv,b_n>(p_module, root)
    {}
#else
    CFFT_mult(MT p_module = 469762049, MT root = 3) :
        CFFT<ET,MT>(p_module, root)
    {}
#endif
    /* destructor */
    ~CFFT_mult()
    {}

    /* polinom multiplication */
    void poli_multiply(const ET *F, const MT F_Length, const ET *G, const MT G_Length, ET *Result_Out) const
    {
        static ET F_Copy[n_max];
        static ET G_Copy[n_max];

        static ET Hat_F[n_max];
        static ET Hat_G[n_max];
        static ET Result[n_max];
#ifdef BARRETT
        MT p_Large = CFFT<ET,MT,p_num,p_inv,b_n>::p;
#else
        MT p_Large = CFFT<ET,MT>::p;
#endif
        MT i;
        typedef typename doubled_type<ET>::d_value dd;

        MT Result_Length = 1;
        MT r = F_Length + G_Length;
        do
        {
            Result_Length <<= 1;
        }
        while (r >>= 1);

        /* *COMMENT* check buffer length here -> create larger buffers or return error */

        /* copy polinoms data to buffers */
        memcpy(F_Copy,F,sizeof(ET)*F_Length);
        memset(&F_Copy[F_Length],0,sizeof(ET)*(Result_Length-F_Length));

        memcpy(G_Copy,G,sizeof(ET)*G_Length);
        memset(&G_Copy[G_Length],0,sizeof(ET)*(Result_Length-G_Length));

        /* make images */
        //FFT_h(F_Copy,Result_Length, Hat_F);
        //FFT_h(G_Copy,Result_Length, Hat_G);
        FFT(F_Copy,Result_Length, Hat_F);
        FFT(G_Copy,Result_Length, Hat_G);

        /* make image of result*/
        for (i=0; i<Result_Length;i++)
        {
            Result[i] = ((dd(Hat_F[i]) * Hat_G[i]) > p_Large) ?
#ifdef BARRETT
                        barrett_reduction<p_num,p_inv,b_n>(dd(Hat_F[i]) * Hat_G[i]) :
#else
                        (ET)((dd(Hat_F[i]) * Hat_G[i]) % p_Large) :
#endif
                        (ET)(dd(Hat_F[i]) * Hat_G[i]);
        }

        /* make result from image */
        FFT_inverse(Result,Result_Length,Result_Out);
    };
};


} // namesapce Arageli

// UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS fft.cpp FILE
//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_fft
//    #include "fft.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_fft
//#endif

#endif    // #ifndef _ARAGELI_fft_hpp_
