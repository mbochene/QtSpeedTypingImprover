#include "abstractkeyboardhandler.h"

AbstractKeyboardHandler::AbstractKeyboardHandler()
{

}

QString AbstractKeyboardHandler::handleKeyboardPress(const QString& pressedKey, const bool& altPressed) const
{
    QString pressedKeyLower = pressedKey.toLower();
    if(pressedKeyLower.compare(a) >= 0 && pressedKeyLower.compare(z) <= 0)
    {
        if(!altPressed)
            return pressedKeyLower;
        else
            return handleSpecialCharacter(pressedKeyLower);
    }
    else if(pressedKeyLower.compare(enter) == 0 || pressedKeyLower.compare(dash) == 0 || pressedKeyLower.compare(apostrophe) == 0)
    {
        return pressedKeyLower;
    }
    return QString("");
}
