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
    // ��� ����� ������ ���� ��� ��������� ����� ���, ���������, ����� ���
};

// ������� ��� ��������� ���� ������ ���
GameSession createNewGameSession() {
    GameSession newSession;
    // ��� ����� ������������ ���� ����, ���������, �������� ���� ����� ���
    return newSession;
}

struct MapCell {
    int id;  // ID �������
    // ��� ����� ������ �������� ����, ���������, ���� �������
};

// ��������� ��� ������������� ���� ���
struct GameMap {
    int width;  // ������ ����
    int height;  // ������ ����
    std::vector<std::vector<MapCell>> cells;  // ������� �������

    // �����������, ���� ������� ���� � ������� ������� � �������
    GameMap(int width, int height) : width(width), height(height) {
        cells.resize(height, std::vector<MapCell>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cells[y][x].id = y * width + x;
                // ��� ����� ������������ �������� ���� �������
            }
        }
    }
};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::vector<std::string> bot_commands = { "start", "test", "begin", "createmap"}; // ������ ������ ����

    TgBot::Bot bot("6516537239:AAFrdbhYujv8rQc-7Mu9Cpo8ak0yhyui_hU");

    // ��������� ��� ��������� ������� ����. ���� - �� ID �����������
    std::map<int64_t, GameSession> gameSessions;

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "�����!");
        });

    bot.getEvents().onCommand("begin", [&bot, &gameSessions](TgBot::Message::Ptr message) {
        int64_t userId = message->chat->id;
        gameSessions[userId] = createNewGameSession();
        bot.getApi().sendMessage(userId, "������ ���� ��������!");
        });

    bot.getEvents().onCommand("createmap", [&bot, &gameSessions](TgBot::Message::Ptr message) {
        std::istringstream iss(message->text);
        std::string command;
        int width, height;
        if (!(iss >> command >> width >> height)) {
            bot.getApi().sendMessage(message->chat->id, "������������ ������ �������. ��������������: /createmap ������ ������");
            return;
        }
        if (width < 1 || width > 16 || height < 1 || height > 26) {
            bot.getApi().sendMessage(message->chat->id, "���������� ������ ����. ������ �� ���� �� 1 �� 16, ������ - �� 1 �� 26");
            return;
        }
        GameMap map(width, height);
        bot.getApi().sendMessage(message->chat->id, "���� ��������!");
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