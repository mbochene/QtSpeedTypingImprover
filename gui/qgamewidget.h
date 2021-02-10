#ifndef QGAMEWIDGET_H
#define QGAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>

class QGameWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit QGameWidget(QWidget *parent = nullptr);

    protected:
        bool event(QEvent *event) override;

    signals:
        void keyClicked(const QKeyEvent* keyEvent);

};

#endif // QGAMEWIDGET_H
