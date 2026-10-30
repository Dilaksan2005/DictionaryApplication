/*
 * File:     SimmilarityMeasure.h
 * Purpose:  Abstract base class for every "how close are these two words?"
 *           measure used by Smart Search (Task 5).
 *           (The odd spelling "Simmilarity" is copied from the brief on
 *           purpose, so the class name matches what the marker expects.)
 *
 *           Every measure answers the same question - "what's the distance
 *           between word A and word B?" - where SMALLER means MORE similar and
 *           0 means identical. That way AdvancedDictionary can keep a
 *           vector<SimmilarityMeasure*> and treat them all the same way
 *           (polymorphism), without knowing which one is which.
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef SIMMILARITYMEASURE_H
#define SIMMILARITYMEASURE_H

#include <string>

class SimmilarityMeasure
{
public:
    // Virtual destructor is essential: we delete these through base-class
    // pointers, and without it the derived destructors would never run.
    virtual ~SimmilarityMeasure() {}

    /*
     * Function: SimmilarityMeasure::distance
     * Purpose:  Pure virtual - each measure supplies its own calculation.
     * Input:    a, b - the two words to compare
     * Output:   distance as a double; lower = closer, 0 = identical
     * Author:   Dilaksan Gnanasegaran
     * Created:  30/10/2026
     */
    virtual double distance(const std::string& a, const std::string& b) const = 0;

    /*
     * Function: SimmilarityMeasure::getName
     * Purpose:  Pure virtual - lets Smart Search print a heading for each measure.
     * Input:    none
     * Output:   human-readable name of the measure
     * Author:   Dilaksan Gnanasegaran
     * Created:  30/10/2026
     */
    virtual std::string getName() const = 0;
};

#endif
