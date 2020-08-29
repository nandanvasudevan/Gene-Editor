#include "geneEditor.hpp"
#include <fstream>
#include <string.h>

int main()
{
    std::vector<std::pair<unsigned int, std::pair<char, char>>> vAnnotated;
    std::ifstream fAnnotationFile;
    // const char *originalDataFile = "./resources/cq1b/original.txt";
    // fAnnotationFile.open(originalDataFile);

    std::string sGeneName = "";

    while (true)
    {
        std::cout << "\nEnter name of gene (q to quit): ";
        std::cin >> sGeneName;

        if (sGeneName == "q")
        {
            break;
        }

        std::string sAnnotationPath = "./resources/" + sGeneName + "/changeList.txt";
        std::string sGenePath = "./resources/" + sGeneName + "/original.txt";
        std::string sOutputPath = "./resources/" + sGeneName + "/modified.txt";

        // std::clog << sAnnotationPath;

        if (ImportChanges(vAnnotated, sAnnotationPath))
        {
            ModifyGene(vAnnotated, sGenePath, sOutputPath);
        }
    }
}