#include "Game.h"
#include "HandComparator.h"
#include <iostream>
#include <algorithm>

namespace Poker {

Game::Game()
    : human_player_(std::make_unique<HumanPlayer>("玩家")),
      ai_player_(std::make_unique<AIPlayer>("庄家")),
      human_wins_(0),
      ai_wins_(0),
      ties_(0) {}

void Game::print_separator() const {
    std::cout << "========================================\n";
}

void Game::deal_cards() {
    deck_.reset();
    deck_.shuffle();

    std::cout << "发牌...\n\n";

    human_player_->get_hand().clear();
    ai_player_->get_hand().clear();

    // 发5张牌给每个玩家
    for (int i = 0; i < Hand::HAND_SIZE; ++i) {
        if (auto card = deck_.deal_card()) {
            human_player_->get_hand().add_card(*card);
        }
        if (auto card = deck_.deal_card()) {
            ai_player_->get_hand().add_card(*card);
        }
    }
}

void Game::replace_cards_phase() {
    print_separator();
    std::cout << "换牌阶段\n";
    print_separator();

    // AI玩家换牌（不显示过程）
    auto aiCardsToReplace = ai_player_->decide_cards_to_replace();
    std::sort(aiCardsToReplace.begin(), aiCardsToReplace.end(), std::greater<size_t>());

    for (size_t index : aiCardsToReplace) {
        ai_player_->get_hand().remove_card(index);
    }

    for (size_t i = 0; i < aiCardsToReplace.size(); ++i) {
        if (auto card = deck_.deal_card()) {
            ai_player_->get_hand().add_card(*card);
        }
    }

    std::cout << "庄家换了 " << aiCardsToReplace.size() << " 张牌.\n\n";

    // 人类玩家换牌
    human_player_->show_hand();

    HandEvaluation eval = HandEvaluator::evaluate(human_player_->get_hand());
    std::cout << "你的手牌: " << eval.to_string() << "\n\n";

    auto humanCardsToReplace = human_player_->decide_cards_to_replace();

    if (humanCardsToReplace.empty()) {
        std::cout << "你选择不换牌.\n";
    } else {
        std::cout << "换 " << humanCardsToReplace.size() << " 张牌...\n";

        // 按降序排序以避免索引问题
        std::sort(humanCardsToReplace.begin(), humanCardsToReplace.end(), std::greater<size_t>());

        for (size_t index : humanCardsToReplace) {
            human_player_->get_hand().remove_card(index);
        }

        for (size_t i = 0; i < humanCardsToReplace.size(); ++i) {
            if (auto card = deck_.deal_card()) {
                human_player_->get_hand().add_card(*card);
            }
        }

        std::cout << "\n你的新手牌:\n";
        human_player_->show_hand();
    }
}

void Game::showdown() {
    print_separator();
    std::cout << "SHOWDOWN\n";
    print_separator();

    // 显示双方的牌
    std::cout << "\n";
    human_player_->show_hand();
    HandEvaluation humanEval = HandEvaluator::evaluate(human_player_->get_hand());
    std::cout << "Hand: " << humanEval.to_string() << "\n\n";

    ai_player_->show_hand();
    HandEvaluation aiEval = HandEvaluator::evaluate(ai_player_->get_hand());
    std::cout << "Hand: " << aiEval.to_string() << "\n\n";

    // 比较结果
    ComparisonResult result = HandComparator::compare(
        human_player_->get_hand(),
        ai_player_->get_hand()
    );

    print_separator();
    std::cout << "结果: ";
    switch (result) {
        case ComparisonResult::Hand1Wins:
            std::cout << human_player_->get_name() << " 胜!\n";
            human_wins_++;
            break;
        case ComparisonResult::Hand2Wins:
            std::cout << ai_player_->get_name() << " 胜!\n";
            ai_wins_++;
            break;
        case ComparisonResult::Tie:
            std::cout << "平局!\n";
            ties_++;
            break;
    }
    print_separator();
}

void Game::play_round() {
    deal_cards();

    human_player_->show_hand();

    HandEvaluation eval = HandEvaluator::evaluate(human_player_->get_hand());
    std::cout << "当前手牌: " << eval.to_string() << "\n";

    replace_cards_phase();
    showdown();
}

void Game::play_multiple_rounds(int numRounds) {
    for (int i = 0; i < numRounds; ++i) {
        std::cout << "\n\n";
        print_separator();
        std::cout << "轮 " << (i + 1) << " / " << numRounds << "\n";
        print_separator();
        std::cout << "\n";

        play_round();

        if (i < numRounds - 1) {
            std::cout << "\n按回车继续下一轮...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cout << "\n\n";
    show_statistics();
}

void Game::show_statistics() const {
    print_separator();
    std::cout << "游戏统计\n";
    print_separator();
    std::cout << human_player_->get_name() << " 获胜: " << human_wins_ << "\n";
    std::cout << ai_player_->get_name() << " 获胜: " << ai_wins_ << "\n";
    std::cout << "平局: " << ties_ << "\n";
    print_separator();

    int totalGames = human_wins_ + ai_wins_ + ties_;
    if (totalGames > 0) {
        std::cout << "胜率: "
                  << (human_wins_ * 100.0 / totalGames) << "%\n";
        print_separator();
    }
}

void Game::start() {
    std::cout << "========================================\n";
    std::cout << "         欢迎来到扑克牌游戏!\n";
    std::cout << "========================================\n\n";

    std::cout << "你想玩几轮? ";
    int numRounds;
    std::cin >> numRounds;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (numRounds <= 0) {
        numRounds = 1;
    }

    play_multiple_rounds(numRounds);
}

} // namespace Poker
