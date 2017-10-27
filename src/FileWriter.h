/**
 * This FileWriter is designed to print the single intermediate steps from the grahamScan
 * to a file where the information about left/right
 */

#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "Point.h"



class FileWriter {

public:
    void writeGrahamStep(int baseIdx, int lastIdx, int secLastIdx, int addedIdx, int removedIdx, int orientation) {
        using namespace std;
        ofstream file;
        file.open(fileName, std::ios_base::app);
        file << to_string(baseIdx) << "," << to_string(lastIdx) << "," << to_string(secLastIdx) << ","
             << to_string(addedIdx) << "," << to_string(removedIdx) << "," << to_string(orientation) << endl;
        file.close();
    }

    void writeGraham(Point last, Point secLast, Point base, int addRemove, Point addRemPoint, int orientation) {
        using namespace std;
        ofstream file;
        file.open(fileName, std::ios_base::app);
        file << last.x << "," << secLast.x << "," << base.x << ","
             << last.y << "," << secLast.y << "," << base.y << ","
             << addRemove << "," << addRemPoint.x << "," << addRemPoint.y << "," << orientation << endl;

        file.close();
    }

    void updateFileName() {
        fileName = baseName + "_" + std::to_string(grahamSubsetIdx) + ".dat";
    }

    void setBaseName(const char* name) {
        baseName = name;
        updateFileName();
    }

    void setGrahamSubsetIdx(int& idx) {
        grahamSubsetIdx = idx;
        updateFileName();

    }

    /**
     * Removes the file with current fileName
     */
    void cleanOutputFile() {
        if( remove( fileName.c_str() ) != 0 )
            std::cout << "Could NOT delete " << fileName.c_str() << std::endl;
        else
            std::cout << "Deleted " << fileName.c_str() << std::endl;

    }

    /**
     * Writes a vector of
     * @param points
     * @param fileName
     */
    static void writePointsToFile(std::vector<Point> points, const std::string& fileName, bool removeFirst) {
        if (removeFirst)
            remove(fileName.c_str());
        std::ofstream allPointsFile;
        allPointsFile.open(fileName);
        for (Point point : points) {
            allPointsFile << point.x << "," << point.y << std::endl;
        }
        allPointsFile.close();
    }

private:
    int grahamSubsetIdx = 0;
    std::string baseName = "";
    std::string fileName = "";

};