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
#include <math.h>

#define MAX_SUPPORTED_REGIONS 5
#define ORIGINAL_DATA_FILE "original.txt"
#define ANNOTATION_FILE "changeList.txt"
#define GENE "c1qb"
#define PATH "./resources/" + GENE + "/"

/**
 * @brief Enumeration for selection of population region.
 */
enum PopulationRegion
{
    REG_AFRICA = 0, ///< African region
    REG_AMERICA,    ///< American region
    REG_EAST_ASIA,  ///< East asian region
    REG_EUROPE,     ///< European region
    REG_SOUTH_ASIA  ///< South asian region
};

/**
 * @brief Structure maintaining all the necessary details required to modify input gene.
 */
struct sChangeDetail
{
    size_t szLocation;     ///< The location of the nucleotide
    char cReferenceAllele; ///< The reference allele to be existing at @ref szLocation
    char cAlternateAllele; ///< The alternate allele to be swapped with @ref cReferenceAllele
    char cMinorAllele;     ///< Currently unused

    sChangeDetail()
        : szLocation(0),
          cReferenceAllele('-'),
          cAlternateAllele('-')
    {
    }

    sChangeDetail(sChangeDetail *og) : szLocation(og->szLocation),
                                       cReferenceAllele(og->cReferenceAllele),
                                       cAlternateAllele(og->cAlternateAllele)
    {
    }
};

/**
 * @brief Structure to store data from the entire population spread across @ref PopulationRegion
 */
struct sPopulationDetail
{
    std::string sVerificationID; ///< Unused
    std::string sChromosome;     ///< Chromosome ID of nucleotide
    size_t szLocation;           ///< The location of nucleotide in the gene
    std::string sDbSNP;          ///< Unused
    char cReferenceAllele;       ///< Nucleotide to be replaced
    char cAlternateAllele;       ///< Nucleotide to be replaced with
    char cMinorAllele;           ///< Unused
    unsigned int iRatio[5];      ///< Percentage of occurace of mutation across the 5 main regions of @ref PopulationRegion

    sPopulationDetail()
        : sVerificationID("-"),
          sChromosome("-"),
          szLocation(0),
          sDbSNP("-"),
          cReferenceAllele('-'),
          cAlternateAllele('-'),
          cMinorAllele('-'),
          iRatio{0, 0, 0, 0, 0}

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
        for (int i = 0; i < 5; i++)
        {
            iRatio[i] = og->iRatio[i];
        }
    }
};

/**
 * @brief Structure containing options to be passed in while processing
 */
struct ChangeFileOptions
{
    PopulationRegion eRegion;     ///< The region selected by the user
    unsigned int uiGain;          ///< Power of 10 to be multiplied with ratio to remove floating point comparison
    unsigned int iRatioThreshold; ///< The threshold percentage below which mutation must be ignored (x10000)
    size_t szStartIndex;          ///< Index to begin from (starting index from original gene file)
    size_t szEndIndex;            ///< Index to end at (last index of original gene file)

    ChangeFileOptions() : eRegion(REG_AFRICA),
                          uiGain(4),
                          iRatioThreshold(0),
                          szStartIndex(0),
                          szEndIndex(0)
    {
    }

    ChangeFileOptions(PopulationRegion region, float threshold, size_t start, size_t end, unsigned int gain = 4)
        : eRegion(region),
          uiGain(gain),
          iRatioThreshold(threshold * pow(10, uiGain)),
          szStartIndex(start),
          szEndIndex(end)
    {
    }
};

typedef std::vector<sChangeDetail> vChangeDetail;
typedef std::vector<sPopulationDetail> vPopulationDetail;

[[nodiscard]] std::pair<PopulationRegion, std::string> getRegion(const int region, const int offset = 0, const std::string &sPrefix = "_");

/**
 * @brief Imports population data into a vector from a specified path
 * 
 * @param vPopData Vector to which data is imported into
 * @param sPopFilePath Path at which population data is present
 * @param bIgnoreHeader Ignores the first line
 * @return true Successful import
 * @return false File not found
 */
[[nodiscard]] bool ImportPopulationData(vPopulationDetail &vPopData, std::string sPopFilePath, bool bIgnoreHeader = true);

/**
 * @brief Derive change file from population data
 * 
 * @param vPopData Vector containing population data
 * @param vChangeData Vector to store details of changes to be made based on user input
 * @param options Options passed from user input
 * @return true Successful
 * @return false Invalid population data
 */
[[nodiscard]] bool DeriveChangeFile(const vPopulationDetail &vPopData, vChangeDetail &vChangeData, const ChangeFileOptions &options);

/**
 * @brief Create an annotation vector with information related to the changes to be made.
 * 
 * @param vAnnotatedVector Vector containing data for changes to be made
 * @param sPath Path to file containing changes
 * @return true if import is successful, else false
 */
[[nodiscard]] bool ImportChangeFile(vChangeDetail &vAnnotatedVector, std::string);

/**
 * @brief Modify nucleotides in a gene based on passed data
 * 
 * @param vAnnotatedVector Vector containing data on changes to be made
 * @param sInputPath path to file containing gene sequence
 * @param sOutputPath Path to modified file
 */
void ModifyGene(vChangeDetail &vAnnotatedVector, std::string sInputPath, std::string sOutputPath, size_t startOffset);
#endif