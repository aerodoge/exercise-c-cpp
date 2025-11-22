#pragma once

#include "Deck.h"
#include "Player.h"
#include <memory>
#include <vector>

namespace Poker {

class Game {
public:
    Game();

    // 开始游戏
    void start();

    // 玩一轮游戏
    void play_round();

    // 玩多轮游戏
    void play_multiple_rounds(int numRounds);

    // 显示统计信息
    void show_statistics() const;

private:
    Deck deck_;
    std::unique_ptr<HumanPlayer> human_player_;
    std::unique_ptr<AIPlayer> ai_player_;
    int human_wins_;
    int ai_wins_;
    int ties_;

    // 发牌给所有玩家
    void deal_cards();

    // 玩家换牌阶段
    void replace_cards_phase();

    // 显示结果
    void showdown();

    // 显示分隔线
    void print_separator() const;
};

} // namespace Poker
