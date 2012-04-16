#ifndef NvTEXTHTMLIMPORTER_H
#define NvTEXTHTMLIMPORTER_H

#include "QtGui/qtextdocument.h"
#include "QtGui/qtexttable.h"
//#include "QtCore/qatomic.h"
#include "QtCore/qlist.h"
//#include "QtCore/qmap.h"
#include "QtCore/qpointer.h"

#include "htmlparser.h"

/**
 *  Parses html text via NvTextHtmlParser functions, then organizes returned
 *  node list as a QTextDocument object.
 *
*/
class NvTextHtmlImporter : public NvTextHtmlParser
{
    struct Table;
public:
    NvTextHtmlImporter( QTextDocument *_doc, const QString &html, const QTextDocument *resourceProvider = 0 );

    void import();

    bool containsCompleteDocument() const {
        return containsCompleteDoc;
    }

private:
    bool closeTag( int i );

    Table scanTable( int tableNodeIdx );

    void appendBlock( const QTextBlockFormat &format, QTextCharFormat charFmt = QTextCharFormat() );

    struct List {
        QTextListFormat format;
        QPointer<QTextList> list;
    };
    QVector<List> lists;
    int indent;

    // insert a named anchor the next time we emit a char format,
    // either in a block or in regular text
    bool setNamedAnchorInNextOutput;
    QString namedAnchor;

#ifdef Q_CC_SUN
    friend struct NvTextHtmlImporter::Table;
#endif
    struct TableCellIterator {
        inline TableCellIterator( QTextTable *t = 0 ) : table( t ), row( 0 ), column( 0 ) {}

        inline TableCellIterator &operator++() {
            do {
                const QTextTableCell cell = table->cellAt( row, column );
                if ( !cell.isValid() )
                    break;
                column += cell.columnSpan();
                if ( column >= table->columns() ) {
                    column = 0;
                    ++row;
                }
            } while ( row < table->rows() && table->cellAt( row, column ).row() != row );

            return *this;
        }

        inline bool atEnd() const {
            return table == 0 || row >= table->rows();
        }

        QTextTableCell cell() const {
            return table->cellAt( row, column );
        }

        QTextTable *table;
        int row;
        int column;
    };

    struct Table {
        Table() : isTextFrame( false ), rows( 0 ), columns( 0 ), currentRow( 0 ) {}
        QPointer<QTextFrame> frame;
        bool isTextFrame;
        int rows;
        int columns;
        int currentRow; // ... for buggy html (see html_skipCell testcase)
        TableCellIterator currentCell;
    };
    QVector<Table> tables;

    QTextDocument *doc;
    QTextCursor cursor;
    bool containsCompleteDoc;
};

#endif
