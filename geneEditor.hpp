/**
 * @file geneEditor.hpp
 * @author Nandan V (nvpotti.mec@gmail.com)
 * @brief Header for functions to modify nucleotides in a given gene.
 * @version 0.1
 * @date Saturday, 29-Aug-2020
 * 
 * @copyright Copyright (c) 2020
 */

#ifndef GENE_EDITOR
#define GENE_EDITOR
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#define ORIGINAL_DATA_FILE "original.txt"
#define ANNOTATION_FILE "changeList.txt"
#define GENE "c1qb"
#define PATH "./resources/" + GENE + "/"

enum PopulationRegion
{
    REG_NONE = 0,
    REG_AFRICA,
    REG_AMERICA,
    REG_EAST_ASIA,
    REG_EUROPE,
    REG_SOUTH_ASIA
};

struct sChangeDetail
{
    size_t szLocation;
    char cReferenceAllele;
    char cAlternateAllele;
    char cMinorAllele;

    sChangeDetail()
        : szLocation(0),
          cReferenceAllele('-'),
          cAlternateAllele('-'),
          cMinorAllele('-')
    {
    }

    sChangeDetail(sChangeDetail *og) : szLocation(og->szLocation),
                                       cReferenceAllele(og->cReferenceAllele),
                                       cAlternateAllele(og->cAlternateAllele),
                                       cMinorAllele(og->cMinorAllele)
    {
    }
};

struct sPopulationDetail
{
    std::string sVerificationID;
    std::string sChromosome;
    size_t szLocation;
    std::string sDbSNP;
    char cReferenceAllele;
    char cAlternateAllele;
    char cMinorAllele;
    PopulationRegion eRegion;

    sPopulationDetail()
        : sVerificationID("-"),
          sChromosome("-"),
          szLocation(0),
          sDbSNP("-"),
          cReferenceAllele('-'),
          cAlternateAllele('-'),
          cMinorAllele('-'),
          eRegion(REG_NONE)

    {
    }

    sPopulationDetail(sPopulationDetail *og)
        : sVerificationID(og->sVerificationID),
          sChromosome(og->sChromosome),
          szLocation(og->szLocation),
          sDbSNP(og->sDbSNP),
          cReferenceAllele(og->cReferenceAllele),
          cAlternateAllele(og->cAlternateAllele),
          cMinorAllele(og->cMinorAllele)
    {
    }
};

typedef std::vector<sChangeDetail> vChangeDetail;
typedef std::vector<sPopulationDetail> vPopulationDetail;

bool ImportPopulationData(vPopulationDetail &vPopData, vChangeDetail &vChangeData, std::string sPopFilePath);

/**
 * @brief Create an annotation vector with information related to the changes to be made.
 * 
 * @param vAnnotatedVector Vector containing data for changes to be made
 * @param sPath Path to file containing changes
 * @return true if import is successful, else false
 */
bool ImportChangeFile(vChangeDetail &vAnnotatedVector, std::string);

/**
 * @brief Modify nucleotides in a gene based on passed data
 * 
 * @param vAnnotatedVector Vector containing data on changes to be made
 * @param sInputPath path to file containing gene sequence
 * @param sOutputPath Path to modified file
 */
void ModifyGene(vChangeDetail &vAnnotatedVector, std::string sInputPath, std::string sOutputPath);

#endif