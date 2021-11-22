#pragma once
#include <vector>
#include <set>
#include <string>
#include "../core/Actor.h"

class AreaManager {

public :
    int Width;
    int Height;
    const int Size = 10;

    std::vector<std::vector<std::set<std::string>>> Grid;

    AreaManager(int WidthMap, int HeightMap);

    void PlaceActor(Pitbull::Actor &Actor);

    void MoveActor(Pitbull::Actor &Actor);

    std::set<std::string> GetActors(float X, float Y);


        
};