#ifndef POLISHKEYBOARDHANDLER_H
#define POLISHKEYBOARDHANDLER_H

#include "abstractkeyboardhandler.h"
#include <QVector>

class PolishKeyboardHandler : public AbstractKeyboardHandler
{
    public:
        PolishKeyboardHandler();

    protected:
        virtual QString checkIfSpecialCharacter(const QString& character) const override;

    private:
        QVector<QString> specialChars = {"ą", "ć", "ę", "ł", "ń", "ó", "ś", "ź", "ż"};
};


#endif // POLISHKEYBOARDHANDLER_H
