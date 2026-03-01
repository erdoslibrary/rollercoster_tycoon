#ifndef RCT_UI_HPP
#define RCT_UI_HPP

#include <vector>
#include <string>
#include <functional>
#include "Renderer.hpp"

namespace rct {

/**
 * @brief Base class for UI elements like buttons and labels.
 */
class Widget {
public:
    int x, y, w, h;
    virtual void render(Renderer& renderer, int offsetX, int offsetY) = 0;
    virtual bool handleInput(int mx, int my, bool clicked) { return false; }
    virtual ~Widget() = default;
};

/**
 * @brief A draggable in-game window.
 */
class Window {
public:
    int x, y, w, h;
    std::string title;
    bool isClosed;
    bool isDragging;

    std::vector<Widget*> widgets;

    Window(const std::string& t, int x, int y, int w, int h);
    virtual ~Window();

    void addWidget(Widget* widget) { widgets.push_back(widget); }
    virtual void render(Renderer& renderer);
    bool handleInput(int mx, int my, bool clicked, int deltaX, int deltaY);
};

/**
 * @brief Window specifically for showing park finances.
 */
class ParkInfoWindow : public Window {
private:
    class Economy* economy;
public:
    ParkInfoWindow(class Economy* econ, int x, int y, int w, int h);
    void render(Renderer& renderer) override;
};

/**
 * @brief Window for showing scenario goals and progress.
 */
class ScenarioWindow : public Window {
private:
    class Scenario* scenario;
    int guestCount;
public:
    ScenarioWindow(class Scenario* scen, int x, int y, int w, int h);
    void updateProgress(int guests) { guestCount = guests; }
    void render(Renderer& renderer) override;
};

/**
 * @brief Manages multiple windows and UI layering.
 */
class UIManager {
private:
    std::vector<Window*> windows;

public:
    ~UIManager();
    void addWindow(Window* window) { windows.push_back(window); }
    void render(Renderer& renderer);
    void handleInput(int mx, int my, bool clicked, int deltaX, int deltaY);
};

} // namespace rct

#endif // RCT_UI_HPP
