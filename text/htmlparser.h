#ifndef NvHTMLPARSER_H
#define NvHTMLPARSER_H

#include "QtCore/qvector.h"
#include "QtGui/qcolor.h"
#include "QtGui/qfont.h"
#include <QHash>
#include "QtGui/qtextdocument.h"
#include "QtGui/qtextcursor.h"
#include "cssparser.h"

//#include "private/qtexthtmlparser_p.h"

/**
*/

//using namespace QCss;

enum NvTextHTMLElements {
    Html_qt,
    Html_body,

    Html_a,
    Html_em,
    Html_i,
    Html_big,
    Html_small,
    Html_strong,
    Html_b,
    Html_cite,
    Html_address,
    Html_var,
    Html_dfn,

    Html_h1,
    Html_h2,
    Html_h3,
    Html_h4,
    Html_h5,
    Html_h6,
    Html_p,
    Html_center,

    Html_font,

    Html_ul,
    Html_ol,
    Html_li,

    Html_code,
    Html_tt,
    Html_kbd,
    Html_samp,

    Html_img,
    Html_br,
    Html_hr,
    Html_comment_split,      ///my code

    Html_sub,
    Html_sup,

    Html_pre,
    Html_blockquote,
    Html_head,
    Html_div,
    Html_span,
    Html_dl,
    Html_dt,
    Html_dd,
    Html_u,
    Html_s,
    Html_nobr,

    // tables
    Html_table,
    Html_tr,
    Html_td,
    Html_th,
    Html_thead,
    Html_tbody,
    Html_tfoot,
    Html_html,

    // misc...
    Html_style,
    Html_title,
    Html_meta,
    Html_link,

    Html_NumElements
};

struct NvTextHtmlElement {
    const char *name;
    int id;
    enum DisplayMode { DisplayBlock, DisplayInline, DisplayTable, DisplayNone } displayMode;
};

class NvTextHtmlParser;

enum NvTriState { Off = 0, On = 1, Unspecified = 2 };

/**
 this structure stores attributes of a document fragment, which are derived from the parsed html text.
 */
struct NvTextHtmlParserNode {
    enum WhiteSpaceMode { WhiteSpaceNormal, WhiteSpacePre, WhiteSpaceNoWrap,
                          WhiteSpacePreWrap, WhiteSpaceModeUndefined = -1
                        };

    NvTextHtmlParserNode();
    QString tag;
    QString text;
    QStringList attributes;
    int parent;
    QVector<int> children;
    int id;
uint isBlock :
    1;
uint isListItem :
    1;
uint isListStart :
    1;
uint isTableCell :
    1;
uint isAnchor :
    1;

uint fontItalic :
    2; // Tristate
uint fontUnderline :
    2; // Tristate
uint fontOverline :
    2; // Tristate
uint fontStrikeOut :
    2; // Tristate
uint fontFixedPitch :
    2; // Tristate
uint cssFloat :
    2;
uint hasOwnListStyle :
    1;
uint hasFontPointSize :
    1;
uint hasFontPixelSize :
    1;
uint hasFontSizeAdjustment :
    1;
uint hasCssBlockIndent :
    1;
uint hasCssListIndent :
    1;

uint isEmptyParagraph :
    1;
uint isTextFrame :
    1;
uint direction :
    2; // 3 means unset
uint displayMode :
    3; // QTextHtmlElement::DisplayMode
    QTextFormat::PageBreakFlags pageBreakPolicy;

    QString fontFamily;
    int fontPointSize;
    int fontPixelSize;
    int fontSizeAdjustment;
    int fontWeight;

    QBrush foreground;
    QBrush background;
    Qt::Alignment alignment;
    QTextCharFormat::VerticalAlignment verticalAlignment;
    QTextListFormat::Style listStyle;
    QString anchorHref;
    QString anchorName;

    QString anchorTitle;  ///my code
    QString anchorTarget;  ///my code

    QString imageName;
    qreal imageWidth;
    qreal imageHeight;

    QString imageTitle;   ///my code
    QString imageAlternateText;   ///my code

    QTextLength width;
    QTextLength height;

    qreal tableBorder;
    int tableCellRowSpan;
    int tableCellColSpan;
    qreal tableCellSpacing;
    qreal tableCellPadding;

    int cssBlockIndent;
    int cssListIndent;
    qreal text_indent;

