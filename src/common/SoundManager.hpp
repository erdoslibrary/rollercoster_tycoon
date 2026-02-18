#ifndef RCT_SOUND_MANAGER_HPP
#define RCT_SOUND_MANAGER_HPP

#include <string>
#include <iostream>

namespace rct {

/**
 * @brief Mock SoundManager for environments without SDL_mixer.
 */
class SoundManager {
public:
    SoundManager() {
        // SDL_mixer initialization skipped
    }

    ~SoundManager() {
        // Cleanup skipped
    }

    bool loadSound(const std::string& name, const std::string& path) {
        return true; // Pretend it's loaded
    }

    void playSound(const std::string& name) {
        // Sound playback disabled
    }
};

} // namespace rct

#endif // RCT_SOUND_MANAGER_HPP
