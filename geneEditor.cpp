/**
 * @file geneEditor.cpp
 * @author Nandan V (nvpotti.mec@gmail.com)
 * @brief Modify a text file based on changes from a tab delimitted nx3 file structure.
 * @version 0.1
 * @date Saturday, 29-Aug-2020
 * 
 * @copyright Copyright (c) 2020 
 */
#include "geneEditor.hpp"
#include "common.hpp"

#include <string.h>
#include <cstdio>
#include <sstream>

std::string getRegion(PopulationRegion eRegion)
{
    std::string sRegion;
    switch (eRegion)
    {
    case 0:
        sRegion = "__africa";
        break;
    case 1:
        sRegion = "__america";
        break;
    case 2:
        sRegion = "__east_asia";
        break;
    case 3:
        sRegion = "__europe";
        break;
    case 4:
        sRegion = "__south_asia";
        break;
    default:
        sRegion = "";
    }

    return sRegion;
}

bool ImportPopulationData(vPopulationDetail &vPopData, std::string sPopFilePath, bool bIgnoreHeader)
{
    std::ifstream fPopFile;
    fPopFile.open(sPopFilePath);

    if (fPopFile.is_open())
    {
        std::string sRawFile;

        { // Copy entire file to string and close the file
            fPopFile.seekg(0, std::ios::end);
            sRawFile.reserve(fPopFile.tellg());
            fPopFile.seekg(0, std::ios::beg);

            sRawFile.assign((std::istreambuf_iterator<char>(fPopFile)), std::istreambuf_iterator<char>());

            fPopFile.close();
        }

        {
            std::string sLine;
            std::stringstream sRawStream(sRawFile);

            if (bIgnoreHeader)
            {
                getline(sRawStream, sLine, '\n');
            }

            while (getline(sRawStream, sLine, '\n'))
            {
                std::stringstream sLineStream(sLine);
                std::string sTabDelemitted;
                sPopulationDetail popData = new sPopulationDetail();
                size_t state = 0;

                float fRatioAccumulator = -0.001;
                while (getline(sLineStream, sTabDelemitted, '\t'))
                {
                    static size_t iRatioIndex = 0;
                    char cRead = '0';
                    bool bSkipLine = false;

                    switch (state)
                    {
                    case 0:

                        popData.sVerificationID = sTabDelemitted;
                        state++;
                        break;

                    case 1:
                        popData.sChromosome = sTabDelemitted;
                        state++;
                        break;

                    case 2:
                        popData.szLocation = stoi(sTabDelemitted);
                        state++;
                        break;

                    case 3:
                        popData.sDbSNP = sTabDelemitted;
                        state++;
                        break;

                    case 4:
                        cRead = sTabDelemitted.c_str()[0];
                        if (cRead != '-')
                        {
                            popData.cReferenceAllele = cRead;
                            state++;
                        }
                        else
                        {
                            bSkipLine = true;
                        }
                        break;

                    case 5:
                        cRead = sTabDelemitted.c_str()[0];
                        if (cRead != '-')
                        {
                            popData.cAlternateAllele = cRead;
                            state++;
                        }
                        else
                        {
                            bSkipLine = true;
                        }
                        break;

                    case 6:
                        cRead = sTabDelemitted.c_str()[0];
                        if (cRead != '-')
                        {
                            popData.cMinorAllele = cRead;
                            state++;
                        }
                        else
                        {
                            bSkipLine = true;
                        }
                        break;

                    case 7:
                        fRatioAccumulator = -0.0001;
                        iRatioIndex = state;
                        [[fallthrough]];
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        popData.iRatio[state - iRatioIndex] = stof(sTabDelemitted) * 10000;
                        fRatioAccumulator += popData.iRatio[state - iRatioIndex];
                        state++;
                        break;
                    default:
                        break;
                    }

                    if (bSkipLine)
                    {
                        break;
                    }
                }
                if (fRatioAccumulator > 0)
                {
                    vPopData.push_back(popData);
                }
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool DeriveChangeFile(const vPopulationDetail &vPopData, vChangeDetail &vChangeData, const ChangeFileOptions &options)
{
    sChangeDetail sChangeData;

    if (vPopData.size() <= 0)
    {
        return false;
    }

    for (auto popDataIterator : vPopData)
    {
        sChangeData = new sChangeDetail();
        unsigned int iRatio = popDataIterator.iRatio[(int)options.eRegion];
        if ((popDataIterator.szLocation >= options.szStartIndex) && (popDataIterator.szLocation <= options.szEndIndex))
        {
            if (iRatio > options.iRatioThreshold)
            {
                sChangeData.szLocation = popDataIterator.szLocation;
                sChangeData.cReferenceAllele = popDataIterator.cReferenceAllele;
                sChangeData.cAlternateAllele = popDataIterator.cAlternateAllele;
                vChangeData.push_back(sChangeData);
            }
        }
    }

    return true;
}

[[deprecated]] bool ImportChangeFile(vChangeDetail &vAnnotatedVector, std::string sPath)
{
    std::ifstream fAnnotationFile;
    fAnnotationFile.open(sPath);

    if (fAnnotationFile.is_open())
    {
        std::string tabDelimitted;
        std::string rawAnnotation;
        std::pair<char, char> pairNucleotide;
        std::pair<unsigned int, std::pair<char, char>> pairChangeInfo;
        size_t uiElementindex;
        char cReferenceAllele;
        char cReplacementNucleotide;

        while (getline(fAnnotationFile, rawAnnotation))
        {
            std::stringstream wordString(rawAnnotation);
            std::string word;

            while (getline(wordString, word, '\t'))
            {
                static int parameterID = 0;
                static sChangeDetail detail;

                switch (parameterID)
                {
                case 0:
                    detail = new sChangeDetail();
                    uiElementindex = std::stoi(word);
                    detail.szLocation = uiElementindex;

                    parameterID = 1;
                    break;
                case 1:
                    cReferenceAllele = word.c_str()[0];
                    detail.cReferenceAllele = cReferenceAllele;

                    parameterID = 2;
                    break;
                case 2:
                    cReplacementNucleotide = word.c_str()[0];
                    detail.cAlternateAllele = cReplacementNucleotide;

                    vAnnotatedVector.push_back(detail);
                    parameterID = 0;
                    break;
                default:
                    std::clog << "Default triggered: " << word << '\n';
                    break;
                }
            }
        }

        // for (const auto &iterator : vAnnotatedVector)
        // {
        //     std::clog << "Index: " << iterator.szLocation << '\t';
        //     std::clog << "Existing nucleotide: " << iterator.cReferenceAllele << '\t';
        //     std::clog << "Replacement nucleotide: " << iterator.cAlternateAllele << '\n';
        // }
    }
    else
    {
        std::cerr << "File not open\n";
        fAnnotationFile.close();
        return false;
    }

    fAnnotationFile.close();
    return true;
}

void ModifyGene(vChangeDetail &vAnnotatedVector, std::string sInputPath, std::string sOutputPath, size_t startOffset)
{
    std::ifstream fAnnotationFile;
    std::ofstream fModifiedFile;
    std::string sGene;

    fAnnotationFile.open(sInputPath);
    fModifiedFile.open(sOutputPath, std::ios::out);

    { // Copy entire file to string and close the file
        fAnnotationFile.seekg(0, std::ios::end);
        sGene.reserve(fAnnotationFile.tellg());
        fAnnotationFile.seekg(0, std::ios::beg);

        sGene.assign((std::istreambuf_iterator<char>(fAnnotationFile)), std::istreambuf_iterator<char>());
    }

    vChangeDetail::iterator annotationIterator = vAnnotatedVector.begin();

    size_t index = 0;

    for (char &cNucleotide : sGene)
    {

        if (annotationIterator->szLocation == index + startOffset)
        {
            if (cNucleotide == annotationIterator->cReferenceAllele)
            {
                cNucleotide = annotationIterator->cAlternateAllele;
                annotationIterator++;
            }
            else
            {
                std::clog << FONT_RED_BOLD_INVERSE << "Nucleotide mismatch | Index match" << FONT_RESET << FONT_RED_BOLD << "\n"
                          << "Index = " << annotationIterator->szLocation << " : " << index << " Curr: " << cNucleotide << " Alt: " << annotationIterator->cAlternateAllele << '\n'
                          << FONT_RESET;
            }
        }
        index++;
    }

    fModifiedFile << sGene;

    fAnnotationFile.close();
    fModifiedFile.close();
}