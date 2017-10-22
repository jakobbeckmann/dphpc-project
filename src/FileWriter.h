/**
 * This FileWriter is designed to print the single intermediate steps from the grahamScan
 * to a file where the information about left/right
 */

#ifndef DPHPC_PROJECT_FILEWRITER_H
#define DPHPC_PROJECT_FILEWRITER_H

#include <string>
#include <fstream>

#include "Point.h"


class FileWriter {

public:
    void appendPointToFile(Point& point, int direction) {
        std::ofstream pointsFile;
        pointsFile.open(fileName, std::ios_base::app);
        pointsFile << point.x << "," << point.y << "," << direction << std::endl;
        pointsFile.close();
    }

    void updateFileName() {
        fileName = baseName + "_" + std::to_string(grahamSubsetIdx) + ".dat";
        std::cout << "Writing graham scan subset " << grahamSubsetIdx << " to new file: " << fileName << std::endl;

    }

    void setBaseName(const char* name) {
        baseName = name;
        updateFileName();
    }

    void setGrahamSubsetIdx(int& idx) {
        grahamSubsetIdx = idx;
        updateFileName();
    }

private:
    int grahamSubsetIdx = 0;
    std::string baseName = "";
    std::string fileName = "";

};


#endif //DPHPC_PROJECT_FILEWRITER_H
