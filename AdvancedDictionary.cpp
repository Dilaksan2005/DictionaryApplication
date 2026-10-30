/*
 * File:     AdvancedDictionary.cpp
 * Purpose:  Implementation of AdvancedDictionary (Tasks 4 and 5).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "AdvancedDictionary.h"
#include "AlgorithmHelper.h"
#include "HammingDistance.h"
#include "LevenshteinDistance.h"
#include "JaroWinklerDistance.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

/*
 * Function: AdvancedDictionary::AdvancedDictionary
 * Purpose:  Constructor. Creates one object for each similarity measure and
 *           stores the pointers, ready for Smart Search to loop over. To add
 *           a fourth measure later you'd just add one line here.
 * Input:    none
 * Output:   none (constructs the object)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
AdvancedDictionary::AdvancedDictionary()
{
    measures.push_back(new HammingDistance());
    measures.push_back(new LevenshteinDistance());
    measures.push_back(new JaroWinklerDistance());
}

/*
 * Function: AdvancedDictionary::~AdvancedDictionary
 * Purpose:  Destructor. We used 'new' in the constructor, so we must 'delete'
 *           every measure here or the memory would leak.
 * Input:    none
 * Output:   none
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
AdvancedDictionary::~AdvancedDictionary()
{
    for (size_t i = 0; i < measures.size(); i++)
    {
        delete measures[i];
    }
    measures.clear();
}

/*
 * Function: AdvancedDictionary::displayMenu
 * Purpose:  Prints the updated menu: the three original tasks plus the two
 *           new ones.
 * Input:    none
 * Output:   none (prints to the screen)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void AdvancedDictionary::displayMenu() const
{
    std::cout << "\n====== ADVANCED DICTIONARY MENU ======\n"
              << "  1. Load new Dictionary\n"
              << "  2. Search Word\n"
              << "  3. About Dictionary\n"
              << "  4. Guessing Game\n"
              << "  5. Smart Search\n"
              << "  " << getExitOption() << ". Exit\n"
              << "======================================\n";
}

/*
 * Function: AdvancedDictionary::getExitOption
 * Purpose:  With five tasks on the menu, Exit becomes option 6.
 * Input:    none
 * Output:   6
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
int AdvancedDictionary::getExitOption() const
{
    return 6;
}

/*
 * Function: AdvancedDictionary::processChoice
 * Purpose:  Handles the two new menu numbers (4 and 5) here, and hands
 *           everything else (1-3, or invalid numbers) to the base class
 *           so I don't duplicate its code.
 * Input:    choice - the number the user entered
 * Output:   none
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void AdvancedDictionary::processChoice(int choice)
{
    switch (choice)
    {
    case 4: guessingGame(); break;
    case 5: smartSearch();  break;
    default: Dictionary::processChoice(choice); break;
    }
}

/*
 * Function: AdvancedDictionary::guessingGame
 * Purpose:  Task 4 - the guessing game.
 *
 *           Rules as implemented:
 *             - Words in play have 3+ letters and no hyphens. I build a list of
 *               the positions of those words ONCE at the start, then
 *               AlgorithmHelper::getRandomIndex picks from that list.
 *             - Start with 5 lives and 0 points.
 *             - Correct guess: +3 points.
 *             - Wrong guess: the answer is revealed, -1 life, new word.
 *             - 1 extra life for every 5 points earned. Because points go up in
 *               threes (3, 6, 9, 12, 15...) the score jumps OVER multiples of
 *               5 rather than landing on them, so I compare (score / 5) before
 *               and after each win: 6 points crosses 5 (+1 life), 12 crosses
 *               10 (+1 life), 15 crosses 15 (+1 life), and so on.
 *             - Game over at 0 lives. (I added "/quit" so a player can bail out
 *               early; it can't clash with a real answer because words in this
 *               game never contain a slash.)
 * Input:    none (prompts for guesses)
 * Output:   none (prints the game and the final score)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void AdvancedDictionary::guessingGame()
{
    if (!requireLoaded())
    {
        return;
    }

    // Collect the indices of every word that is allowed in the game
    std::vector<size_t> eligible;
    for (size_t i = 0; i < words.size(); i++)
    {
        const std::string& name = words[i].getName();
        if (name.size() >= 3 && name.find('-') == std::string::npos)
        {
            eligible.push_back(i);
        }
    }
    if (eligible.empty())
    {
        std::cout << "\nThere are no suitable words (3+ letters, no hyphens) to play with.\n";
        return;
    }

    int lives = 5;
    int score = 0;

    std::cout << "\n========== GUESSING GAME ==========\n"
              << "I'll show you a definition - you guess the word!\n"
              << "You start with 5 lives. Right answer = 3 points, and you earn\n"
              << "an extra life for every 5 points. Type /quit to stop early.\n";

    while (lives > 0)
    {
        // Pick a random eligible word
        int pick = AlgorithmHelper::getRandomIndex(static_cast<int>(eligible.size()) - 1);
        const Word& target = words[eligible[pick]];

        std::cout << "\n-----------------------------------\n";
        std::cout << "Lives: " << lives << "    Score: " << score << "\n\n";

        std::vector<std::string> definitions = target.getDefinitions();
        std::cout << (definitions.size() == 1 ? "Definition:\n" : "Definitions:\n");
        for (size_t d = 0; d < definitions.size(); d++)
        {
            if (definitions.size() > 1)
            {
                std::cout << "  " << (d + 1) << ". " << definitions[d] << "\n";
            }
            else
            {
                std::cout << "  " << definitions[d] << "\n";
            }
        }
        std::cout << "\nType:           (" << target.getFullType() << ")\n"
                  << "First letter:   " << target.getName()[0] << "\n"
                  << "Length:         " << target.getName().size() << " letters\n";

        std::string guess;
        if (!readLine("\nYour guess: ", guess))
        {
            break;      // input ended
        }
        guess = AlgorithmHelper::toLower(guess);

        if (guess == "/quit")
        {
            std::cout << "\nOkay, stopping early.\n";
            break;
        }

        if (guess == target.getName())
        {
            int oldScore = score;
            score += 3;
            std::cout << "\nCorrect! +3 points.\n";

            // How many multiples of 5 did we just step over? Each one = a bonus life.
            int bonusLives = (score / 5) - (oldScore / 5);
            if (bonusLives > 0)
            {
                lives += bonusLives;
                std::cout << "Bonus! You earned " << bonusLives
                          << (bonusLives == 1 ? " extra life.\n" : " extra lives.\n");
            }
        }
        else
        {
            lives--;
            std::cout << "\nNot quite - the word was \"" << target.getName() << "\".\n";
        }
    }

    std::cout << "\n========== GAME OVER ==========\n"
              << "Final score: " << score << "\n";
}

/*
 * Function: AdvancedDictionary::smartSearch
 * Purpose:  Task 5 - Smart Search. Asks for a word, then for EACH similarity
 *           measure in the 'measures' vector scores every dictionary word
 *           and prints the five closest.
 *
 *           Because we go through base-class pointers, this function has no
 *           idea (or need to know) which measure is which - that's the
 *           polymorphism the brief is after.
 *
 *           How I pick the top five: build a list of (distance, index) pairs
 *           and use std::partial_sort, which only bothers to fully sort the
 *           first five - quicker than sorting 105,000 entries. Pairs compare
 *           by distance first and then by index, so ties are broken by
 *           dictionary order and results are the same every run.
 *
 *           If the search word is itself in the dictionary it shows up first
 *           with distance 0 - I left that in on purpose, as it confirms the
 *           word exists and the other four are its nearest neighbours.
 * Input:    none (prompts for the word)
 * Output:   none (prints five results per measure)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
void AdvancedDictionary::smartSearch()
{
    if (!requireLoaded())
    {
        return;
    }

    std::string input;
    if (!readLine("\nEnter a word for Smart Search: ", input))
    {
        return;
    }
    std::string target = AlgorithmHelper::toLower(input);
    if (target.empty())
    {
        std::cout << "\nYou didn't type a word.\n";
        return;
    }

    const size_t TOP_N = 5;
    const size_t SNIPPET_LENGTH = 60;   // keep the preview of the definition short

    for (size_t m = 0; m < measures.size(); m++)
    {
        // Score every word with this measure
        std::vector<std::pair<double, size_t> > scored;
        scored.reserve(words.size());
        for (size_t i = 0; i < words.size(); i++)
        {
            scored.push_back(std::make_pair(measures[m]->distance(target, words[i].getName()), i));
        }

        // Only the best five need to be in order
        size_t top = std::min(TOP_N, scored.size());
        std::partial_sort(scored.begin(), scored.begin() + top, scored.end());

        std::cout << "\n=== " << measures[m]->getName() << " - top " << top << " closest to \"" << target << "\" ===\n";
        for (size_t r = 0; r < top; r++)
        {
            const Word& w = words[scored[r].second];

            // Show just the first definition, shortened, as a reminder of what the word means
            std::string snippet = w.getDefinitions()[0];
            if (snippet.size() > SNIPPET_LENGTH)
            {
                snippet = snippet.substr(0, SNIPPET_LENGTH) + "...";
            }

            std::cout << "  " << (r + 1) << ". " << std::left << std::setw(20) << w.getName()
                      << std::right << "distance " << std::setprecision(4) << scored[r].first
                      << "  (" << w.getFullType() << ")  " << snippet << "\n";
        }
    }
}
