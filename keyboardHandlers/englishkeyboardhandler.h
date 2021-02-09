#ifndef ENGLISHKEYBOARDHANDLER_H
#define ENGLISHKEYBOARDHANDLER_H

#include "abstractkeyboardhandler.h"

class EnglishKeyboardHandler : public AbstractKeyboardHandler
{
    public:
        EnglishKeyboardHandler();

    protected:
        virtual QString handleSpecialCharacter(const QString& character) const override;
};

#endif // ENGLISHKEYBOARDHANDLER_H
