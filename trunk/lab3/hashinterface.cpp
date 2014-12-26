#include "hashinterface.h"

#include <QIODevice>

#include <QDebug>

HashInterface::HashInterface()
{
}

HashInterface::~HashInterface()
{
	data.clear();
}

QByteArray HashInterface::hash(QIODevice *inputDevice, const HashSize &/*size*/)
{
	data = inputDevice->readAll();
	return QByteArray();
}
