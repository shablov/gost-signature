#include "cryptogeneratorgost_1994.h"

#include "prime.hpp"

CryptoGeneratorGost_1994::CryptoGeneratorGost_1994(const unsigned int &multiplier)
	: pSize(512), normalBitsQuantity(16)
{
	bitsMultiplier = (multiplier == 2) ? multiplier : 1;
	parameters["p"] = big_int();
	parameters["q"] = big_int();
	parameters["alpha"] = big_int();
}

CryptoGeneratorGost_1994::~CryptoGeneratorGost_1994()
{

}

void CryptoGeneratorGost_1994::generate()
{
	generateBeginParameters();
	generatePAndQ();
	generateAlpha();
}

void CryptoGeneratorGost_1994::setY0(const big_int &y0)
{
	this->y0 = y0;
}

big_int CryptoGeneratorGost_1994::getY0()
{
	return y0;
}

void CryptoGeneratorGost_1994::setKeyLength(const unsigned int &length)
{
	pSize = length;
}

void CryptoGeneratorGost_1994::generateBeginParameters()
{
	y0 = generateNumber();
	c = generateOddNumber();
}

big_int CryptoGeneratorGost_1994::generateNumber()
{
	return big_int::random_with_length_or_less(normalBitsQuantity * bitsMultiplier);
}

big_int CryptoGeneratorGost_1994::generateOddNumber()
{
	big_int tmp;
	do
	{
		tmp = generateNumber();
	} while (!tmp.is_odd());
	return tmp;
}

void CryptoGeneratorGost_1994::generateAlpha()
{
	do
	{
		big_int d = big_int::random_in_range(parameters["p"] - 3) + 2;
		parameters["alpha"] = power_mod(d, (parameters["p"] - 1)/parameters["q"], parameters["p"]);
	} while (parameters["alpha"] == 1);
}

big_int CryptoGeneratorGost_1994::calculateP(big_int pSize, big_int prevP)
{
	big_int p;
	do
	{
		p = calculateProbablyP(pSize, prevP);
	} while (p.is_null());
	return p;
}

big_int CryptoGeneratorGost_1994::calculateProbablyP(const big_int &pSize, const big_int &prevP)
{
	QList<big_int> y_array = calculateY_Array(calculateRm(pSize));

	y0 = y_array.last();

	big_int N = calculateN(prevP, pSize,
			calculateYm(y_array));

	big_int k = 0;

	big_int probablyP = parameters["p"];
	do
	{
		parameters["p"] = calculateNumber(N, k, prevP);

		if (checkP(prevP, N, k, pSize))
		{
			break;
		}
		k += 2;
	} while (true);

	swap(parameters["p"], probablyP);
	return probablyP;
}

QList<big_int> CryptoGeneratorGost_1994::calculateY_Array(const unsigned int &arraySize)
{
	QList<big_int> y_array;
	y_array.append(y0);
	for (unsigned int i = 0; i < arraySize; i++)
	{
		big_int module = pow2<big_int>(normalBitsQuantity * bitsMultiplier);
		big_int number = y_array[i] * standartGostMultiplier() + c;
		y_array.append(mod(number, module));
	}
	return y_array;
}

unsigned int CryptoGeneratorGost_1994::standartGostMultiplier()
{
	return (bitsMultiplier == 2 ? 97781173 : 19381);
}

big_int CryptoGeneratorGost_1994::calculateRm(const unsigned int &bitsQuantity)
{
	return  (bitsQuantity / (normalBitsQuantity * bitsMultiplier)) +
			((bitsQuantity % (normalBitsQuantity * bitsMultiplier)) != 0);
}

big_int CryptoGeneratorGost_1994::calculateYm(const QList<big_int> &y_array)
{
	big_int Ym;
	for(int i = 0; i < y_array.size() - 2; i++)
	{
		Ym += y_array[i] * pow2<big_int>(bitsMultiplier * normalBitsQuantity * i);
	}
	return Ym;
}

CryptoGeneratorGost_1994_MethodA::CryptoGeneratorGost_1994_MethodA(const unsigned int &multiplier)
	: CryptoGeneratorGost_1994(multiplier)
{

}

void CryptoGeneratorGost_1994_MethodA::generateQ()
{
	big_int tmp = y0;
	generateBeginParameters();
	y0 = tmp;
	generatePAndQ();
}

