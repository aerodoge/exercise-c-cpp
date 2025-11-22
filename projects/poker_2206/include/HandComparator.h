#pragma once

#include "Hand.h"
#include "HandEvaluator.h"

namespace Poker {

enum class ComparisonResult {
    Hand1Wins,
    Hand2Wins,
    Tie
};

class HandComparator {
public:
    // 比较两手牌，返回哪一手牌获胜
    static ComparisonResult compare(const Hand& hand1, const Hand& hand2);

    // 获取比较结果的字符串描述
    static std::string result_to_string(ComparisonResult result);

private:
    // 比较两个牌型评估结果
    static ComparisonResult compare_evaluations(
        const HandEvaluation& eval1,
        const HandEvaluation& eval2,
        const Hand& hand1,
        const Hand& hand2
    );

    // 比较kickers（点数相同时比较花色）
    static ComparisonResult compare_kickers(
        const std::vector<Rank>& kickers1,
        const std::vector<Rank>& kickers2,
        const Hand& hand1,
        const Hand& hand2
    );

    // 获取指定点数的最大花色牌
    static Card get_highest_suit_card(const Hand& hand, Rank rank);
};

} // namespace Poker
