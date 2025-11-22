#pragma once

#include <string>

namespace Poker {

enum class Suit {  // 花色
    Hearts = 0,    // 红心
    Diamonds = 1,  // 方块
    Clubs = 2,     // 梅花
    Spades = 3     // 黑桃
};

enum class Rank { // 点数
    Ace = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12,
    King = 13
};

class Card {
public:
    Card() = default;
    Card(Suit suit, Rank rank);
    // 花色
    [[nodiscard]] Suit get_suit() const noexcept { return suit_; }
    // 点书
    [[nodiscard]] Rank get_rank() const noexcept { return rank_; }
    // 转成字符串
    [[nodiscard]] std::string to_string() const;
    // 字符串花色
    [[nodiscard]] std::string get_suit_string() const;
    // 字符串点数
    [[nodiscard]] std::string get_rank_string() const;

    // C++20 three-way comparison
    auto operator<=>(const Card& other) const noexcept = default;
    bool operator==(const Card& other) const noexcept = default;

private:
    Suit suit_ = Suit::Hearts;
    Rank rank_ = Rank::Ace;
};

} // namespace Poker
