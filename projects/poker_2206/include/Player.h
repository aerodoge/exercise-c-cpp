#pragma once

#include "Hand.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include <string>
#include <vector>

namespace Poker {

class Player {
public:
    explicit Player(const std::string& name);
    virtual ~Player() = default;

    [[nodiscard]] const std::string& get_name() const noexcept { return name_; }
    [[nodiscard]] Hand& get_hand() noexcept { return hand_; }
    [[nodiscard]] const Hand& get_hand() const noexcept { return hand_; }

    // 决定要换哪些牌（返回要换掉的牌的索引）
    virtual std::vector<size_t> decide_cards_to_replace() = 0;

    // 显示手牌
    void show_hand(bool hideCards = false) const;

protected:
    std::string name_;
    Hand hand_;
};

// 人类玩家
class HumanPlayer : public Player {
public:
    explicit HumanPlayer(const std::string& name);

    std::vector<size_t> decide_cards_to_replace() override;
};

// AI玩家（庄家）
class AIPlayer : public Player {
public:
    explicit AIPlayer(const std::string& name);

    std::vector<size_t> decide_cards_to_replace() override;

private:
    // 根据牌的质量决定换牌策略
    std::vector<size_t> analyze_hand();

    // 检查是否接近同花
    [[nodiscard]] bool is_almost_flush() const;

    // 检查是否接近顺子
    [[nodiscard]] bool is_almost_straight() const;
};

} // namespace Poker