void CryptoGeneratorGost_1994_MethodA::generatePAndQ()
{
	QList<big_int> t_array = calculateT_Array();
	big_int p_array[t_array.size()];
	p_array[t_array.size() - 1] = generateMinPrimeWithLength(t_array.last());

	int m = t_array.size() - 2;
	for (; m >= 0; m--)
	{
		p_array[m] = calculateP(t_array[m], p_array[m + 1]);
	}


	std::cout<<p_array[5]<<"\n";
	std::cout<<p_array[4]<<"\n";
	std::cout<<p_array[3]<<"\n";
	std::cout<<p_array[2]<<"\n";
	std::cout<<p_array[1]<<"\n";
	std::cout<<p_array[0]<<"\n";

	parameters["p"] = p_array[0];
	parameters["q"] = p_array[1];
}

QList<big_int> CryptoGeneratorGost_1994_MethodA::calculateT_Array()
{
	QList<big_int> t_array;
	t_array.append(pSize);
	do
	{
		t_array.append(t_array.last() / 2);
	} while (t_array.last() >= normalBitsQuantity * bitsMultiplier + 1);
	return t_array;
}

big_int CryptoGeneratorGost_1994_MethodA::generateMinPrimeWithLength(const unsigned int &bitsQuantity)
{
	big_int minNumber = pow2<big_int>(bitsQuantity - 1);
	return next_prime(minNumber);
}

big_int CryptoGeneratorGost_1994_MethodA::calculateN(const big_int &p, const big_int &t, const big_int &Ym)
{
	big_int tmp = pow2<big_int>(t - 1);
	big_int N = (tmp / p) +
			(tmp % p != 0) +
			((tmp * Ym) / (pow2<big_int>(normalBitsQuantity * bitsMultiplier * calculateRm(t)) * p));
	N += N % 2;
	return N;
}

big_int CryptoGeneratorGost_1994_MethodA::calculateNumber(const big_int &N, const big_int &k, const big_int &prevP)
{
	return prevP * (N + k) + 1;
}

bool CryptoGeneratorGost_1994_MethodA::checkP(const big_int &prevP, const big_int &N, const big_int &k, const big_int &pSize)
{
	if (parameters["p"] > (pow2<big_int>(pSize)))
	{
		parameters["p"] = 0;
		return true;
	}
	return ((power_mod(static_cast<big_int>(2), prevP * (N + k), parameters["p"]) == 1) &&
			(power_mod(static_cast<big_int>(2), N + k, parameters["p"]) != 1));
}

CryptoGeneratorGost_1994_MethodB::CryptoGeneratorGost_1994_MethodB(const unsigned int &multiplier)
	: CryptoGeneratorGost_1994(multiplier)
{

}

void CryptoGeneratorGost_1994_MethodB::generatePAndQ()
{
	big_int tmp = y0;
	parameters["q"] = generateQ(256);
	big_int prevP = generateQ(512);
	swap(y0, tmp);

	parameters["p"] = calculateP(1024, prevP);
}

big_int CryptoGeneratorGost_1994_MethodB::generateQ(const unsigned int &bitsQuantity)
{
	CryptoGeneratorGost_1994_MethodA tmp(bitsMultiplier);
	tmp.setY0(y0);
	tmp.setKeyLength(bitsQuantity * 2);

	tmp.generateQ();
	y0 = tmp.getY0();
	return tmp.getParameter("q");
}

big_int CryptoGeneratorGost_1994_MethodB::calculateN(const big_int &q, const big_int &pSize, const big_int &Ym)
{
	big_int tmp = pow2<big_int>(pSize - 1);
	big_int N = (tmp / (parameters["q"] * q)) +
			((tmp % (parameters["q"] * q)) != 1) +
			(tmp * Ym / (parameters["q"] * q * pow2<big_int>(pSize)));
	N += N % 2;
	return N;
}

QList<big_int> CryptoGeneratorGost_1994_MethodB::calculateY_Array(const big_int &)
{
	return CryptoGeneratorGost_1994::calculateY_Array(64 / bitsMultiplier);
}

big_int CryptoGeneratorGost_1994_MethodB::calculateNumber(const big_int &N, const big_int &k, const big_int &prevP)
{
	return parameters["q"] * prevP * (N + k) + 1;
}

bool CryptoGeneratorGost_1994_MethodB::checkP(const big_int &prevP, const big_int &N, const big_int &k, const big_int &pSize)
{
	if (parameters["p"] > pow2<big_int>(pSize))
	{
		parameters["p"] = 0;
		return true;
	}
	return (power_mod(static_cast<big_int>(2), parameters["q"] * prevP * (N + k), parameters["p"]) == 1 &&
			power_mod(static_cast<big_int>(2), parameters["q"] * (N + k), parameters["p"]) != 1);
}
