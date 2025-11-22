#pragma once

#include "Card.h"
#include <array>
#include <vector>
#include <optional>
#include <random>

namespace Poker {

class Deck {
public:
    // size_t 是一个无符号整数类型（unsigned integer type），定义在 <cstddef> 头文件中。
    // 它的具体大小取决于平台：
    // - 32位系统：通常是32位（4字节）
    // - 64位系统：通常是64位（8字节）
    static constexpr size_t DECK_SIZE = 52;
    static constexpr size_t NUM_SUITS = 4;
    static constexpr size_t NUM_RANKS = 13;

    Deck();

    // 洗牌（Fisher-Yates shuffle）算法
    void shuffle();

    // 发一张牌
    [[nodiscard]] std::optional<Card> deal_card();

    // 发多张牌
    [[nodiscard]] std::vector<Card> deal_cards(size_t count);

    // 重置牌堆
    void reset();

    // 获取剩余牌数
    [[nodiscard]] size_t remaining_cards() const noexcept { return DECK_SIZE - currentIndex_; }

    // 检查是否还有牌
    [[nodiscard]] bool has_cards() const noexcept { return currentIndex_ < DECK_SIZE; }

private:
    std::array<Card, DECK_SIZE> cards_;
    size_t currentIndex_;
    std::mt19937 rng_;

    void init_deck();
};

} // namespace Poker
