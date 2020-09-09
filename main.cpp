#include "geneEditor.hpp"
#include "common.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <experimental/filesystem>
#include <sstream>
#include <string.h>
#include <vector>
#include <set>
#include <bits/stdc++.h>

namespace fs = std::experimental::filesystem;

//TODO Output directory must be user defined

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
        std::set<std::pair<PopulationRegion, std::string>> MultipleRegionEntry;
        std::string pResources = "./resources/";
        std::string popFile;
        std::string sPopFilePath;
        std::string sOriginalGeneFile;
        float fRatioThreshold = 0.0;

        vAnnotated.reserve(2000);
        vPopData.reserve(1000);

        std::cout
            << "\nEnter name of gene (q to quit): ";
        std::cin >> sGeneName;
        std::transform(sGeneName.begin(), sGeneName.end(), sGeneName.begin(), ::tolower);

        // Check if {gene} directory exists
        {
            fs::path pGeneDirectory(pResources + sGeneName);
            fs::file_status fsStatus = fs::status(pGeneDirectory);

            if (fs::status_known(fsStatus) ? fs::exists(fsStatus) : fs::exists(pGeneDirectory))
            {
                std::clog << "\n\t" + GLYPH_CHECK_MARK << FONT_GREEN << " Directory for " + sGeneName + " found" + FONT_RESET;
                {
                    popFile = sGeneName;
                    std::transform(popFile.begin(), popFile.end(), popFile.begin(), ::toupper);

                    sPopFilePath = pResources + sGeneName + "/" + popFile + "-POP.txt";

                    fs::path pGenePopulationFile(sPopFilePath);
                    fsStatus = fs::status(pGenePopulationFile);
                    if (fs::status_known(fsStatus) ? fs::exists(fsStatus) : fs::exists(pGenePopulationFile))
                    {
                        std::clog << "\n\t" + GLYPH_CHECK_MARK << FONT_GREEN << " Population file for " + sGeneName + " found" + FONT_RESET;

                        {
                            sOriginalGeneFile = pResources + sGeneName + "/" + "original.txt";

                            fs::path pOriginalGeneFile(sOriginalGeneFile);
                            fsStatus = fs::status(pOriginalGeneFile);
                            if (fs::status_known(fsStatus) ? fs::exists(fsStatus) : fs::exists(pOriginalGeneFile))
                            {
                                std::clog << "\n\t" + GLYPH_CHECK_MARK << FONT_GREEN << " Original gene file for " + sGeneName + " found" + FONT_RESET;
                            }
                            else
                            {
                                std::clog << FONT_RED_BOLD_INVERSE + "\n\tCannot original gene file [original.txt] for \"" + sGeneName + "\" in " + pResources + sGeneName + FONT_RESET + "\n\n";
                                continue;
                            }
                        }
                    }
                    else
                    {
                        std::clog << FONT_RED_BOLD_INVERSE + "\n\tCannot find population file for \"" + sGeneName + "\" in " + pResources + sGeneName + FONT_RESET;
                        std::clog << FONT_RED_BOLD << R"(
            Probable causes:
                1. File does not exist
                2. Incorrect file name format
                        <GENE>-POP.txt
                                )" << FONT_RESET
                                  << '\n';
                        continue;
                    }
                }
            }
            else
            {
                std::clog << FONT_RED_BOLD_INVERSE + "\n\tCannot find gene \"" + sGeneName + "\" in " + pResources + FONT_RESET + "\n\n";
                continue;
            }
        }

        if (sGeneName == "q")
        {
            std::clog << FONT_GREEN_BOLD << "\n\tExiting..." << FONT_RESET << '\n';
            break;
        }
        std::cout
            << "\nEnter start offset: ";
        std::cin >> startOffset;

        std::cout << "\nEnter end index: ";
        std::cin >> endIndex;

        {
            char geneMenuChoice;
            bool exitMenu = false;

            do
            {
                std::cout << "\nEnter threshold for ratio: ";
                std::cin >> fRatioThreshold;

                std::cout << R"(
Available regions
    1. Africa   2. America  3. East Asia    4. Europe   5. South Asia

Multiple regions can be entered as a comma seperated list
Select region: )";
                {
                    std::string regionInput;
                    MultipleRegionEntry.clear();

                    {
                        exitMenu = true;
                        const int maxInputSize = 100;
                        char *tempInput = new char[maxInputSize];
                        std::cin.ignore();
                        std::cin.getline(tempInput, maxInputSize);
                        size_t i;

                        for (i = 0; '\0' != tempInput[i]; ++i)
                        {
                            if (' ' != tempInput[i])
                            {
                                regionInput += tempInput[i];
                                exitMenu = false;
                            }
                        }
                        delete[] tempInput;
                    }

                    if (regionInput.length() > 0)
                    {
                        std::string sCSV;
                        std::stringstream tempInputBuffer(regionInput);
                        std::string sLine;

                        getline(tempInputBuffer, sLine, '\n');
                        std::stringstream tempLineBuffer(sLine);

                        while (getline(tempLineBuffer, sCSV, ','))
                        {
                            int selectedRegion;

                            try
                            {
                                selectedRegion = stoi(sCSV);
                            }
                            catch (const std::exception &e)
                            {
                                std::cerr << FONT_RED_BOLD_INVERSE << "\n\tInvalid input\n"
                                          << FONT_RESET;
                            }

                            std::pair<PopulationRegion, std::string> pairRegion = getRegion(selectedRegion, -1);
                            MultipleRegionEntry.insert(pairRegion);
                        }
                        if (ImportPopulationData(vPopData, sPopFilePath))
                        {
                            for (auto iterator : MultipleRegionEntry)
                            {
                                std::clog << FONT_GREEN_BOLD << "\n\t\t" << iterator.second << FONT_RESET;
                                std::string sRatio = std::to_string((int)(fRatioThreshold * 10'000));

                                std::string sOutputPath = pResources + sGeneName + "/modified" + iterator.second + "__0." + sRatio + ".txt";

                                ChangeFileOptions *options = new ChangeFileOptions(iterator.first, fRatioThreshold, startOffset, endIndex);

                                if (DeriveChangeFile(vPopData, vAnnotated, *options))
                                {
                                    ModifyGene(vAnnotated, sOriginalGeneFile, sOutputPath, startOffset);
                                }

                                vAnnotated.clear();
                                delete options;
                            }
                        }
                        else
                        {
                            std::clog << FONT_RED_BOLD_INVERSE + "\n\t\tCould not process population data" + FONT_RESET + '\n';
                            std::clog << R"(
            Probables causes:
                1. File does not exist
                2. Incorrect file name format
                        <GENE>-POP.txt
                                )";
                        }
                    }
                    else
                    {
                        exit(0);
                        break;
                    }
                }

                {
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
                }
            } while (false == exitMenu);
        }
    }
} // namespace fs=std::experimental::filesystemintmain()