#include "Player.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <sstream>

namespace Poker {

Player::Player(const std::string& name) : name_(name) {}

void Player::show_hand(bool hideCards) const {
    std::cout << name_ << "的手牌:\n";
    if (hideCards) {
        for (size_t i = 0; i < hand_.size(); ++i) {
            std::cout << "[Hidden Card " << (i + 1) << "]\n";
        }
    } else {
        const auto& cards = hand_.get_cards();
        for (size_t i = 0; i < cards.size(); ++i) {
            std::cout << "[" << (i + 1) << "] " << cards[i].to_string() << "\n";
        }
    }
}

HumanPlayer::HumanPlayer(const std::string& name) : Player(name) {}

std::vector<size_t> HumanPlayer::decide_cards_to_replace() {
    std::vector<size_t> cardsToReplace;
    std::cout << "\n输入你想换的牌 (1-5), 空格隔开.\n";
    std::cout << "如果你觉得这手牌不错，直接按回车跳过换牌: ";

    std::string line;
    std::getline(std::cin, line);

    if (line.empty()) {
        return cardsToReplace;
    }

    std::istringstream iss(line);
    int cardNum;
    while (iss >> cardNum) {
        if (cardNum >= 1 && cardNum <= static_cast<int>(hand_.size())) {
            cardsToReplace.push_back(cardNum - 1);
        }
    }

    // 移除重复的索引并排序
    std::sort(cardsToReplace.begin(), cardsToReplace.end());
    cardsToReplace.erase(
        std::unique(cardsToReplace.begin(), cardsToReplace.end()),
        cardsToReplace.end()
    );

    return cardsToReplace;
}

AIPlayer::AIPlayer(const std::string& name) : Player(name) {}

bool AIPlayer::is_almost_flush() const {
    std::map<Suit, int> suitCounts;
    for (const auto& card : hand_.get_cards()) {
        suitCounts[card.get_suit()]++;
    }

    for (const auto& [suit, count] : suitCounts) {
        if (count >= 4) return true;
    }
    return false;
}

bool AIPlayer::is_almost_straight() const {
    std::vector<int> ranks;
    for (const auto& card : hand_.get_cards()) {
        ranks.push_back(static_cast<int>(card.get_rank()));
    }
    std::sort(ranks.begin(), ranks.end());

    // 检查是否有4张连续的牌
    int consecutive = 1;
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] == ranks[i-1] + 1) {
            consecutive++;
            if (consecutive >= 4) return true;
        } else if (ranks[i] != ranks[i-1]) {
            consecutive = 1;
        }
    }

    return false;
}

std::vector<size_t> AIPlayer::analyze_hand() {
    HandEvaluation eval = HandEvaluator::evaluate(hand_);
    std::vector<size_t> cardsToReplace;

    // 如果已经有好牌，不换牌
    if (eval.rank >= HandRank::ThreeOfKind) {
        return cardsToReplace;
    }

    // 统计每个点数的数量
    std::map<Rank, std::vector<size_t>> rankPositions;
    for (size_t i = 0; i < hand_.get_cards().size(); ++i) {
        rankPositions[hand_.get_cards()[i].get_rank()].push_back(i);
    }

    // 两对：不换牌
    if (eval.rank == HandRank::TwoPair) {
        return cardsToReplace;
    }

    // 一对：换掉非对子的牌
    if (eval.rank == HandRank::OnePair) {
        for (const auto& [rank, positions] : rankPositions) {
            if (positions.size() == 1) {
                cardsToReplace.push_back(positions[0]);
            }
        }
        return cardsToReplace;
    }

    // 接近同花：只换一张不同花色的牌
    if (is_almost_flush()) {
        std::map<Suit, int> suitCounts;
        Suit majorSuit;
        int maxCount = 0;

        for (const auto& card : hand_.get_cards()) {
            suitCounts[card.get_suit()]++;
            if (suitCounts[card.get_suit()] > maxCount) {
                maxCount = suitCounts[card.get_suit()];
                majorSuit = card.get_suit();
            }
        }

        for (size_t i = 0; i < hand_.get_cards().size(); ++i) {
            if (hand_.get_cards()[i].get_suit() != majorSuit) {
                cardsToReplace.push_back(i);
                return cardsToReplace;
            }
        }
    }

    // 接近顺子：换掉不连续的牌
    if (is_almost_straight()) {
        std::vector<std::pair<int, size_t>> ranksWithPos;
        for (size_t i = 0; i < hand_.get_cards().size(); ++i) {
            ranksWithPos.push_back({static_cast<int>(hand_.get_cards()[i].get_rank()), i});
        }
        std::sort(ranksWithPos.begin(), ranksWithPos.end());

        // 找到最长连续序列，换掉其他的牌
        int bestStart = 0, bestLen = 1;
        int currentStart = 0, currentLen = 1;

        for (size_t i = 1; i < ranksWithPos.size(); ++i) {
            if (ranksWithPos[i].first == ranksWithPos[i-1].first + 1) {
                currentLen++;
            } else {
                if (currentLen > bestLen) {
                    bestLen = currentLen;
                    bestStart = currentStart;
                }
                currentStart = i;
                currentLen = 1;
            }
        }

        if (currentLen > bestLen) {
            bestLen = currentLen;
            bestStart = currentStart;
        }

        // 换掉不在最长连续序列中的牌
        for (size_t i = 0; i < ranksWithPos.size(); ++i) {
            if (i < static_cast<size_t>(bestStart) || i >= static_cast<size_t>(bestStart + bestLen)) {
                cardsToReplace.push_back(ranksWithPos[i].second);
            }
        }

        if (cardsToReplace.size() <= 2) {
            return cardsToReplace;
        }
    }

    // 高牌：换掉最小的3张牌
    std::vector<std::pair<Rank, size_t>> ranksWithPos;
    for (size_t i = 0; i < hand_.get_cards().size(); ++i) {
        ranksWithPos.push_back({hand_.get_cards()[i].get_rank(), i});
    }
    std::sort(ranksWithPos.begin(), ranksWithPos.end());

    for (size_t i = 0; i < 3 && i < ranksWithPos.size(); ++i) {
        cardsToReplace.push_back(ranksWithPos[i].second);
    }

    return cardsToReplace;
}

std::vector<size_t> AIPlayer::decide_cards_to_replace() {
    return analyze_hand();
}

} // namespace Poker
