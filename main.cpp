#include "geneEditor.hpp"
#include <fstream>
#include <string.h>

int main()
{
    while (true)
    {
        vChangeDetail vAnnotated;
        std::ifstream fAnnotationFile;
        std::string sGeneName = "";

        vAnnotated.reserve(2000);

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

        if (ImportChangeFile(vAnnotated, sAnnotationPath))
        {
            ModifyGene(vAnnotated, sGenePath, sOutputPath);
        }
    }
}