#include "stdafx.h"
#include "AreaManager.h"

#include <cmath>

using ActorPtr = std::shared_ptr<Pitbull::Actor>;

using namespace std;
void AreaManager::Init(int WidthMap, int HeightMap)
{
    if (WidthMap % Size == 0) {
        Width = WidthMap / Size;
    }
    else {
        Width = 1 + (int)floor((float)WidthMap / (float)Size);
    }

    if (HeightMap % Size == 0) {
        Height = HeightMap / Size;
    }
    else {
        Height = 1 + (int)floor((float)HeightMap / (float)Size);
    }

    

    Grid = vector(Width*2, vector(Height*2, set<ActorPtr>()));
}

void AreaManager::PlaceActor(ActorPtr &Actor)
{
    float X = Actor->Transform.p.x / Size;
    float Y = Actor->Transform.p.z / Size;

    int XGrid;
    int YGrid;

    if (X < 0) {
        XGrid = Width + (int) ceil(X);
    }
    else {
        XGrid = Width + (int) floor(X);
    }

    if (Y < 0) {
        YGrid = Width + (int)ceil(Y);
    }
    else {
        YGrid = Width + (int)floor(Y);
    }


    Actor->GridX = XGrid;
    Actor->GridY = YGrid;

    Grid[XGrid][YGrid].insert(Actor);
}

void AreaManager::PlaceCamera(ActorPtr& Actor)
{
    Camera = Actor;
}

void AreaManager::MoveActor(ActorPtr &Actor)
{
    float X = Actor->Transform.p.x / Size;
    float Y = Actor->Transform.p.z / Size;

    int XGrid;
    int YGrid;

    if (X < 0) {
        XGrid = Width + (int)ceil(X);
    }
    else {
        XGrid = Width + (int)floor(X);
    }

    if (Y < 0) {
        YGrid = Width + (int)ceil(Y);
    }
    else {
        YGrid = Width + (int)floor(Y);
    }

    if ((Actor->GridX == XGrid) && (Actor->GridY == YGrid))
        return;

    Grid[Actor->GridX][Actor->GridY].erase(Actor);

    Actor->GridX = XGrid;
    Actor->GridY = YGrid;

    Grid[XGrid][YGrid].insert(Actor);

}

std::set<ActorPtr> AreaManager::GetActors()
{

    float X = Camera->Transform.p.x / Size;
    float Y = Camera->Transform.p.z / Size;

    int XGrid;
    int YGrid;

    if (X < 0) {
        XGrid = Width + (int)ceil(X);
    }
    else {
        XGrid = Width + (int)floor(X);
    }

    if (Y < 0) {
        YGrid = Width + (int)ceil(Y);
    }
    else {
        YGrid = Width + (int)floor(Y);
    }

    return  Grid[XGrid][YGrid];
}
