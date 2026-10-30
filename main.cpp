/*
 * File:     main.cpp
 * Purpose:  Entry point for The Dictionary.
 *           Seeds the random number generator, then hands control to an
 *           AdvancedDictionary, which runs the menu until the user exits.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>    // only needed for SetConsoleOutputCP below
#endif

#include "AdvancedDictionary.h"

/*
 * Function: main
 * Purpose:  Program entry point.
 * Input:    none (command-line arguments are not used)
 * Output:   0 on a normal exit
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
int main()
{
#ifdef _WIN32
    // The Windows console doesn't use UTF-8 by default, which would turn the
    // en dash in "Word Found - with N definitions" into gibberish.
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Seed rand() once, using the clock, so the guessing game differs each run.
    // (Seeding repeatedly would give repeats, so it's done here and only here.)
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "Welcome to Dilaksan's Dictionary!\n";

    AdvancedDictionary dictionary;
    dictionary.run();

    return 0;
}
