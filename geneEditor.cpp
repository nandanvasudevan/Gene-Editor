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

bool ImportChanges(std::vector<std::pair<unsigned int, std::pair<char, char>>> &vAnnotatedVector, std::string sPath)
{
    std::ifstream fAnnotationFile;
    fAnnotationFile.open(sPath);

    if (fAnnotationFile.is_open())
    {
        std::string tabDelimitted;
        std::string rawAnnotation;
        std::pair<char, char> pairNucleotide;
        std::pair<unsigned int, std::pair<char, char>> pairChangeInfo;
        unsigned int uiElementindex;
        char cOriginalNucleotide;
        char cReplacementNucleotide;

        while (getline(fAnnotationFile, rawAnnotation))
        {
            std::stringstream wordString(rawAnnotation);
            std::string word;

            while (getline(wordString, word, '\t'))
            {
                static int parameterID = 0;

                switch (parameterID)
                {
                case 0:
                    uiElementindex = std::stoi(word);
                    pairChangeInfo.first = uiElementindex;
                    parameterID = 1;
                    break;
                case 1:
                    cOriginalNucleotide = word.c_str()[0];
                    pairNucleotide.first = cOriginalNucleotide;
                    parameterID = 2;
                    break;
                case 2:
                    cReplacementNucleotide = word.c_str()[0];
                    pairNucleotide.second = cReplacementNucleotide;

                    pairChangeInfo.second = pairNucleotide;
                    vAnnotatedVector.push_back(pairChangeInfo);
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
        //     std::clog << "Index: " << iterator.first << '\t';
        //     std::clog << "Existing nucleotide: " << iterator.second.first << '\t';
        //     std::clog << "Replacement nucleotide: " << iterator.second.second << '\n';
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

void ModifyGene(std::vector<std::pair<unsigned int, std::pair<char, char>>> &vAnnotatedVector, std::string sInputPath, std::string sOutputPath)
{
    std::ifstream fAnnotationFile;
    std::ofstream fModifiedFile;
    std::string sGene;

    fAnnotationFile.open(sInputPath);
    fModifiedFile.open(sOutputPath, std::ios::out | std::ios::app);

    fAnnotationFile.seekg(0, std::ios::end);
    sGene.reserve(fAnnotationFile.tellg());
    fAnnotationFile.seekg(0, std::ios::beg);

    sGene.assign((std::istreambuf_iterator<char>(fAnnotationFile)), std::istreambuf_iterator<char>());

    std::vector<std::pair<unsigned int, std::pair<char, char>>>::iterator annotationIterator = vAnnotatedVector.begin();

    for (char &cNucleotide : sGene)
    {
        static size_t index = 1;

        if (annotationIterator->first == index)
        {
            if (cNucleotide == annotationIterator->second.first)
            {
                std::clog << "Index: " << annotationIterator->first << '\t';
                std::clog << "Existing nucleotide: " << cNucleotide << " : " << sGene.at(index - 1) << '\t';

                std::exchange(cNucleotide, annotationIterator->second.second);

                std::clog << "Replacement nucleotide: " << annotationIterator->second.second << " : " << sGene.at(index - 1) << '\n';
            }
            else
            {
                std::clog << "Index: " << annotationIterator->first << '\t';
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