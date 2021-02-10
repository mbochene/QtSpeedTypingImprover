#ifndef ABSTRACTKEYBOARDHANDLER_H
#define ABSTRACTKEYBOARDHANDLER_H

#include <QString>

class AbstractKeyboardHandler
{
    public:
        AbstractKeyboardHandler();
        virtual ~AbstractKeyboardHandler() {}

        QString handleKeyboardPress(const QString& pressedKey) const;

    protected:
        virtual QString checkIfSpecialCharacter(const QString& character) const = 0;

    private:
        const QString enter = QString("\r");
        const QChar a = QChar('a');
        const QChar z = QChar('z');
        const QChar apostrophe = QChar('\'');
        const QChar dash = QChar('-');
};

#endif // ABSTRACTKEYBOARDHANDLER_H
