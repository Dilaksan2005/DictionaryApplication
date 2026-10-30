/*
 * File:     LevenshteinDistance.cpp
 * Purpose:  Implementation of the Levenshtein distance.
 *
 * References (I wrote the code myself from these descriptions):
 *   Levenshtein, V. I. (1966). Binary codes capable of correcting deletions,
 *       insertions and reversals. Soviet Physics Doklady, 10(8), 707-710.
 *   Wikipedia contributors. (n.d.). Levenshtein distance. Wikipedia.
 *       https://en.wikipedia.org/wiki/Levenshtein_distance
 *       (the "iterative with two matrix rows" version)
 *
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#include "LevenshteinDistance.h"
#include <vector>
#include <algorithm>

/*
 * Function: LevenshteinDistance::distance
 * Purpose:  Minimum number of single-letter edits (insert, delete or
 *           substitute) needed to turn word a into word b.
 *           "kitten" -> "sitting" is 3 (k->s, e->i, add g).
 *
 *           It's classic dynamic programming. Imagine a grid where cell
 *           (i, j) holds the distance between the first i letters of a and
 *           the first j letters of b. Each cell only depends on the cell to
 *           its left, above, and diagonally up-left, so I only keep TWO rows
 *           of the grid in memory instead of the whole thing. That matters
 *           because Smart Search runs this ~105,000 times per search.
 * Input:    a, b - the two words to compare
 * Output:   the edit distance (whole number stored in a double); 0 = identical
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
double LevenshteinDistance::distance(const std::string& a, const std::string& b) const
{
    // Easy cases first: comparing against an empty word means "insert/delete every letter"
    if (a.empty()) return static_cast<double>(b.size());
    if (b.empty()) return static_cast<double>(a.size());

    // previous = row i-1 of the grid, current = row i.
    // Row 0 is the cost of building the first j letters of b from nothing: 0,1,2,3...
    std::vector<size_t> previous(b.size() + 1);
    std::vector<size_t> current(b.size() + 1);
    for (size_t j = 0; j <= b.size(); j++)
    {
        previous[j] = j;
    }

    for (size_t i = 1; i <= a.size(); i++)
    {
        current[0] = i;     // turning i letters of a into nothing costs i deletions

        for (size_t j = 1; j <= b.size(); j++)
        {
            // Matching letters cost nothing, different letters cost one substitution
            size_t substitutionCost = (a[i - 1] == b[j - 1]) ? 0 : 1;

            size_t deletion     = previous[j] + 1;                          // drop a letter from a
            size_t insertion    = current[j - 1] + 1;                       // add a letter to a
            size_t substitution = previous[j - 1] + substitutionCost;       // swap a letter

            current[j] = std::min(std::min(deletion, insertion), substitution);
        }

        std::swap(previous, current);   // the row we just built becomes "previous" for the next pass
    }

    // After the final swap, the finished row lives in 'previous'
    return static_cast<double>(previous[b.size()]);
}

/*
 * Function: LevenshteinDistance::getName
 * Purpose:  Heading text used when printing this measure's results.
 * Input:    none
 * Output:   "Levenshtein Distance"
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */
std::string LevenshteinDistance::getName() const
{
    return "Levenshtein Distance";
}
