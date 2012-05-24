#include "DateFilterDialog.h"
#include "ui_datefilterdialog.h"

DateFilterDialog::DateFilterDialog(const QDate &start, const QDate &end, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateFilterDialog)
{
    ui->setupUi(this);
    ui->calStart->setFirstDayOfWeek(Qt::Monday);
    ui->calStart->setSelectedDate(start);

    ui->calEnd->setFirstDayOfWeek(Qt::Monday);
    ui->calEnd->setSelectedDate(end);
}

DateFilterDialog::~DateFilterDialog()
{
    delete ui;
}

QDate DateFilterDialog::getStartDate() const
{
    return ui->calStart->selectedDate();
}

QDate DateFilterDialog::getEndDate() const
{
    return ui->calEnd->selectedDate();
}
