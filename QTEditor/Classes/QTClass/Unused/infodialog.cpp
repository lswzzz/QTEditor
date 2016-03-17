#include "infodialog.h"
#include "ui_infodialog.h"
#include "Global/Global.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
	//SetLayeredWindowAttributes((HWND)this->winId(), RGB(0, 0, 0), 0, LWA_COLORKEY);
	this->setWindowOpacity(1);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::test()
{
	
}

void InfoDialog::mousePressEvent(QMouseEvent *e)
{
	addConsoleInfo("YES");
}