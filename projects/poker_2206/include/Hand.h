#pragma once

#include "Card.h"
#include <vector>
#include <string>

namespace Poker {

class Hand {
public:
    static constexpr size_t HAND_SIZE = 5;

    Hand() = default;
    explicit Hand(const std::vector<Card>& cards);

    // 添加牌
    void add_card(const Card& card);

    // 替换指定位置的牌
    void replace_card(size_t index, const Card& card);

    // 移除指定位置的牌
    void remove_card(size_t index);

    // 清空手牌
    void clear();

    // 获取手牌
    [[nodiscard]] const std::vector<Card>& get_cards() const noexcept { return cards_; }

    // 获取手牌大小
    [[nodiscard]] size_t size() const noexcept { return cards_.size(); }

    // 判断手牌是否已满
    [[nodiscard]] bool is_full() const noexcept { return cards_.size() == HAND_SIZE; }

    // 显示手牌
    [[nodiscard]] std::string to_string() const;

    // 按点数排序手牌
    void sort_by_rank();

private:
    std::vector<Card> cards_;
};

} // namespace Poker
