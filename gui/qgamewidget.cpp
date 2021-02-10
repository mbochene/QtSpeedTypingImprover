#include "qgamewidget.h"

QGameWidget::QGameWidget(QWidget *parent) : QWidget(parent)
{

}

bool QGameWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        emit keyClicked(static_cast<QKeyEvent *>(event));
        return true;
    }
    return QWidget::event(event);
}
