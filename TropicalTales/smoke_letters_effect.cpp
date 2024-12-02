#include "smoke_letters_effect.h"


SmokeLettersEffect::SmokeLettersEffect(float x, float y): x(x), y(y) { }

SmokeLettersEffect::~SmokeLettersEffect() {

}

void SmokeLettersEffect::render(TextRenderer& tr) {

	for (SmokeLetter& letter : activeSmokeLetters) {
        tr.RenderText(
            std::string(1, letter.character),
            letter.x,
            letter.y,
            1.0f,
            glm::vec3(0.5f, 0.5f, 0.5f) * letter.opacity
        );
    }

}

void SmokeLettersEffect::updateSpawner(float deltaTime) {

    if (!spawningActive || currentLetterIndex >= smokeMessage.size()) {
        return;
    }

    letterSpawnTimer += deltaTime;

    if (letterSpawnTimer >= letterSpawnInterval) {

        activeSmokeLetters.emplace_back(
            smokeMessage[currentLetterIndex],
            x,
            y,
            150.0f,
            2.0f 
        );

        currentLetterIndex++;
        letterSpawnTimer = 0.0f;

        if (currentLetterIndex >= smokeMessage.size()) {
            spawningActive = false;
        }
    }

}

void SmokeLettersEffect::updateLetters(float deltaTime) {

    for (std::vector<SmokeLetter>::iterator it = activeSmokeLetters.begin(); it != activeSmokeLetters.end();) {
        it->y += it->velocity * deltaTime;
        
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0.0f) {
            it = activeSmokeLetters.erase(it);

        } else {
            ++it;
        }
    }

}

void SmokeLettersEffect::clicked() {

    if (spawningActive) {
        return;
    }

    spawningActive = true;
    currentLetterIndex = 0;

}
