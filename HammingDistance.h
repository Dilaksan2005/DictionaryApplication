/*
 * File:     HammingDistance.h
 * Purpose:  Hamming distance measure (derived from SimmilarityMeasure).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef HAMMINGDISTANCE_H
#define HAMMINGDISTANCE_H

#include "SimmilarityMeasure.h"

class HammingDistance : public SimmilarityMeasure
{
public:
    double distance(const std::string& a, const std::string& b) const override;
    std::string getName() const override;
};

#endif
