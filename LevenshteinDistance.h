/*
 * File:     LevenshteinDistance.h
 * Purpose:  Levenshtein (edit) distance measure (derived from SimmilarityMeasure).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef LEVENSHTEINDISTANCE_H
#define LEVENSHTEINDISTANCE_H

#include "SimmilarityMeasure.h"

class LevenshteinDistance : public SimmilarityMeasure
{
public:
    double distance(const std::string& a, const std::string& b) const override;
    std::string getName() const override;
};

#endif
