#include "polishkeyboardhandler.h"

PolishKeyboardHandler::PolishKeyboardHandler()
{

}

QString PolishKeyboardHandler::checkIfSpecialCharacter(const QString& character) const
{
    return specialChars.indexOf(character) != -1 ? character : QString("");
}
