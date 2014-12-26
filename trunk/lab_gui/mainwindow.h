#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QLabel;
class QActionGroup;

class CryptoWidget;
class SignWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);

private:
	void initializeGui();

	void initializeCentralWidget();

	void initializeMenu();
	void initializeFileMenu(QMenu *fileMenu);
	void initializeKeyMenu(QMenu *keyMenu);
	void initializeSignMenu(QMenu *signMenu);
	void initializeGostMenu(QMenu *gostMenu);
	QActionGroup *gostActionGroup;

	bool mayBeSigned();
	bool mayBeVerifyed();

	void initializeStatusBar();
	QLabel *labelForCurrentFile;

	CryptoWidget *cryptoWidget;
	SignWidget * signWidget;


private slots:
	void onSelectFile();
	void onChangeGost();

	void onGenerateNewParameters();
	void onGenerateNewKeys();
	void onSavePublicKey();
	void onSaveSecretKey();
	void onLoadPublicKey();
	void onLoadSecretKey();

	void onSignFile();
	void onVerifySign();
};

#endif // MAINWINDOW_H
