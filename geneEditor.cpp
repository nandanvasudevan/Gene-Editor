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
#include <algorithm>

//TODO Verify changes made

std::pair<PopulationRegion, std::string> getRegion(const int region, const int offset, const std::string &sPrefix)
{
    std::pair<PopulationRegion, std::string> pairRet;
    PopulationRegion eRegion;
    std::string sRegion;
    unsigned int actualRegion = std::clamp(region + offset, 0, MAX_SUPPORTED_REGIONS);

    switch (actualRegion)
    {
    case 0:
        eRegion = REG_AFRICA;
        sRegion = sPrefix + "africa";
        break;
    case 1:
        eRegion = REG_AMERICA;
        sRegion = sPrefix + "america";
        break;
    case 2:
        eRegion = REG_EAST_ASIA;
        sRegion = sPrefix + "east_asia";
        break;
    case 3:
        eRegion = REG_EUROPE;
        sRegion = sPrefix + "europe";
        break;
    case 4:
        eRegion = REG_SOUTH_ASIA;
        sRegion = sPrefix + "south_asia";
        break;
    default:
        eRegion = REG_AFRICA;
        sRegion = sPrefix + "africa";
        std::cout << FONT_CYAN_BOLD_INVERSE << "\n\tInvalid choice. Region defaulted to Africa\n"
                  << FONT_RESET;
    }

    pairRet.first = eRegion;
    pairRet.second = sRegion;

    return std::make_pair(eRegion, sRegion);
}

bool ImportPopulationData(vPopulationDetail &vPopData, std::string sPopFilePath, bool bIgnoreHeader)
{
    std::ifstream fPopFile;
    fPopFile.open(sPopFilePath);

    if (fPopFile.is_open())
    {
        std::string sRawFile;
        std::clog << FONT_CYAN_BOLD_INVERSE << "\n\tImporting population data\r";

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

            unsigned int uiLinesRead = 0;
            /* {
                std::clog << "\n";
            } */
            while (getline(sRawStream, sLine, '\n'))
            {
                {
                    std::clog << "\tLines read: " << ++uiLinesRead << '\r';
                }
                std::stringstream sLineStream(sLine);
                std::string sTabDelemitted;
                sPopulationDetail popData = new sPopulationDetail();
                size_t state = 0;

                while (getline(sLineStream, sTabDelemitted, '\t'))
                {
                    char cRead = '0';
                    bool bSkipLine = false;

                    switch (state)
                    {
                    case 0:

                        popData.sVerificationID = sTabDelemitted;
                        ++state;
                        break;

                    case 1:
                        popData.sChromosome = sTabDelemitted;
                        ++state;
                        break;

                    case 2:
                        popData.szLocation = stoi(sTabDelemitted);
                        ++state;
                        break;

                    case 3:
                        popData.sDbSNP = sTabDelemitted;
                        ++state;
                        break;

                    case 4:
                        cRead = sTabDelemitted.c_str()[0];
                        if (cRead != '-')
                        {
                            popData.cReferenceAllele = cRead;
                            ++state;
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
                            ++state;
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
                            ++state;
                        }
                        else
                        {
                            bSkipLine = true;
                        }
                        break;

                    case 7:
                        popData.iRatio[0] = stof(sTabDelemitted) * 10000;
                        ++state;
                        break;
                    case 9:
                        popData.iRatio[1] = stof(sTabDelemitted) * 10000;
                        ++state;
                        break;
                    case 8:
                        popData.iRatio[2] = stof(sTabDelemitted) * 10000;
                        ++state;
                        break;
                    case 10:
                        popData.iRatio[3] = stof(sTabDelemitted) * 10000;
                        ++state;
                        break;
                    case 11:
                        popData.iRatio[4] = stof(sTabDelemitted) * 10000;
                        ++state;
                        break;
                    default:
                        break;
                    }

                    if (bSkipLine)
                    {
                        break;
                    }
                }
                vPopData.push_back(popData);
            }

            {
                std::clog << "\tImporting population data... " << GLYPH_CHECK_MARK << FONT_RESET << '\n';
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
        std::clog << FONT_RED_BOLD_INVERSE << "\n\tDeriving change file... " << GLYPH_CROSS_MARK << FONT_RESET;
        std::clog << FONT_RED_BOLD << "\n\tInvalid population data loaded\n"
                  << FONT_RESET;
        return false;
    }

    std::clog << FONT_CYAN_BOLD_INVERSE << "\n\tDeriving change file...\r";
    unsigned int tempCount = 0;
    for (auto popDataIterator : vPopData)
    {
        sChangeData = new sChangeDetail();
        unsigned int iRatio = popDataIterator.iRatio[(int)(options.eRegion)];
        if ((popDataIterator.szLocation >= options.szStartIndex) && (popDataIterator.szLocation <= options.szEndIndex))
        {
            if (iRatio > options.iRatioThreshold)
            {
                ++tempCount;
                sChangeData.szLocation = popDataIterator.szLocation;
                sChangeData.cReferenceAllele = popDataIterator.cReferenceAllele;
                sChangeData.cAlternateAllele = popDataIterator.cAlternateAllele;
                vChangeData.push_back(sChangeData);
            }
        }
    }
    std::clog << "\tDeriving change file... " << GLYPH_CHECK_MARK << FONT_RESET << "\n";

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
    unsigned int uiChangesMade = 0;
    unsigned int uiMismatches = 0;

    std::clog << FONT_CYAN_BOLD_INVERSE << "\n\tModifying gene... \n";
    for (char &cNucleotide : sGene)
    {

        if (annotationIterator->szLocation == index + startOffset)
        {
            if (cNucleotide == annotationIterator->cReferenceAllele)
            {
                ++uiChangesMade;
                cNucleotide = annotationIterator->cAlternateAllele;
                annotationIterator++;
            }
            else
            {
                ++uiMismatches;
            }
            std::clog << FONT_CYAN_BOLD << "\tReplaced: " << uiChangesMade << FONT_RED_BOLD << " Mismatch: " << uiMismatches << "\n";
        }
        index++;
    }
    {
        std::clog << "\n"
                  << FONT_RESET;
    }

    fModifiedFile << sGene;

    fAnnotationFile.close();
    fModifiedFile.close();
}

// void