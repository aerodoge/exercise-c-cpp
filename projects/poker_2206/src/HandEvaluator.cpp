#include "HandEvaluator.h"
#include <algorithm>
#include <set>

namespace Poker {

std::string HandEvaluation::to_string() const {
    switch (rank) {
        case HandRank::HighCard:      return "High Card";
        case HandRank::OnePair:       return "One Pair";
        case HandRank::TwoPair:       return "Two Pair";
        case HandRank::ThreeOfKind:   return "Three of a Kind";
        case HandRank::Straight:      return "Straight";
        case HandRank::Flush:         return "Flush";
        case HandRank::FullHouse:     return "Full House";
        case HandRank::FourOfKind:    return "Four of a Kind";
        case HandRank::StraightFlush: return "Straight Flush";
    }
    return "Unknown";
}

std::map<Rank, int> HandEvaluator::countRanks(const Hand& hand) {
    std::map<Rank, int> counts;
    for (const auto& card : hand.get_cards()) {
        counts[card.get_rank()]++;
    }
    return counts;
}

bool HandEvaluator::allSameSuit(const Hand& hand) {
    if (hand.size() < 2) return false;

    Suit firstSuit = hand.get_cards()[0].get_suit();
    for (const auto& card : hand.get_cards()) {
        if (card.get_suit() != firstSuit) {
            return false;
        }
    }
    return true;
}

bool HandEvaluator::isSequential(const std::vector<Rank>& ranks) {
    if (ranks.size() != Hand::HAND_SIZE) return false;

    std::vector<Rank> sortedRanks = ranks;
    std::sort(sortedRanks.begin(), sortedRanks.end());

    // 检查普通顺子
    for (size_t i = 1; i < sortedRanks.size(); ++i) {
        if (static_cast<int>(sortedRanks[i]) != static_cast<int>(sortedRanks[i-1]) + 1) {
            // 检查 A-2-3-4-5 的特殊顺子
            if (i == sortedRanks.size() - 1 &&
                sortedRanks[0] == Rank::Ace &&
                sortedRanks[1] == Rank::Two &&
                sortedRanks[2] == Rank::Three &&
                sortedRanks[3] == Rank::Four &&
                sortedRanks[4] == Rank::Five) {
                return true;
            }
            return false;
        }
    }
    return true;
}

bool HandEvaluator::hasPair(const Hand& hand) {
    auto counts = countRanks(hand);
    for (const auto& [rank, count] : counts) {
        if (count == 2) return true;
    }
    return false;
}

bool HandEvaluator::hasTwoPair(const Hand& hand) {
    auto counts = countRanks(hand);
    int pairCount = 0;
    for (const auto& [rank, count] : counts) {
        if (count == 2) pairCount++;
    }
    return pairCount == 2;
}

bool HandEvaluator::hasThreeOfKind(const Hand& hand) {
    auto counts = countRanks(hand);
    for (const auto& [rank, count] : counts) {
        if (count == 3) return true;
    }
    return false;
}

bool HandEvaluator::hasFourOfKind(const Hand& hand) {
    auto counts = countRanks(hand);
    for (const auto& [rank, count] : counts) {
        if (count == 4) return true;
    }
    return false;
}

bool HandEvaluator::isFlush(const Hand& hand) {
    return allSameSuit(hand);
}

bool HandEvaluator::isStraight(const Hand& hand) {
    std::vector<Rank> ranks;
    for (const auto& card : hand.get_cards()) {
        ranks.push_back(card.get_rank());
    }
    return isSequential(ranks);
}

bool HandEvaluator::isStraightFlush(const Hand& hand) {
    return isStraight(hand) && isFlush(hand);
}

HandEvaluation HandEvaluator::evaluate(const Hand& hand) {
    if (hand.size() != Hand::HAND_SIZE) {
        return HandEvaluation(HandRank::HighCard);
    }

    auto counts = countRanks(hand);

    // 收集kickers（用于平局比较）
    std::vector<Rank> kickers;
    for (const auto& card : hand.get_cards()) {
        kickers.push_back(card.get_rank());
    }
    std::sort(kickers.begin(), kickers.end(), std::greater<Rank>());

    // 同花顺
    if (isStraightFlush(hand)) {
        return HandEvaluation(HandRank::StraightFlush, kickers);
    }

    // 四条
    if (hasFourOfKind(hand)) {
        std::vector<Rank> fourKindKickers;
        for (const auto& [rank, count] : counts) {
            if (count == 4) {
                fourKindKickers.push_back(rank);
            }
        }
        for (const auto& [rank, count] : counts) {
            if (count == 1) {
                fourKindKickers.push_back(rank);
            }
        }
        return HandEvaluation(HandRank::FourOfKind, fourKindKickers);
    }

    // 葫芦（三带二）
    if (hasThreeOfKind(hand) && hasPair(hand)) {
        std::vector<Rank> fullHouseKickers;
        for (const auto& [rank, count] : counts) {
            if (count == 3) {
                fullHouseKickers.push_back(rank);
            }
        }
        for (const auto& [rank, count] : counts) {
            if (count == 2) {
                fullHouseKickers.push_back(rank);
            }
        }
        return HandEvaluation(HandRank::FullHouse, fullHouseKickers);
    }

    // 同花
    if (isFlush(hand)) {
        return HandEvaluation(HandRank::Flush, kickers);
    }

    // 顺子
    if (isStraight(hand)) {
        return HandEvaluation(HandRank::Straight, kickers);
    }

    // 三条
    if (hasThreeOfKind(hand)) {
        std::vector<Rank> threeKindKickers;
        for (const auto& [rank, count] : counts) {
            if (count == 3) {
                threeKindKickers.push_back(rank);
            }
        }
        for (const auto& [rank, count] : counts) {
            if (count == 1) {
                threeKindKickers.push_back(rank);
            }
        }
        std::sort(threeKindKickers.begin() + 1, threeKindKickers.end(), std::greater<Rank>());
        return HandEvaluation(HandRank::ThreeOfKind, threeKindKickers);
    }

    // 两对
    if (hasTwoPair(hand)) {
        std::vector<Rank> twoPairKickers;
        for (const auto& [rank, count] : counts) {
            if (count == 2) {
                twoPairKickers.push_back(rank);
            }
        }
        std::sort(twoPairKickers.begin(), twoPairKickers.end(), std::greater<Rank>());
        for (const auto& [rank, count] : counts) {
            if (count == 1) {
                twoPairKickers.push_back(rank);
            }
        }
        return HandEvaluation(HandRank::TwoPair, twoPairKickers);
    }

    // 一对
    if (hasPair(hand)) {
        std::vector<Rank> pairKickers;
        for (const auto& [rank, count] : counts) {
            if (count == 2) {
                pairKickers.push_back(rank);
            }
        }
        for (const auto& [rank, count] : counts) {
            if (count == 1) {
                pairKickers.push_back(rank);
            }
        }
        std::sort(pairKickers.begin() + 1, pairKickers.end(), std::greater<Rank>());
        return HandEvaluation(HandRank::OnePair, pairKickers);
    }

    // 高牌
    return HandEvaluation(HandRank::HighCard, kickers);
}

} // namespace Poker
