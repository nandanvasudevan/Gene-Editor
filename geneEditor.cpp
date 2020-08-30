/**
 * @file geneEditor.cpp
 * @author Nandan V (nvpotti.mec@gmail.com)
 * @brief Modify a text file based on changes from a tab delimitted nx3 file structure.
 * @version 0.1
 * @date Saturday, 29-Aug-2020
 * 
 * @copyright Copyright (c) 2020 
 */

#include <string.h>
#include <cstdio>
#include <sstream>
#include <streambuf>
#include "geneEditor.hpp"

bool ImportPopulationData(vPopulationDetail &vPopData, vChangeDetail &vChangeData, std::string sPopFilePath)
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
    }

    return true;
}

bool ImportChangeFile(vChangeDetail &vAnnotatedVector, std::string sPath)
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
                static sChangeDetail *detail;

                switch (parameterID)
                {
                case 0:
                    detail = new sChangeDetail();
                    uiElementindex = std::stoi(word);
                    detail->szLocation = uiElementindex;

                    parameterID = 1;
                    break;
                case 1:
                    cReferenceAllele = word.c_str()[0];
                    detail->cReferenceAllele = cReferenceAllele;

                    parameterID = 2;
                    break;
                case 2:
                    cReplacementNucleotide = word.c_str()[0];
                    detail->cAlternateAllele = cReplacementNucleotide;

                    vAnnotatedVector.emplace_back(detail);
                    parameterID = 0;

                    delete detail;
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

void ModifyGene(vChangeDetail &vAnnotatedVector, std::string sInputPath, std::string sOutputPath)
{
    std::ifstream fAnnotationFile;
    std::ofstream fModifiedFile;
    std::string sGene;

    fAnnotationFile.open(sInputPath);
    fModifiedFile.open(sOutputPath, std::ios::out | std::ios::app);

    { // Copy entire file to string and close the file
        fAnnotationFile.seekg(0, std::ios::end);
        sGene.reserve(fAnnotationFile.tellg());
        fAnnotationFile.seekg(0, std::ios::beg);

        sGene.assign((std::istreambuf_iterator<char>(fAnnotationFile)), std::istreambuf_iterator<char>());
    }

    vChangeDetail::iterator annotationIterator = vAnnotatedVector.begin();

    for (char &cNucleotide : sGene)
    {
        static size_t index = 1;

        if (annotationIterator->szLocation == index)
        {
            if (cNucleotide == annotationIterator->cReferenceAllele)
            {
                std::clog << "Index: " << annotationIterator->cReferenceAllele << '\t';
                std::clog << "Existing nucleotide: " << cNucleotide << " : " << sGene.at(index - 1) << '\t';

                std::exchange(cNucleotide, annotationIterator->cAlternateAllele);

                std::clog << "Replacement nucleotide: " << annotationIterator->cAlternateAllele << " : " << sGene.at(index - 1) << '\n';
            }
            else
            {
                std::clog << "Index: " << annotationIterator->szLocation << '\t';
                std::clog << index << " : " << cNucleotide << '\n';
            }
            annotationIterator++;
        }
        index++;
    }

    fModifiedFile << sGene;

    fAnnotationFile.close();
    fModifiedFile.close();
}