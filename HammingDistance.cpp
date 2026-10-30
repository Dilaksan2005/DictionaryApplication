/*
 * File:     HammingDistance.cpp
 * Purpose:  Implementation of the Hamming distance.
 *
 * References (I wrote the code myself from these descriptions):
 *   Hamming, R. W. (1950). Error detecting and error correcting codes.
 *       Bell System Technical Journal, 29(2), 147-160.
 *   Wikipedia contributors. (n.d.). Hamming distance. Wikipedia.
 *       https://en.wikipedia.org/wiki/Hamming_distance
 *
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "HammingDistance.h"
#include <algorithm>

/*
 * Function: HammingDistance::distance
 * Purpose:  Counts the positions where the two words have different letters.
 *           "cart" vs "card" -> 1 (only the last letter differs).
 *
 *           Textbook Hamming distance is only defined for strings of EQUAL
 *           length, but a dictionary is full of words of all lengths, so I
 *           need a rule for the rest. My choice: compare the letters the two
 *           words have in common positions, then count every leftover letter
 *           on the longer word as one extra mismatch. In other words
 *           distance = mismatches in the overlap + difference in length.
 *           It means that a 3-letter search word will naturally prefer
 *           3-letter matches, which feels right for Hamming.
 * Input:    a, b - the two words to compare
 * Output:   the distance (whole number stored in a double); 0 = identical
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
double HammingDistance::distance(const std::string& a, const std::string& b) const
{
    size_t shorter = std::min(a.size(), b.size());
    size_t longer = std::max(a.size(), b.size());

    // Count the mismatching letters in the part where both words have letters
    size_t mismatches = 0;
    for (size_t i = 0; i < shorter; i++)
    {
        if (a[i] != b[i])
        {
            mismatches++;
        }
    }

    // Letters hanging off the end of the longer word each count as a mismatch
    mismatches += (longer - shorter);

    return static_cast<double>(mismatches);
}

/*
 * Function: HammingDistance::getName
 * Purpose:  Heading text used when printing this measure's results.
 * Input:    none
 * Output:   "Hamming Distance"
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
std::string HammingDistance::getName() const
{
    return "Hamming Distance";
}
