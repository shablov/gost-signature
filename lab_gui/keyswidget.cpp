#include "keyswidget.h"

#include <QApplication>

#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

#include <QFormLayout>

#include <QTextStream>
#include <QRegExpValidator>

#include <QDebug>

#include "signaturefactory.h"
#include "signatureinterface.h"
#include "function.h"

KeysWidget::KeysWidget(QWidget *parent) :
	QWidget(parent)
{
	sign = SignatureFactory::signatureByYear(1994);

	createWidgets();
	QFormLayout *mainLayout = new QFormLayout(this);

	mainLayout->addRow("p = ", pLE);
	mainLayout->addRow("q = ", qLE);
	mainLayout->addRow("alpha = ", alphaLE);
	mainLayout->addRow("secret key = ", secKeyLE);
	mainLayout->addRow("public key = ", pubKeyLE);

	connect(this, SIGNAL(generated()), SLOT(updateGui()));
}

void KeysWidget::createWidgets()
{
	QRegExp hexRegExp("[A-Fa-f0-9]+");

	pLE = new QLineEdit;
	pLE->setValidator(new QRegExpValidator(hexRegExp));

	qLE = new QLineEdit;
	qLE->setValidator(new QRegExpValidator(hexRegExp));

	alphaLE = new QLineEdit;
	alphaLE->setValidator(new QRegExpValidator(hexRegExp));

	secKeyLE = new QLineEdit;
	secKeyLE->setValidator(new QRegExpValidator(hexRegExp));

	pubKeyLE = new QLineEdit;
	pubKeyLE->setValidator(new QRegExpValidator(hexRegExp));
}

SignatureInterface *KeysWidget::signature()
{
	QByteArray pHex = pLE->text().toUtf8();
	big_int p = Function::big_intFromByteArray(QByteArray::fromHex(pHex));
	QByteArray qHex = pLE->text().toUtf8();
	big_int q = Function::big_intFromByteArray(QByteArray::fromHex(qHex));
	QByteArray alphaHex = pLE->text().toUtf8();
	big_int alpha = Function::big_intFromByteArray(QByteArray::fromHex(alphaHex));
	sign->setParameter("p", p);
	sign->setParameter("q", q);
	sign->setParameter("alpha", alpha);
	return sign;
}

bool KeysWidget::maybeSave()
{
	bool maybeSave = true;
	if (cryptoParametersIsModified())
	{
		maybeSave = maybeSaveKey(pubKeyLE);
		setCryptoParametersModified(true);
		maybeSave &= maybeSaveKey(secKeyLE);
	}
	else if (pubKeyLE->isModified())
	{
		maybeSave = maybeSaveKey(pubKeyLE);
	}
	else if (secKeyLE->isModified())
	{
		maybeSave = maybeSaveKey(secKeyLE);
	}
	return maybeSave;
}

void KeysWidget::openPubKey()
{
	if (maybeSaveKey(pubKeyLE))
	{
		QString fileName = QFileDialog::getOpenFileName(parentWidget());
		if (!fileName.isEmpty())
		{
			loadKeyFile(fileName, pubKeyLE);
		}
	}
}

void KeysWidget::openSecKey()
{
	if (maybeSaveKey(secKeyLE))
	{
		QString fileName = QFileDialog::getOpenFileName(parentWidget());
		if (!fileName.isEmpty())
		{
			loadKeyFile(fileName, secKeyLE);
		}
	}
}

bool KeysWidget::maybeSaveKey(QLineEdit *keyLE)
{
	if (cryptoParametersIsModified() || keyLE->isModified())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(parentWidget(), tr("Application"),
								   tr("The %1 key has been modified.\n"
									  "Do you want to save your changes?")
								   .arg(keyLE == pubKeyLE ? "public" : "secret"),
								   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
		{
			return saveKeyAs(keyLE);
		}
		else if (ret == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

bool KeysWidget::cryptoParametersIsModified()
{
	return  pLE->isModified() ||
			qLE->isModified() ||
			alphaLE->isModified();
}

void KeysWidget::setCryptoParametersModified(bool modified)
{
	pLE->setModified(modified);
	qLE->setModified(modified);
	alphaLE->setModified(modified);
}

void KeysWidget::loadKeyFile(const QString fileName, QLineEdit *keyLE)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::warning(this, tr("Application"),
							 tr("Cannot read file %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		return;
	}

	QDataStream in(&file);
	setOverrideWaitCursor();

	pLE->setText(parameterFromStream(in).toHex());
	qLE->setText(parameterFromStream(in).toHex());
	alphaLE->setText(parameterFromStream(in).toHex());
	keyLE->setText(parameterFromStream(in).toHex());
	setCryptoParametersModified(false);
	keyLE->setModified(false);

	restoreOverrideCursor();
}

QByteArray KeysWidget::parameterFromStream(QDataStream &in)
{
	quint32 size = 0;
	in >> size;
	QByteArray parameter;
	parameter.resize(size);
	in >> parameter;
	return parameter;
}

void KeysWidget::setOverrideWaitCursor()
{
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
}

void KeysWidget::restoreOverrideCursor()
{
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
}

bool KeysWidget::savePubKey()
{
	return saveKeyAs(pubKeyLE);
}

bool KeysWidget::saveSecKey()
{
	return saveKeyAs(secKeyLE);
}

bool KeysWidget::saveKeyAs(QLineEdit *keyLE)
{
	QString fileName = QFileDialog::getSaveFileName(parentWidget());
	if (fileName.isEmpty())
	{
		return false;
	}

	return saveKeyFile(fileName, keyLE);
}

bool KeysWidget::saveKeyFile(const QString &fileName, QLineEdit *keyLE)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly))
	{
		QMessageBox::warning(parentWidget(), tr("Application"),
							 tr("Cannot write file %1:\n%2.")
							 .arg(fileName)
							 .arg(file.errorString()));
		return false;
	}

	QDataStream out(&file);
	setOverrideWaitCursor();

	QByteArray p = QByteArray::fromHex(pLE->text().toUtf8());
	QByteArray q = QByteArray::fromHex(qLE->text().toUtf8());
	QByteArray alpha = QByteArray::fromHex(alphaLE->text().toUtf8());
	QByteArray keyBA = QByteArray::fromHex(keyLE->text().toUtf8());

	parameterToStream(out, p);
	parameterToStream(out, q);
	parameterToStream(out, alpha);
	parameterToStream(out, keyBA);
	setCryptoParametersModified(false);
	keyLE->setModified(false);

	restoreOverrideCursor();

	return true;
}

void KeysWidget::parameterToStream(QDataStream &out, const QByteArray& parameter)
{
	out << static_cast<quint32>(parameter.length()) << parameter;
}

void KeysWidget::generateNewKeys()
{
	sign->generateNewParameters();
	emit generated();
}

void KeysWidget::updateGui()
{
	QByteArray p = Function::big_intToByteArray(sign->getParameter("p"));
	QByteArray q = Function::big_intToByteArray(sign->getParameter("q"));
	QByteArray alpha = Function::big_intToByteArray(sign->getParameter("alpha"));
	QByteArray publicKey = Function::big_intToByteArray(sign->getPublicKey());
	QByteArray secretKey = Function::big_intToByteArray(sign->getSecretKey());

	pLE->setText(p.toHex());
	qLE->setText(q.toHex());
	alphaLE->setText(alpha.toHex());
	pubKeyLE->setText(publicKey.toHex());
	secKeyLE->setText(secretKey.toHex());
}
