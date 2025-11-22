#pragma once

#include "Hand.h"
#include <map>
#include <vector>

namespace Poker {

// 牌型枚举
enum class HandRank {
    HighCard = 0,      // 高牌
    OnePair = 1,       // 一对
    TwoPair = 2,       // 两对
    ThreeOfKind = 3,   // 三条
    Straight = 4,      // 顺子
    Flush = 5,         // 同花
    FullHouse = 6,     // 葫芦（三带二）
    FourOfKind = 7,    // 四条
    StraightFlush = 8  // 同花顺
};

// 牌型评估结果
struct HandEvaluation {
    HandRank rank;
    std::vector<Rank> kickers;  // 用于平局时的比较

    HandEvaluation(HandRank r, const std::vector<Rank>& k = {})
        : rank(r), kickers(k) {}

    std::string to_string() const;
};

class HandEvaluator {
public:
    // 评估一手牌
    static HandEvaluation evaluate(const Hand& hand);

    // 判断是否有对子
    static bool hasPair(const Hand& hand);

    // 判断是否有两对
    static bool hasTwoPair(const Hand& hand);

    // 判断是否有三条
    static bool hasThreeOfKind(const Hand& hand);

    // 判断是否有四条
    static bool hasFourOfKind(const Hand& hand);

    // 判断是否是同花
    static bool isFlush(const Hand& hand);

    // 判断是否是顺子
    static bool isStraight(const Hand& hand);

    // 判断是否是同花顺
    static bool isStraightFlush(const Hand& hand);

private:
    // 统计每个点数的数量
    static std::map<Rank, int> countRanks(const Hand& hand);

    // 检查是否所有牌都是同一花色
    static bool allSameSuit(const Hand& hand);

    // 检查是否是连续的点数
    static bool isSequential(const std::vector<Rank>& ranks);
};

} // namespace Poker
