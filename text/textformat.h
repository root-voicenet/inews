#ifndef NvTEXTFORMAT_H
#define NvTEXTFORMAT_H

#include <qtextformat.h>


// class NvTextCharFormat : public QTextCharFormat

class NvTextFormat
{
public:

    enum Property {
        /// Anchor properties
        AnchorTitle = 0x100010,
        AnchorTarget = 0x100011,

        /// Image properties
        ImageTitle = 0x100020,
        ImageAlternateText = 0x100021,
        
        HasCodeStyle = 0x100030,

        /// Block Properties
        HtmlHeading = 0x100040, //zero if block is not in heading format, 1 for Heading 1, and so on.
        IsBlockQuote = 0x100041,

        IsHtmlTagSign = 0x100042
    };

};

#endif
