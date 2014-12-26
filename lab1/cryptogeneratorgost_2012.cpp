#include "cryptogeneratorgost_2012.h"

#include "function.h"

CryptoGeneratorGost_2012::CryptoGeneratorGost_2012()
{
	/// Curve P-521
	parameters["p"] = "68647976601306097149819007990813932172694353"
					  "00143305409394463459185543183397656052122559"
					  "64066145455497729631139148085803712198799971"
					  "6643812574028291115057151";

	parameters["a"] = -3;

	QByteArray b = QByteArray::fromHex("51953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00");
	parameters["b"] = Function::big_intFromByteArray(b);

	QByteArray Gx = QByteArray::fromHex("c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66");
	parameters["Gx"] = Function::big_intFromByteArray(Gx);

	QByteArray Gy = QByteArray::fromHex("11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650");
	parameters["Gy"] = Function::big_intFromByteArray(Gy);

	parameters["n"] = "68647976601306097149819007990813932172694353"
					  "00143305409394463459185543183397655394245057"
					  "74633321719753296399637136332111386476861244"
					  "0380340372808892707005449";
	parameters["h"] = 1;

	/// Curve P-192
//	parameters["p"] = "6277101735386680763835789423207666416083908700390324961279";
//	parameters["a"] = -3;
//	parameters["b"] = "2455155546008943817740293915197451784769108058161191238065";
//	parameters["Gx"] =  "602046282375688656758213480587526111916698976636884684818";
//	parameters["Gy"] =  "174050332293622031404857552280219410364023488927386650641";
//	parameters["n"] = "6277101735386680763835789423176059013767194773182842284081";
//	parameters["h"] = 1;
}

CryptoGeneratorGost_2012::~CryptoGeneratorGost_2012()
{

}

void CryptoGeneratorGost_2012::generate()
{

}