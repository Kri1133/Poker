#include <vector>
#include <string>

// Helper function: returns all 5-card combinations from cards
std::vector<std::vector<std::vector<std::string>>> getAllFiveCardCombinations(
    const std::vector<std::vector<std::string>>& cards)
{
    std::vector<std::vector<std::vector<std::string>>> result;
    int n = cards.size();

    for (int i = 0; i < n - 4; ++i) {
        for (int j = i + 1; j < n - 3; ++j) {
            for (int k = j + 1; k < n - 2; ++k) {
                for (int l = k + 1; l < n - 1; ++l) {
                    for (int m = l + 1; m < n; ++m) {
                        std::vector<std::vector<std::string>> combo;
                        combo.push_back(cards[i]);
                        combo.push_back(cards[j]);
                        combo.push_back(cards[k]);
                        combo.push_back(cards[l]);
                        combo.push_back(cards[m]);
                        result.push_back(combo);
                    }
                }
            }
        }
    }
    return result;
}

