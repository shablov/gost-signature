#include "mainwindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QLabel>
#include <QActionGroup>
#include <QMessageBox>

#include <QDebug>

#include "cryptowidget.h"
#include "cryptowidgetfactory.h"
#include "signwidget.h"

#include "signatureinterface.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  cryptoWidget(0), signWidget(0)
{
	initializeGui();
	setMinimumSize(1024, 768);
	setWindowTitle(tr("Gost realization"));
}

void MainWindow::initializeGui()
{
	initializeCentralWidget();
	initializeMenu();
	initializeStatusBar();
}

void MainWindow::initializeCentralWidget()
{
	labelForCurrentFile = new QLabel("");

	QTabWidget *tabWidget = new QTabWidget(this);
	setCentralWidget(tabWidget);

	signWidget = new SignWidget;
	tabWidget->addTab(signWidget, "Signature");
}

void MainWindow::initializeMenu()
{
	QMenuBar *bar = menuBar();

	QMenu *fileMenu = bar->addMenu(tr("File"));
	QMenu *keyMenu = bar->addMenu(tr("Key"));
	QMenu *signMenu = bar->addMenu(tr("Sign"));
	QMenu *gostMenu = bar->addMenu(tr("Gost"));

	initializeFileMenu(fileMenu);
	initializeKeyMenu(keyMenu);
	initializeSignMenu(signMenu);
	initializeGostMenu(gostMenu);
}

void MainWindow::initializeFileMenu(QMenu *fileMenu)
{
	fileMenu->addAction(tr("Select file"), this, SLOT(onSelectFile()));
	fileMenu->addSeparator();
	fileMenu->addAction(tr("Exit"), this, SLOT(close()));
}

void MainWindow::initializeKeyMenu(QMenu *keyMenu)
{
	keyMenu->addAction(tr("Generate new parameters"), this, SLOT(onGenerateNewParameters()));
	keyMenu->addAction(tr("Generate new keys"), this, SLOT(onGenerateNewKeys()));
	keyMenu->addAction(tr("Save public key"), this, SLOT(onSavePublicKey()));
	keyMenu->addAction(tr("Save secret key"), this, SLOT(onSaveSecretKey()));
	keyMenu->addAction(tr("Load public key"), this, SLOT(onLoadPublicKey()));
	keyMenu->addAction(tr("Load secret key"), this, SLOT(onLoadSecretKey()));
}

void MainWindow::initializeSignMenu(QMenu *signMenu)
{
	signMenu->addAction(tr("Open signature"), signWidget, SLOT(onOpenFile()));
	signMenu->addAction(tr("Save signature"), signWidget, SLOT(onSaveFile()));
	signMenu->addAction(tr("Sign file"), this, SLOT(onSignFile()));
	signMenu->addAction(tr("Verify signature"), this, SLOT(onVerifySign()));
}

void MainWindow::initializeGostMenu(QMenu *gostMenu)
{
	QAction *gostAction_1994 = gostMenu->addAction(tr("Gost 1994 year"));
	QAction *gostAction_2012 = gostMenu->addAction(tr("Gost 2012 year"));
	gostAction_1994->setCheckable(true);
	gostAction_2012->setCheckable(true);
	gostAction_1994->setData(1994);
	gostAction_2012->setData(2012);
	connect(gostAction_1994, SIGNAL(toggled(bool)), this, SLOT(onChangeGost()));
	connect(gostAction_2012, SIGNAL(toggled(bool)), this, SLOT(onChangeGost()));

	gostActionGroup = new QActionGroup(this);
	gostActionGroup->addAction(gostAction_1994);
	gostActionGroup->addAction(gostAction_2012);
	gostActionGroup->setExclusive(true);
}

void MainWindow::initializeStatusBar()
{
	QStatusBar *bar = statusBar();
	bar->addWidget(labelForCurrentFile);
}

void MainWindow::onSelectFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Choose file"),
													QDir::currentPath());

	if (!filePath.isEmpty())
	{
		labelForCurrentFile->setText(filePath);
		QFontMetrics fontMetrics(font());
		int newMinimumWidth = (fontMetrics.lineSpacing() << 1) + fontMetrics.width(filePath);
		newMinimumWidth = (newMinimumWidth < 200) ? 200 : newMinimumWidth;
		setMinimumWidth(newMinimumWidth);
	}
}

