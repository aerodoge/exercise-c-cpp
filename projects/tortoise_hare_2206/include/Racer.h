/**
 * @file Racer.h
 * @brief 参赛者抽象基类声明
 */

#ifndef RACER_H
#define RACER_H

#include "Constants.h"
#include <string>
#include <string_view>

/**
 * @class Racer
 * @brief 参赛者抽象基类，定义所有参赛者的公共接口和行为
 * @details 使用多态实现不同参赛者的特定移动规则
 */
class Racer {
public:
    /**
     * @brief 构造函数
     * @param racerName 参赛者名称（使用string_view提高性能）
     * @param racerSymbol 赛道上显示的符号字符
     */
    Racer(std::string_view racerName, char racerSymbol);

    /// 虚析构函数，确保正确释放派生类对象
    virtual ~Racer() = default;

    /// 禁用拷贝构造和拷贝赋值（遵循Rule of Five）
    Racer(const Racer&) = delete;
    Racer& operator=(const Racer&) = delete;

    /// 允许移动构造和移动赋值
    Racer(Racer&&) = default;
    Racer& operator=(Racer&&) = default;

    /**
     * @brief 纯虚函数：移动方法
     * @details 由派生类实现具体的移动规则，每次调用表示一个时钟滴答
     */
    virtual void move() = 0;

    /**
     * @brief 获取当前位置
     * @return 当前所在的格子编号
     */
    [[nodiscard]] int getPosition() const noexcept;

    /**
     * @brief 获取参赛者名称
     * @return 名称的字符串视图
     */
    [[nodiscard]] std::string_view getName() const noexcept;

    /**
     * @brief 获取赛道显示符号
     * @return 单个字符符号
     */
    [[nodiscard]] char getSymbol() const noexcept;

    /**
     * @brief 判断是否到达终点
     * @return true表示到达或超过终点线
     */
    [[nodiscard]] bool hasFinished() const noexcept;

    /**
     * @brief 重置位置到起点
     */
    void reset() noexcept;

protected:
    /**
     * @brief 确保位置不会滑倒到起点之前
     * @details 如果位置小于起点，将其设置为起点位置
     */
    void ensureValidPosition() noexcept;

    int position{START_LINE};       ///< 当前位置，默认为起点
    std::string name;               ///< 参赛者名称
    char symbol;                    ///< 在赛道上的显示符号
};

#endif // RACER_H
