/*
 * File:     AlgorithmHelper.h
 * Purpose:  A small collection of static helper functions. The important one
 *           is getRandomIndex (needed for the guessing game); I also parked
 *           two tiny string helpers here (toLower, trim) because both
 *           Dictionary and AdvancedDictionary need them.
 *           Everything is header-only, as the brief asks for this class to
 *           live in its own .h file.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef ALGORITHMHELPER_H
#define ALGORITHMHELPER_H

#include <cstdlib>   // rand()
#include <cctype>    // tolower, isspace
#include <string>

class AlgorithmHelper
{
public:
    /*
     * Function: AlgorithmHelper::getRandomIndex
     * Purpose:  Returns a random integer from 0 up to AND INCLUDING maxIndex.
     *
     *           The catch: rand() only goes up to RAND_MAX, which is just
     *           32767 on Windows/Visual Studio. The dictionary has over
     *           105,000 words, so plain "rand() % size" could never pick
     *           anything past index 32767 - most of the dictionary would be
     *           unreachable!
     *
     *           My way around it (still only using rand()):
     *             1. Each rand() call gives us 15 good random bits, so I glue
     *                four calls together to build a 60-bit random number.
     *                That is far bigger than any dictionary we'd ever load.
     *             2. Then I squeeze it into 0..maxIndex with modulo. Modulo
     *                on its own slightly favours the low numbers (because
     *                2^60 isn't an exact multiple of the range), so I throw
     *                away any number in the uneven "leftover" tail and
     *                roll again. This is called rejection sampling and it
     *                keeps every index equally likely.
     *
     * Input:    maxIndex - the largest index that may be returned
     *                      (e.g. size - 1 for a vector).
     * Output:   an int in [0, maxIndex]. If maxIndex is 0 or negative there's
     *           only one sensible answer, so we return 0.
     * Author:   Dilaksan Gnanasegaran
     * Created:  30/10/2026
     */
    static int getRandomIndex(int maxIndex)
    {
        if (maxIndex <= 0)
        {
            return 0;
        }

        const unsigned long long range = static_cast<unsigned long long>(maxIndex) + 1ULL;
        const unsigned long long span = 1ULL << 60;                 // size of our big random number
        const unsigned long long limit = span - (span % range);     // biggest multiple of range that fits

        unsigned long long value;
        do
        {
            // & 0x7FFF keeps just the low 15 bits, which every rand() has
            // (RAND_MAX is at least 32767 everywhere).
            value  = static_cast<unsigned long long>(std::rand() & 0x7FFF);
            value  = (value << 15) | static_cast<unsigned long long>(std::rand() & 0x7FFF);
            value  = (value << 15) | static_cast<unsigned long long>(std::rand() & 0x7FFF);
            value  = (value << 15) | static_cast<unsigned long long>(std::rand() & 0x7FFF);
        } while (value >= limit);   // in the uneven tail? roll again

        return static_cast<int>(value % range);
    }

    /*
     * Function: AlgorithmHelper::toLower
     * Purpose:  Returns a lower-case copy of a string, so searches and guesses
     *           can be case-insensitive.
     * Input:    text - any string
     * Output:   the same text in lower case
     * Author:   Dilaksan Gnanasegaran
     * Created:  30/10/2026
     */
    static std::string toLower(std::string text)
    {
        for (size_t i = 0; i < text.size(); i++)
        {
            // cast to unsigned char first - tolower() on a negative char is undefined behaviour
            text[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(text[i])));
        }
        return text;
    }

    /*
     * Function: AlgorithmHelper::trim
     * Purpose:  Strips leading and trailing whitespace (spaces, tabs, and
     *           stray '\r' left over from Windows line endings).
     * Input:    text - any string
     * Output:   the trimmed string (empty if it was all whitespace)
     * Author:   Dilaksan Gnanasegaran
     * Created:  30/10/2026
     */
    static std::string trim(const std::string& text)
    {
        size_t first = 0;
        while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first])))
        {
            first++;
        }
        size_t last = text.size();
        while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])))
        {
            last--;
        }
        return text.substr(first, last - first);
    }
};

#endif
