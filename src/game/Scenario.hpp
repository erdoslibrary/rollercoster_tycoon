#ifndef RCT_SCENARIO_HPP
#define RCT_SCENARIO_HPP

#include <string>
#include <cstdint>

namespace rct {

struct ScenarioGoal {
    int targetGuestCount;
    int64_t targetCash;
    int timeLimitInTicks; // Simplified time limit

    ScenarioGoal(int guests = 50, int64_t cash = 200000, int time = 10000)
        : targetGuestCount(guests), targetCash(cash), timeLimitInTicks(time) {}
};

class Scenario {
private:
    std::string name;
    ScenarioGoal goal;
    bool completed;

public:
    Scenario(const std::string& n, ScenarioGoal g) 
        : name(n), goal(g), completed(false) {}

    bool checkVictory(int currentGuests, int64_t currentCash) {
        if (completed) return true;
        
        if (currentGuests >= goal.targetGuestCount && currentCash >= goal.targetCash) {
            completed = true;
            return true;
        }
        return false;
    }

    const std::string& getName() const { return name; }
    const ScenarioGoal& getGoal() const { return goal; }
    bool isCompleted() const { return completed; }
};

} // namespace rct

#endif // RCT_SCENARIO_HPP
