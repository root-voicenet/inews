#include "mediatitledialog.h"
#include "ui_mediatitledialog.h"

MediaTitleDialog::MediaTitleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MediaTitleDialog)
{
    ui->setupUi(this);
}

MediaTitleDialog::~MediaTitleDialog()
{
    delete ui;
}

void MediaTitleDialog::setTitle(const QString &title)
{
    ui->titleEdit->setText(title);
}

void MediaTitleDialog::setDescription(const QString &description)
{
    ui->descEdit->setPlainText(description);
}

QString MediaTitleDialog::getTitle() const
{
    return ui->titleEdit->text();
}

QString MediaTitleDialog::getDescription() const
{
    QTextDocument *doc = ui->descEdit->document();
    return doc->toPlainText();
}
