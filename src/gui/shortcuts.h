#pragma once

#include <QKeyEvent>

namespace fastoredis
{
    bool isCloseTabShortcut(QKeyEvent *keyEvent);
    bool isNewTabShortcut(QKeyEvent *keyEvent);
    bool isDuplicateTabShortcut(QKeyEvent *keyEvent);
    bool isSetFocusOnQueryLineShortcut(QKeyEvent *keyEvent);
    bool isExecuteScriptShortcut(QKeyEvent *keyEvent);
    bool isAutoCompleteShortcut(QKeyEvent *keyEvent);
    bool isHideAutoCompleteShortcut(QKeyEvent *keyEvent);
    bool isNextTabShortcut(QKeyEvent *keyEvent);
    bool isPreviousTabShortcut(QKeyEvent *keyEvent);
    bool isToggleCommentsShortcut(QKeyEvent *keyEvent);
}
