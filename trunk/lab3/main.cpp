#include <QCoreApplication>
#include <QFile>
#include <QBuffer>

#include <QDebug>

#include <hashinterface.h>
#include <hashfactory.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	unsigned char byte[] = {
		0xfb,0xe2,0xe5,0xf0,0xee,0xe3,0xc8,0x20,0xfb,0xea,0xfa,0xeb,0xef,0x20,0xff,0xfb,
		0xf0,0xe1,0xe0,0xf0,0xf5,0x20,0xe0,0xed,0x20,0xe8,0xec,0xe0,0xeb,0xe5,0xf0,0xf2,
		0xf1,0x20,0xff,0xf0,0xee,0xec,0x20,0xf1,0x20,0xfa,0xf2,0xfe,0xe5,0xe2,0x20,0x2c,
		0xe8,0xf6,0xf3,0xed,0xe2,0x20,0xe8,0xe6,0xee,0xe1,0xe8,0xf0,0xf2,0xd1,0x20,0x2c,
		0xe8,0xf0,0xf2,0xe5,0xe2,0x20,0xe5,0xd1
		};
	QByteArray asd((char*)byte, 72);
	QBuffer *t = new QBuffer(&asd);
	t->open(QIODevice::ReadOnly);
	qDebug() << HashFactory::hash(2012, t, HashInterface::Base).toHex() << "\n"
			 << "508f7e553...........d29d";
	delete t;

	return 0;
}