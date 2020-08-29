#include "geneEditor.hpp"
#include <fstream>
#include <string.h>

int main()
{
    std::vector<std::pair<unsigned int, std::pair<char, char>>> vAnnotated;
    std::ifstream fAnnotationFile;
    // const char *originalDataFile = "./resources/cq1b/original.txt";
    // fAnnotationFile.open(originalDataFile);
    std::string sAnnotationPath = "./resources/c1qb/changeList.txt";
    std::string sGenePath = "./resources/c1qb/original.txt";
    std::string sOutputPath = "./resources/c1qb/modified.txt";

    ImportChanges(vAnnotated, sAnnotationPath);
    ModifyGene(vAnnotated, sGenePath, sOutputPath);
}