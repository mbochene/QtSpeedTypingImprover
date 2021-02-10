#include "autotextfittinglabel.h"

AutoTextFittingLabel::AutoTextFittingLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) : QLabel(text, parent, f)
{
}

AutoTextFittingLabel::AutoTextFittingLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f)
{
}

void AutoTextFittingLabel::setText(const QString & text)
{
    QFont f = this->font(); //Get label font
    const QFontMetrics metrics(f);
    const QSize size = metrics.size(0, text); //Get size of text
    const qreal widthFactor = static_cast<qreal>(this->width()) / static_cast<qreal>(size.width()); //Get the width factor
    const qreal heightFactor =  static_cast<qreal>(this->height()) / static_cast<qreal>(size.height()); //Get the height factor
    const qreal factor = qMin(widthFactor, heightFactor); //To fit contents in the screen select as factor
                                                          //the minimum factor between width and height
    f.setPointSizeF(0.8 * f.pointSizeF() * factor); //Set font size
    setFont(f);
    QLabel::setText(text);
}
