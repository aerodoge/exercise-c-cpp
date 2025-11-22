#include "Deck.h"
#include <chrono>

namespace Poker {

Deck::Deck()
    : currentIndex_(0),
      rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {
    init_deck();
}

void Deck::init_deck() {
    size_t index = 0;
    for (int suit = 0; suit < static_cast<int>(NUM_SUITS); ++suit) {
        for (int rank = 1; rank <= static_cast<int>(NUM_RANKS); ++rank) {
            cards_[index++] = Card(static_cast<Suit>(suit), static_cast<Rank>(rank));
        }
    }
}

void Deck::shuffle() {
    // Fisher-Yates 洗牌算法 - O(n) 时间复杂度，性能优异
    for (size_t i = DECK_SIZE - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = dist(rng_);
        std::swap(cards_[i], cards_[j]);
    }
    currentIndex_ = 0;
}

std::optional<Card> Deck::deal_card() {
    if (!has_cards()) {
        return std::nullopt;
    }
    return cards_[currentIndex_++];
}

std::vector<Card> Deck::deal_cards(size_t count) {
    std::vector<Card> result;
    result.reserve(count);

    for (size_t i = 0; i < count && has_cards(); ++i) {
        if (auto card = deal_card()) {
            result.push_back(*card);
        }
    }

    return result;
}

void Deck::reset() {
    currentIndex_ = 0;
    init_deck();
}

} // namespace Poker
