#include "HandComparator.h"
#include <algorithm>

namespace Poker {

std::string HandComparator::result_to_string(ComparisonResult result) {
    switch (result) {
        case ComparisonResult::Hand1Wins: return "玩家1获胜";
        case ComparisonResult::Hand2Wins: return "玩家2获胜";
        case ComparisonResult::Tie:       return "平局";
    }
    return "未知";
}

Card HandComparator::get_highest_suit_card(const Hand& hand, Rank rank) {
    Card highestCard = hand.get_cards()[0];
    bool found = false;

    for (const auto& card : hand.get_cards()) {
        if (card.get_rank() == rank) {
            if (!found || card.get_suit() > highestCard.get_suit()) {
                highestCard = card;
                found = true;
            }
        }
    }

    return highestCard;
}

ComparisonResult HandComparator::compare_kickers(
    const std::vector<Rank>& kickers1,
    const std::vector<Rank>& kickers2,
    const Hand& hand1,
    const Hand& hand2
) {
    size_t minSize = std::min(kickers1.size(), kickers2.size());

    for (size_t i = 0; i < minSize; ++i) {
        if (kickers1[i] > kickers2[i]) {
            return ComparisonResult::Hand1Wins;
        } else if (kickers1[i] < kickers2[i]) {
            return ComparisonResult::Hand2Wins;
        }
    }

    // 如果点数完全相同，比较花色
    // 获取最大点数的牌
    if (!kickers1.empty()) {
        Card card1 = get_highest_suit_card(hand1, kickers1[0]);
        Card card2 = get_highest_suit_card(hand2, kickers2[0]);

        if (card1.get_suit() > card2.get_suit()) {
            return ComparisonResult::Hand1Wins;
        } else if (card1.get_suit() < card2.get_suit()) {
            return ComparisonResult::Hand2Wins;
        }
    }

    return ComparisonResult::Tie;
}

ComparisonResult HandComparator::compare_evaluations(
    const HandEvaluation& eval1,
    const HandEvaluation& eval2,
    const Hand& hand1,
    const Hand& hand2
) {
    // 首先比较牌型
    if (eval1.rank > eval2.rank) {
        return ComparisonResult::Hand1Wins;
    } else if (eval1.rank < eval2.rank) {
        return ComparisonResult::Hand2Wins;
    }

    // 牌型相同，比较kickers
    return compare_kickers(eval1.kickers, eval2.kickers, hand1, hand2);
}

ComparisonResult HandComparator::compare(const Hand& hand1, const Hand& hand2) {
    HandEvaluation eval1 = HandEvaluator::evaluate(hand1);
    HandEvaluation eval2 = HandEvaluator::evaluate(hand2);

    return compare_evaluations(eval1, eval2, hand1, hand2);
}

} // namespace Poker
