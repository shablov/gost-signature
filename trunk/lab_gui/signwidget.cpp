#include "signwidget.h"

#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>

#include <QVBoxLayout>

SignWidget::SignWidget(QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	textEdit = new QTextEdit(this);
	mainLayout->addWidget(new QLabel("signature = "));
	mainLayout->addWidget(textEdit, 1);
}

QByteArray SignWidget::text()
{
	return QByteArray::fromHex(textEdit->toPlainText().toUtf8());
}

void SignWidget::setText(const QByteArray &text)
{
	textEdit->setText(text.toHex());
}

void SignWidget::onOpenFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Choose file"),
													QDir::currentPath());

	if (filePath.isEmpty())
	{
		return;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	setText(file.readAll());
}

void SignWidget::onSaveFile()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("Choose file"),
													QDir::currentPath());

	if (filePath.isEmpty())
	{
		return;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}
	file.write(text());
}
