#include "UI.hpp"
#include "game/Economy.hpp"
#include "game/Scenario.hpp"
#include <algorithm>

namespace rct {

// --- Window Implementation ---
Window::Window(const std::string& t, int x, int y, int w, int h)
    : x(x), y(y), w(w), h(h), title(t), isClosed(false), isDragging(false) {}

Window::~Window() {
    for (auto w : widgets) delete w;
}

void Window::render(Renderer& renderer) {
    if (isClosed) return;

    // Window Body (Light Gray)
    for (int dy = 0; dy < h; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            renderer.putPixel(x + dx, y + dy, 8);
        }
    }

    // Title Bar (Dark Gray)
    for (int dy = 0; dy < 15; ++dy) {
        for (int dx = 0; dx < w; ++dx) {
            renderer.putPixel(x + dx, y + dy, 7);
        }
    }

    // Render widgets
    for (auto widget : widgets) {
        widget->render(renderer, x, y);
    }
}

bool Window::handleInput(int mx, int my, bool clicked, int deltaX, int deltaY) {
    if (isClosed) return false;

    if (clicked && mx >= x && mx < x + w && my >= y && my < y + 15) {
        isDragging = true;
    }
    if (!clicked) isDragging = false;

    if (isDragging) {
        x += deltaX;
        y += deltaY;
        return true;
    }

    for (auto widget : widgets) {
        if (widget->handleInput(mx - x, my - y, clicked)) return true;
    }

    return (mx >= x && mx < x + w && my >= y && my < y + h);
}

// --- ParkInfoWindow Implementation ---
ParkInfoWindow::ParkInfoWindow(Economy* econ, int x, int y, int w, int h)
    : Window("Park Finances", x, y, w, h), economy(econ) {}

void ParkInfoWindow::render(Renderer& renderer) {
    if (isClosed) return;
    Window::render(renderer);

    int64_t balance = economy->getBalance();
    int barWidth = (balance / 10000) % (w - 20);
    if (barWidth < 0) barWidth = 0;
    if (barWidth > w - 20) barWidth = w - 20;
    
    for (int dy = 30; dy < 45; ++dy) {
        for (int dx = 10; dx < 10 + barWidth; ++dx) {
            renderer.putPixel(x + dx, y + dy, 2); // Money Green
        }
    }
}

// --- ScenarioWindow Implementation ---
ScenarioWindow::ScenarioWindow(Scenario* scen, int x, int y, int w, int h)
    : Window("Objectives", x, y, w, h), scenario(scen), guestCount(0) {}

void ScenarioWindow::render(Renderer& renderer) {
    if (isClosed) return;
    Window::render(renderer);

    int target = scenario->getGoal().targetGuestCount;
    float progress = (float)guestCount / (target > 0 ? target : 1);
    if (progress > 1.0f) progress = 1.0f;

    int barW = (w - 20) * progress;
    uint8_t color = scenario->isCompleted() ? 2 : 1; // Green if won, Red if in progress
    
    for (int dy = 30; dy < 45; ++dy) {
        for (int dx = 10; dx < 10 + barW; ++dx) {
            renderer.putPixel(x + dx, y + dy, color);
        }
    }
}

// --- UIManager Implementation ---
UIManager::~UIManager() {
    for (auto w : windows) delete w;
}

void UIManager::render(Renderer& renderer) {
    for (auto window : windows) {
        window->render(renderer);
    }
}

void UIManager::handleInput(int mx, int my, bool clicked, int deltaX, int deltaY) {
    for (auto it = windows.rbegin(); it != windows.rend(); ++it) {
        if ((*it)->handleInput(mx, my, clicked, deltaX, deltaY)) break;
    }
}

} // namespace rct
