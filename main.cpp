#include "geneEditor.hpp"
#include <fstream>
#include <string.h>

int main()
{
    while (true)
    {
        vChangeDetail vAnnotated;
        vPopulationDetail vPopData;
        std::ifstream fAnnotationFile;
        std::string sGeneName = "";
        size_t startOffset;
        size_t endIndex;
        PopulationRegion eRegion = REG_AFRICA;
        float fRatioThreshold = 0.0;

        vAnnotated.reserve(2000);

        std::cout << "\nEnter name of gene (q to quit): ";
        std::cin >> sGeneName;

        if (sGeneName == "q")
        {
            break;
        }
        std::cout << "\nEnter start offset: ";
        std::cin >> startOffset;

        std::cout << "\nEnter end index: ";
        std::cin >> endIndex;

        std::cout << "\nEnter threshold for ratio: ";
        std::cin >> fRatioThreshold;

        {
            char geneMenuChoice;
            bool exitMenu = false;

            do
            {

                std::cout << R"(
Available regions
    1. Africa   2. America  3. East Asia    4. Europe   5. South Asia

Select region: )";
                {
                    unsigned int regionInput;
                    std::cin >> regionInput;

                    switch (regionInput - 1)
                    {
                    case 0:
                        eRegion = REG_AFRICA;
                        break;
                    case 1:
                        eRegion = REG_AMERICA;
                        break;
                    case 2:
                        eRegion = REG_EAST_ASIA;
                        break;
                    case 3:
                        eRegion = REG_EUROPE;
                        break;
                    case 4:
                        eRegion = REG_SOUTH_ASIA;
                        break;
                    default:
                        std::cout << "\nInvalid choice. Region defaulted to Africa";
                    }
                }
                std::string sAnnotationPath = "./resources/" + sGeneName + "/changeList.txt";
                std::string sPopFilePath = "./resources/" + sGeneName + "/C1QB-POP.txt";
                std::string sGenePath = "./resources/" + sGeneName + "/original.txt";
                std::string sOutputPath = "./resources/" + sGeneName + "/modified_" + getRegion(eRegion) + ".txt";

                // std::clog << sAnnotationPath;

                ChangeFileOptions *options = new ChangeFileOptions(REG_AMERICA, fRatioThreshold, startOffset, endIndex);

                ImportPopulationData(vPopData, sPopFilePath);
                DeriveChangeFile(vPopData, vAnnotated, *options);
                ModifyGene(vAnnotated, sGenePath, sOutputPath, startOffset);

                std::cout << R"(
Choices:
    c: Continue
    p: Previous Menu (to select another gene)
    q: Exit application

Select: )";
                std::cin >> geneMenuChoice;
                if (geneMenuChoice == 'p')
                {
                    exitMenu = true;
                }
                else if (geneMenuChoice == 'q')
                {
                    exit(0);
                }
            } while (false == exitMenu);
        }
    }
}