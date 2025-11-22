#include "Card.h"
#include <format>

namespace Poker {

Card::Card(const Suit suit, const Rank rank)
    : suit_(suit), rank_(rank) {}

std::string Card::get_suit_string() const {
    switch (suit_) {
        case Suit::Hearts:   return "红桃";
        case Suit::Diamonds: return "方块";
        case Suit::Clubs:    return "梅花";
        case Suit::Spades:   return "黑桃";
    }
    return "未知花色";
}

std::string Card::get_rank_string() const {
    switch (rank_) {
        case Rank::Ace:   return "A";
        case Rank::Two:   return "2";
        case Rank::Three: return "3";
        case Rank::Four:  return "4";
        case Rank::Five:  return "5";
        case Rank::Six:   return "6";
        case Rank::Seven: return "7";
        case Rank::Eight: return "8";
        case Rank::Nine:  return "9";
        case Rank::Ten:   return "10";
        case Rank::Jack:  return "J";
        case Rank::Queen: return "Q";
        case Rank::King:  return "K";
    }
    return "未知点数";
}

std::string Card::to_string() const {
    // {:>5} - 右对齐，宽度为5个字符
    // {:<8} - 左对齐，宽度为8个字符
    return std::format("{} {}", get_rank_string(), get_suit_string());
}

} // namespace Poker
