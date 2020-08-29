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

/**
 * @brief Create an annotation vector with information related to the changes to be made.
 * 
 * @param vAnnotatedVector Vector containing data for changes to be made
 * @param sPath Path to file containing changes
 */
bool ImportChanges(std::vector<std::pair<unsigned int, std::pair<char, char>>> &vAnnotatedVector, std::string);

/**
 * @brief Modify nucleotides in a gene based on passed data
 * 
 * @param vAnnotatedVector Vector containing data on changes to be made
 * @param sInputPath path to file containing gene sequence
 * @param sOutputPath Path to modified file
 */
void ModifyGene(std::vector<std::pair<unsigned int, std::pair<char, char>>> &vAnnotatedVector, std::string sInputPath, std::string sOutputPath);

#endif