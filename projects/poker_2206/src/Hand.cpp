#include "Hand.h"
#include <algorithm>
#include <sstream>

namespace Poker {

Hand::Hand(const std::vector<Card>& cards) : cards_(cards) {}

void Hand::add_card(const Card& card) {
    if (!is_full()) {
        cards_.push_back(card);
    }
}

void Hand::replace_card(size_t index, const Card& card) {
    if (index < cards_.size()) {
        cards_[index] = card;
    }
}

void Hand::remove_card(const size_t index) {
    if (index < cards_.size()) {
        cards_.erase(cards_.begin() + index);
    }
}

void Hand::clear() {
    cards_.clear();
}

std::string Hand::to_string() const {
    std::ostringstream oss;
    for (size_t i = 0; i < cards_.size(); ++i) {
        oss << cards_[i].to_string();
        if (i < cards_.size() - 1) {
            oss << "\n";
        }
    }
    return oss.str();
}

void Hand::sort_by_rank() {
    std::sort(cards_.begin(), cards_.end(), [](const Card& a, const Card& b) {
        return a.get_rank() < b.get_rank();
    });
}

} // namespace Poker