void MainWindow::onChangeGost()
{
	QAction *gostAction = gostActionGroup->checkedAction();
	if (!gostAction)
	{
		return;
	}

	QTabWidget *tabWidget = qobject_cast<QTabWidget*>(centralWidget());
	if (cryptoWidget)
	{
		tabWidget->removeTab(tabWidget->indexOf(cryptoWidget));
		cryptoWidget->deleteLater();
	}

	cryptoWidget = CryptoWidgetFactory::getCryptoWidget(gostAction->data().toInt());
	tabWidget->addTab(cryptoWidget, tr("Keys"));
}

void MainWindow::onGenerateNewParameters()
{
	if (cryptoWidget)
	{
		cryptoWidget->generateNewParameters();
	}
}

void MainWindow::onGenerateNewKeys()
{
	if (cryptoWidget)
	{
		cryptoWidget->generateNewKeys();
	}
}

void MainWindow::onSavePublicKey()
{
	if (cryptoWidget)
	{
		QString filePath = QFileDialog::getSaveFileName(this, tr("Save public key"),
													 QDir::currentPath());
		if (filePath.isEmpty())
		{
			return;
		}
		QFile file(filePath);
		file.open(QIODevice::WriteOnly);
		cryptoWidget->savePublicKey(&file);
		file.close();
	}
}

void MainWindow::onSaveSecretKey()
{
	if (cryptoWidget)
	{
		QString filePath = QFileDialog::getSaveFileName(this, tr("Save secret key"),
													 QDir::currentPath());
		if (filePath.isEmpty())
		{
			return;
		}
		QFile file(filePath);
		file.open(QIODevice::WriteOnly);
		cryptoWidget->saveSecretKey(&file);
		file.close();
	}
}

void MainWindow::onLoadPublicKey()
{
	if (cryptoWidget)
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open public key"),
													 QDir::currentPath());
		if (filePath.isEmpty())
		{
			return;
		}
		QFile file(filePath);
		file.open(QIODevice::ReadOnly);
		if (cryptoWidget->loadPublicKey(&file))
		{
			cryptoWidget->updateGui();
		}
		file.close();
	}
}

void MainWindow::onLoadSecretKey()
{
	if (cryptoWidget)
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open secret key"),
													 QDir::currentPath());
		if (filePath.isEmpty())
		{
			return;
		}
		QFile file(filePath);
		file.open(QIODevice::ReadOnly);
		if (cryptoWidget->loadSecretKey(&file))
		{
			cryptoWidget->updateGui();
		}
		file.close();
	}
}

void MainWindow::onSignFile()
{
	if (!mayBeSigned())
	{
		return;
	}
	QFile file(labelForCurrentFile->text());
	file.open(QIODevice::ReadOnly);
	SignatureInterface *signature = cryptoWidget->signature();
	signature->setInputDevice(&file);
	if (!signature->calculateSign())
	{
		return;
	}
	signWidget->setText(signature->getSign());
	file.close();
}

bool MainWindow::mayBeSigned()
{
	return  !(labelForCurrentFile->text().isEmpty()) &&
			cryptoWidget->signature()->getParameters() &&
			!(cryptoWidget->signature()->getSecretKey().is_null());
}

void MainWindow::onVerifySign()
{
	if (!mayBeVerifyed())
	{
		return;
	}
	QFile file(labelForCurrentFile->text());
	file.open(QIODevice::ReadOnly);
	SignatureInterface *signature = cryptoWidget->signature();
	signature->setInputDevice(&file);
	signature->setSign(signWidget->text());
	QMessageBox::information(this, windowTitle(),
							 tr("This sign is %1.")
								.arg(signature->verifySign() ?
										 tr("correct") :
										 tr("incorrect")));
	file.close();
}

bool MainWindow::mayBeVerifyed()
{
	return  !(labelForCurrentFile->text().isEmpty()) &&
			cryptoWidget->signature()->getParameters() &&
			!(signWidget->text().isEmpty()) &&
			!(cryptoWidget->signature()->getPublicKey().is_null());
}
