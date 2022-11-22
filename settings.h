#ifndef SETTINGS_H
#define SETTINGS_H

#include <QList>
#include <QString>

typedef struct {
        QString name;
        QString time;
        QString dtim;
} RESULT_t;

typedef struct {
        int gameType;
        unsigned int closedColor;
        unsigned int openedColor;
        bool showToolbar;
        bool enableSounds;
        QList<RESULT_t> top[3];
} SETTINGS_t;

#define STORE_GAME_TYPE     "Game/GameType"
#define STORE_CLOSED_COLOR  "Settings/ClosedColor"
#define STORE_OPENED_COLOR  "Settings/OpenedColor"
#define STORE_SHOW_TOOLBAR	"Settings/ShowToolbar"
#define STORE_ENABLE_SOUNDS	"Settings/EnableSounds"
#define STORE_GEOMETRY      "Settings/Geometry"

extern SETTINGS_t g_Settings;

#endif // SETTINGS_H
