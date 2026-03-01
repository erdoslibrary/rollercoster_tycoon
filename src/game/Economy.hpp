#ifndef RCT_ECONOMY_HPP
#define RCT_ECONOMY_HPP

#include <cstdint>

namespace rct {

/**
 * @brief Manages park finances.
 */
class Economy {
private:
    int64_t cash; // Store in cents to avoid float issues (e.g., $100.00 = 10000)
    int64_t totalProfit;
    int64_t totalLoss;

    int entryFee; // Price per guest entry

public:
    Economy(int64_t startingCash = 1000000) // Default $10,000.00
        : cash(startingCash), totalProfit(0), totalLoss(0), entryFee(1000) {}

    void addIncome(int amount) {
        cash += amount;
        totalProfit += amount;
    }

    void addExpense(int amount) {
        cash -= amount;
        totalLoss += amount;
    }

    int64_t getBalance() const { return cash; }
    int getEntryFee() const { return entryFee; }
    void setEntryFee(int fee) { entryFee = fee; }

    float getBalanceFloat() const { return cash / 100.0f; }
};

} // namespace rct

#endif // RCT_ECONOMY_HPP
