#include "stdafx.h"
#include "AreaManager.h"

#include <cmath>

using namespace std;
AreaManager::AreaManager(int WidthMap, int HeightMap)
{
    Width = 1 + (int) floor((float) WidthMap / (float) Size);
    Height = 1 + (int) floor((float) HeightMap / (float) Size);

    Grid = vector(Width, vector(Height, set<string>()));
}

void AreaManager::PlaceActor(Pitbull::Actor &Actor)
{
    int X = (int) floor(Actor.Transform.p.x / Size);
    int Y = (int) floor(Actor.Transform.p.y / Size);

    Actor.GridX = X;
    Actor.GridY = Y;

    Grid[X][Y].insert(Actor.Name);
}

void AreaManager::MoveActor(Pitbull::Actor &Actor)
{
    int X = (int) floor(Actor.Transform.p.x / Size);
    int Y = (int) floor(Actor.Transform.p.y / Size);

    if ((Actor.GridX == X) && (Actor.GridY == Y))
        return;

    Grid[Actor.GridX][Actor.GridY].erase(Actor.Name);

    Actor.GridX = X;
    Actor.GridY = Y;

    Grid[X][Y].insert(Actor.Name);

}

std::set<std::string> AreaManager::GetActors(float X, float Y)
{
    int XArea = (int)floor(X / Size);
    int YArea = (int)floor(Y / Size);

    return  Grid[XArea][YArea];
}