    QTextCharFormat charFormat() const;
    QTextBlockFormat blockFormat() const;

    WhiteSpaceMode wsm;

    bool hasCodeStyle;   ///my code

    int htmlHeading;  ///my code
    bool isBlockQuote;  ///my code

    bool isHtmlTagSign;  ///my code

    inline bool isNotSelfNesting() const {
        return id == Html_p || id == Html_li;
    }

    inline bool allowedInContext( int parentId ) const {
        switch ( id ) {
            case Html_dd:
                return ( parentId == Html_dt || parentId == Html_dl );
            case Html_dt:
                return ( parentId == Html_dl );
            case Html_tr:
                return ( parentId == Html_table || parentId == Html_thead || parentId == Html_tbody || parentId == Html_tfoot );
            case Html_th:
            case Html_td:
                return ( parentId == Html_tr );
            case Html_thead:
            case Html_tbody:
            case Html_tfoot:
                return ( parentId == Html_table );
            default:
                break;
        }
        return true;
    }

    inline bool mayNotHaveChildren() const {
//         return id == Html_img || id == Html_hr || id == Html_br;
        return id == Html_img || id == Html_hr || id == Html_br || id == Html_comment_split ;
    }

    void initializeProperties( const NvTextHtmlParserNode *parent, const NvTextHtmlParser *parser );
    inline int uncollapsedMargin( int mar ) const {
        return margin[mar];
    }
    bool isNestedList( const NvTextHtmlParser *parser ) const;
    void applyCssDeclarations( const QVector<QCss::Declaration> &declarations, const QTextDocument *resrouceProvider );
    int margin[4];

    enum FontAttributes {
        Family        = 0x0001,
        Size          = 0x0002,
        StyleHint     = 0x0004,
        StyleStrategy = 0x0008,
        Weight        = 0x0010,
        Style         = 0x0020,
        Underline     = 0x0040,
        Overline      = 0x0080,
        StrikeOut     = 0x0100,
        FixedPitch    = 0x0200,
        Stretch       = 0x0400,
        Kerning       = 0x0800,
        Complete      = 0x0fff
    };


    friend class NvTextHtmlParser;
};
Q_DECLARE_TYPEINFO( NvTextHtmlParserNode, Q_MOVABLE_TYPE );
//

/**
 This class parses a given html text, and creates a list of nodes with certain attributes. Type of the nodes is NvTextHtmlParserNode structure.
 */
class NvTextHtmlParser
{
public:
    enum Margin {
        MarginTop, MarginRight, MarginBottom, MarginLeft
    };

    inline const NvTextHtmlParserNode &at( int i ) const {
        return nodes.at( i );
    }
    inline NvTextHtmlParserNode &operator[]( int i ) {
        return nodes[i];
    }
    inline int count() const {
        return nodes.count();
    }
    inline int last() const {
        return nodes.count() - 1;
    }
    int depth( int i ) const;
    int topMargin( int i ) const;
    int bottomMargin( int i ) const;
    inline int leftMargin( int i ) const {
        return margin( i, MarginLeft );
    }
    inline int rightMargin( int i ) const {
        return margin( i, MarginRight );
    }

    void dumpHtml();

    void parse( const QString &text, const QTextDocument *resourceProvider );

    static int lookupElement( const QString &element );
protected:
    NvTextHtmlParserNode *newNode( int parent );
    QVector<NvTextHtmlParserNode> nodes;
    QString txt;
    int pos, len;

    bool textEditMode;

    void parse();
    void parseTag();
    void parseCloseTag();
//     void parseExclamationTag();
    bool parseExclamationTag();         ///my code
    QString parseEntity();
    QString parseWord();
    void resolveParent();
    void resolveNode();
    QStringList parseAttributes();
    void applyAttributes( const QStringList &attributes );
    void eatSpace();
    inline bool hasPrefix( QChar c, int lookahead = 0 ) const {
        return pos + lookahead < len && txt.at( pos ) == c;
    }
    int margin( int i, int mar ) const;

    QVector<QCss::Declaration> declarationsForNode( int node ) const;
    void resolveStyleSheetImports( const QCss::StyleSheet &sheet );
    void importStyleSheet( const QString &href );

    QHash<QString, QCss::StyleSheet> externalStyleSheets;
    QList<QCss::StyleSheet> inlineStyleSheets;
    const QTextDocument *resourceProvider;
};


#endif
