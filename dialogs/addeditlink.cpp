#include "addeditlink.h"
#include "ui_linkdialog.h"

AddEditLink::AddEditLink(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinkDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->btnClear->setIcon(QIcon("edit-clear"));
    ui->txtAddress->setFocus();
    connect(ui->btnClear, SIGNAL(clicked(bool)), SLOT(slotClearLinkCache()) );
}

AddEditLink::~AddEditLink()
{
    delete ui;
}

void AddEditLink::show(const QString &address, const QString &title, const QString &target)
{
    QDialog::show();
    if ( address.isEmpty() ) {
           ui->txtAddress->insertItem(0, QString());
           ui->txtAddress->setCurrentIndex(0);
           this->setWindowTitle( tr( "verb, to insert a link into the text", "Add Link" ) );
       } else {
            int index = ui->txtAddress->findText( address );
            ui->txtAddress->setCurrentIndex(index);
            this->setWindowTitle( tr( "verb, to modify an existing link", "Edit Link" ) );
       }

       if ( !title.isEmpty() ) {
           ui->txtTitle->setText( title );
       }

       if ( !target.isEmpty() ) {
           if ( target == "_self" ) {
               ui->comboTarget->setCurrentIndex( 1 );
           } else if ( target == "_blank" ) {
               ui->comboTarget->setCurrentIndex( 2 );
           }
       }
}

void AddEditLink::done(int button)
{
    QDialog::done(button);

    QString link = ui->txtAddress->currentText();

    if ( link.isEmpty() )
        return;
    QString linkTarget;
    if ( ui->comboTarget->currentIndex() == 1 ) {
        linkTarget = "_self";
    } else if ( ui->comboTarget->currentIndex() == 2 ) {

        linkTarget = "_blank";
    }
    const QString target = linkTarget;

    emit addLink( link, target, ui->txtTitle->text() );

}

void AddEditLink::slotClearLinkCache()
{

}
