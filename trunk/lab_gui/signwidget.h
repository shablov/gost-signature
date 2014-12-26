#ifndef SIGNWIDGET_H
#define SIGNWIDGET_H

#include <QWidget>

class QTextEdit;

class SignWidget : public QWidget
{
	Q_OBJECT
public:
	SignWidget(QWidget *parent = 0);
	QByteArray text();
	void setText(const QByteArray &text);

public slots:
	void onOpenFile();
	void onSaveFile();

private:

	QTextEdit *textEdit;
};

#endif // SIGNWIDGET_H
