/*****************************************************************************

    rndbase.hpp

    This file is a part of the Arageli library.

    Things in this file have been obtained from Sergey's private library
    Lib_firstlyss::Math::Rnd that was been writing in 2003--2004.

    Copyright (C) 2003, 2004, 2006 Sergey S. Lyalin

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
    \file rndbase.hpp
    \brief Base random number generators.

    This file contains several platform independent generators for pseudo-
    random numbers. These generators are used in more complex pseudo-random
    elements creating.

    All definitions are placed in Arageli::rnd namespace.

    Unfortunately the description of the classes is in Russian.

    Общие правила работы с генераторами.

    Для генераторов-шаблонов: Первый параметр -- тип генерируемого значения.
    Пусть Генератор -- имя класса генератора (инстанцированного из шаблона),
    тогда:

    - Генератор :: value_type -- тип генерируемого значения (возвращается
        Генератор::operator()).
    - Генератор :: seed_type -- тип семени (передаётся конструкторы и функции
        reset).
    - Генератор :: cardinality_type -- тип, который используется для представления
        мощности генератора.
    - Генератор :: period_type -- тип, который используется для представления
        периода генератора (отрезка апериодичности).

    - Генератор() -- создание с некоторым веделенным стартовым семенем.
    - Генератор(СЕМЯ) -- создание с семенем СЕМЯ. СЕМЯ -- значение типа
        Генератор::Seed.

    - value_type Генератор :: operator() () -- получение очередной реализации
        псевдослучайного процесса.

    - value_type Генератор :: min() -- минимальное значение, которое может
        сгенерироваться.
    - value_type Генератор :: max() -- максимальное значение, которое может
        сгенерироваться.
    - cardinality_type Генератор :: cardinality() -- мощность генератора минус еденица.
        Мощность -- это число всевозможных различных значений, которые могут
        быть получены.
    - period_type Генератор :: period() -- период (отрезок апериодичности)
        минус еденица. Период -- это минимальная длина последовательности
        чисел, которая постоянно повторяется при генерации.
    - void Генератор :: reset(СЕМЯ) -- реинициализация генератора семенем СЕМЯ.

*/

// WARNING! Some features in the file need the unsigned int type
// to have 32 bits.


#ifndef _ARAGELI_rndbase_hpp_
#define _ARAGELI_rndbase_hpp_

#include <limits>

#include "config.hpp"
#include "exception.hpp"
#include "factory.hpp"


namespace Arageli
{

/// All things for random elements generating.
namespace rnd
{

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4724)
#endif

/// Линейная конгруентная последовательность.
/** Коэффициенты задаются статически (параметры шаблона).
    Если M == 0, то модуль реализуется через отсечение при целочисленном
    переполнении, т. е. в этом случае модуль равен числу всевозможных
    значений Int.
    Requirements: [Int -- беззнаковый интегральный тип]. */
template <class Int, Int A, Int C, Int M = Int(0)>
class lcseq
{
public:

    typedef Int value_type;
    typedef Int seed_type;

    lcseq () :
        x(1)
    {
        ARAGELI_ASSERT_0(invariant());
    }

    lcseq (const seed_type& seed) :
        x(seed)
    {
        ARAGELI_ASSERT_0(invariant());
    }

    void reset (const seed_type& seed = unit<seed_type>())
    {
        x = seed;
    }

    value_type operator() () const
    {
        // 'if' works in compile time
        if(M)
            return (x = (A*x+C)%M);
        else
            return (x = A*x+C);
    }

    /// коэффициент a процесса
    static value_type a ()
    {
        return A;
    }

    /// коэффициент c процесса
    static value_type c ()
    {
        return C;
    }

    /// модуль-1, т. к. модуль может быть непредставимым
    static value_type m ()
    {
        return value_type(M-1);
    }

private:

    bool invariant ()
    {
        return
            std::numeric_limits<Int>::is_specialized &&
            std::numeric_limits<Int>::is_integer &&
            !std::numeric_limits<Int>::is_signed &&
            (M != 0 || std::numeric_limits<Int>::is_modulo);
    }

    mutable Int x;
};

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

// ---------------------------------------------------------------------------

/// Заготовка для "хороших" (полных) LC-последовательностей.
template <class Int, Int A, Int B, Int M = Int(0)>
class lcseq_fullpow : public lcseq<Int, A, B, M>
{
    typedef lcseq<Int, A, B, M> Base;

public:

    typedef typename Base::value_type value_type;
    typedef value_type cardinality_type;
    typedef value_type period_type;
    typedef typename Base::seed_type seed_type;

    lcseq_fullpow ()
    {}

    lcseq_fullpow (seed_type seed) :
        Base(seed)
    {}

    static cardinality_type cardinality ()
    {
        return Base::m();
    }

    static period_type period ()
    {
        return cardinality();
    }

    static value_type min ()
    {
        return null<value_type>();
    }

    static value_type max ()
    {
        return cardinality();
    }

};

// ---------------------------------------------------------------------------

namespace _Internal
{
    template <bool Gr, bool Eq, int Shift>
    struct module_2_32_helper1;

    template <int Shift>
    struct module_2_32_helper1<true, false, Shift>
    {
        static const unsigned int module = (1 << Shift);
    };

    template <int Shift>
    struct module_2_32_helper1<false, true, Shift>
    {
        static const unsigned int module = 0;
    };

    struct module_2_32 :
        public module_2_32_helper1
        <
            (std::numeric_limits<unsigned int>::digits > 32),
            (std::numeric_limits<unsigned int>::digits == 32),
            32
        >
    {};
}

// lcseq_fineX -- известные "хорошие" генераторы.
// Надо: [как минимум 32-х битный unsigned int]

/// Good lc-sequence 1 from VC++ 7.0 C-library sources.
/** Из реализации функции rand в VC++ 7.0 (см. "rand.c").
    Принимается по умолчанию. */
typedef lcseq_fullpow
<
    unsigned,
    214013u,
    2531011u,
    _Internal::module_2_32::module
>
    lcseq_fine1;

/// Good lc-sequence 2 from C++ Builder 6.0 C-library sources.
/** Из реализации функции rand в С++ Builder 6.0. */
typedef lcseq_fullpow
<
    unsigned,
    22695477u,
    1u,
    _Internal::module_2_32::module
>
    lcseq_fine2;

/// Good lc-sequence 3 from book "Язык программирования C++" by Straustrup.
/** Из книги Б. Страуструпа "Язык программирования C++", 3-е издание, стр. 754. */
typedef lcseq_fullpow
<
    unsigned,
    1103515245u,
    12345u,
    _Internal::module_2_32::module
>
    lcseq_fine3;


} // namespace rnd
} // namesapce Arageli


#if 0 // UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS rndbase.cpp FILE
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_rndbase
    #include "rndbase.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_rndbase
#endif
#endif

#endif    // #ifndef _ARAGELI_rndbase_hpp_
