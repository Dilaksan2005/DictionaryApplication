/*
 * File:     JaroWinklerDistance.h
 * Purpose:  Jaro-Winkler distance measure - my own choice for the "third
 *           measure" (derived from SimmilarityMeasure).
 * Author:   Dilaksan Gnanasegaran
 * Created:  30/10/2026
 */

#ifndef JAROWINKLERDISTANCE_H
#define JAROWINKLERDISTANCE_H

#include "SimmilarityMeasure.h"

class JaroWinklerDistance : public SimmilarityMeasure
{
public:
    double distance(const std::string& a, const std::string& b) const override;
    std::string getName() const override;

private:
    double jaroSimilarity(const std::string& a, const std::string& b) const;
};

#endif
