#include <stdio.h>
#include <vector>
#include <tgbot/tgbot.h>
#include <map>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <Windows.h>
#include <codecvt>

struct GameSession {
    // Тут можна додати поля для зберігання стану гри, наприклад, карту гри
};

// Функція для створення нової ігрової сесії
GameSession createNewGameSession() {
    GameSession newSession;
    // Тут можна ініціалізувати нову сесію, наприклад, створити нову карту гри
    return newSession;
}

struct MapCell {
    int id;  // ID клітинки
    // Тут можна додати додаткові поля, наприклад, опис ресурсів
};

// Структура для представлення мапи гри
struct GameMap {
    int width;  // Ширина мапи
    int height;  // Висота мапи
    std::vector<std::vector<MapCell>> cells;  // Матриця клітинок

    // Конструктор, який створює мапу з заданою шириною і висотою
    GameMap(int width, int height) : width(width), height(height) {
        cells.resize(height, std::vector<MapCell>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cells[y][x].id = y * width + x;
                // Тут можна ініціалізувати додаткові поля клітинки
            }
        }
    }
};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::vector<std::string> bot_commands = { "start", "test", "begin", "createmap"}; // список команд бота

    TgBot::Bot bot("6516537239:AAFrdbhYujv8rQc-7Mu9Cpo8ak0yhyui_hU");

    // Контейнер для зберігання ігрових сесій. Ключ - це ID користувача
    std::map<int64_t, GameSession> gameSessions;

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привіт!");
        });

    bot.getEvents().onCommand("begin", [&bot, &gameSessions](TgBot::Message::Ptr message) {
        int64_t userId = message->chat->id;
        gameSessions[userId] = createNewGameSession();
        bot.getApi().sendMessage(userId, "Ігрову сесію створено!");
        });

    bot.getEvents().onCommand("createmap", [&bot, &gameSessions](TgBot::Message::Ptr message) {
        std::istringstream iss(message->text);
        std::string command;
        int width, height;
        if (!(iss >> command >> width >> height)) {
            bot.getApi().sendMessage(message->chat->id, "Неправильний формат команди. Використовуйте: /createmap ширина висота");
            return;
        }
        if (width < 1 || width > 16 || height < 1 || height > 26) {
            bot.getApi().sendMessage(message->chat->id, "Неправильні розміри мапи. Ширина має бути від 1 до 16, висота - від 1 до 26");
            return;
        }
        GameMap map(width, height);
        bot.getApi().sendMessage(message->chat->id, "Мапу створено!");
        });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}