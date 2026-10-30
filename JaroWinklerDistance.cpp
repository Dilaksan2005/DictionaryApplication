/*
 * File:     JaroWinklerDistance.cpp
 * Purpose:  Implementation of the Jaro-Winkler distance.
 *
 * Why I picked it: Hamming only looks at letters in the same position, and
 * Levenshtein treats an edit at the start of a word the same as one at the
 * end. Jaro-Winkler is a genuinely different idea - it counts letters that
 * match "roughly in the same area" (so shifted letters still count), penalises
 * swapped letters (transpositions), and gives a bonus to words that share the
 * same first few letters. People rarely get the start of a word wrong when
 * misspelling it, so that bonus suits a dictionary look-up well. It was
 * originally designed for matching names, which are short strings just like
 * our words.
 *
 * References (I wrote the code myself from these descriptions):
 *   Jaro, M. A. (1989). Advances in record-linkage methodology as applied to
 *       matching the 1985 census of Tampa, Florida. Journal of the American
 *       Statistical Association, 84(406), 414-420.
 *   Winkler, W. E. (1990). String comparator metrics and enhanced decision
 *       rules in the Fellegi-Sunter model of record linkage. Proceedings of
 *       the Section on Survey Research Methods, American Statistical
 *       Association, 354-359.
 *   Wikipedia contributors. (n.d.). Jaro-Winkler distance. Wikipedia.
 *       https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance
 *
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "JaroWinklerDistance.h"
#include <vector>
#include <algorithm>

/*
 * Function: JaroWinklerDistance::jaroSimilarity (private helper)
 * Purpose:  Calculates the plain Jaro similarity (1.0 = identical, 0.0 = nothing
 *           in common), which Winkler's tweak then builds on.
 *
 *           Steps:
 *             1. Two letters "match" if they're equal AND not further apart
 *                than half the longer word's length (minus 1).
 *             2. Count the matches (m). No matches at all = similarity 0.
 *             3. Walk through the matched letters of both words in order; each
 *                pair that disagrees is half a transposition.
 *             4. similarity = (m/len(a) + m/len(b) + (m - transpositions)/m) / 3
 * Input:    a, b - the two words to compare
 * Output:   similarity between 0.0 and 1.0
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
double JaroWinklerDistance::jaroSimilarity(const std::string& a, const std::string& b) const
{
    if (a == b) return 1.0;                       // identical (this also covers two empty words)
    if (a.empty() || b.empty()) return 0.0;       // one empty, one not: nothing can match

    // How far apart two equal letters may be and still count as a match
    int window = static_cast<int>(std::max(a.size(), b.size()) / 2);
    window = (window > 0) ? window - 1 : 0;

    std::vector<bool> aMatched(a.size(), false);
    std::vector<bool> bMatched(b.size(), false);
    int matches = 0;

    // Step 1 & 2: for each letter of a, look for an unused equal letter in b within the window
    for (size_t i = 0; i < a.size(); i++)
    {
        int lo = std::max(0, static_cast<int>(i) - window);
        int hi = std::min(static_cast<int>(b.size()) - 1, static_cast<int>(i) + window);

        for (int j = lo; j <= hi; j++)
        {
            if (!bMatched[j] && a[i] == b[j])
            {
                aMatched[i] = true;
                bMatched[j] = true;
                matches++;
                break;      // each letter can only be matched once
            }
        }
    }

    if (matches == 0) return 0.0;

    // Step 3: count matched letters that appear in a different ORDER in each word
    int outOfOrder = 0;
    size_t k = 0;   // pointer into b's matched letters
    for (size_t i = 0; i < a.size(); i++)
    {
        if (!aMatched[i]) continue;
        while (!bMatched[k]) k++;       // skip b's unmatched letters
        if (a[i] != b[k]) outOfOrder++;
        k++;
    }
    double transpositions = outOfOrder / 2.0;   // every swap shows up as two disagreements

    // Step 4: the Jaro formula
    double m = static_cast<double>(matches);
    return (m / a.size() + m / b.size() + (m - transpositions) / m) / 3.0;
}

/*
 * Function: JaroWinklerDistance::distance
 * Purpose:  Jaro-Winkler distance = 1 - Jaro-Winkler similarity, so that it
 *           behaves like the other measures (0 = identical, bigger = further
 *           apart).
 *
 *           Winkler's improvement: if the Jaro similarity is already decent
 *           (> 0.7) and the words share a common prefix, nudge the similarity
 *           up by (prefix length, capped at 4) x 0.1 x (1 - jaro). The 4 and
 *           0.1 are the standard values from Winkler's paper.
 * Input:    a, b - the two words to compare
 * Output:   distance between 0.0 (identical) and 1.0 (nothing in common)
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
double JaroWinklerDistance::distance(const std::string& a, const std::string& b) const
{
    double jaro = jaroSimilarity(a, b);
    double similarity = jaro;

    if (jaro > 0.7)
    {
        // Count how many letters at the very start are the same (max 4)
        size_t prefix = 0;
        size_t maxPrefix = std::min(static_cast<size_t>(4), std::min(a.size(), b.size()));
        while (prefix < maxPrefix && a[prefix] == b[prefix])
        {
            prefix++;
        }
        similarity = jaro + prefix * 0.1 * (1.0 - jaro);
    }

    return 1.0 - similarity;
}

/*
 * Function: JaroWinklerDistance::getName
 * Purpose:  Heading text used when printing this measure's results.
 * Input:    none
 * Output:   "Jaro-Winkler Distance"
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
std::string JaroWinklerDistance::getName() const
{
    return "Jaro-Winkler Distance";
}
