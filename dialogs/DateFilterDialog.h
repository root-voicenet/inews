#ifndef DATEFILTERDIALOG_H
#define DATEFILTERDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class DateFilterDialog;
}

class DateFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DateFilterDialog(const QDate& start, const QDate& end, QWidget *parent = 0);
    ~DateFilterDialog();
    
    QDate getStartDate() const;
    QDate getEndDate() const;
private:
    Ui::DateFilterDialog *ui;
};

#endif // DATEFILTERDIALOG_H
