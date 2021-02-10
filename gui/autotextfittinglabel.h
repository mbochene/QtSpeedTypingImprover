#ifndef AUTOTEXTFITTINGLABEL_H
#define AUTOTEXTFITTINGLABEL_H

#include <QLabel>

class AutoTextFittingLabel : public QLabel
{
    Q_OBJECT
    public:
        AutoTextFittingLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        AutoTextFittingLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    public slots:
        void setText(const QString &);

};

#endif // AUTOTEXTFITTINGLABEL_H
