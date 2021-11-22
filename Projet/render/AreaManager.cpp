#include "stdafx.h"
#include "AreaManager.h"

#include <cmath>

using ActorPtr = std::shared_ptr<Pitbull::Actor>;

using namespace std;
void AreaManager::Init(int WidthMap, int HeightMap)
{
    Width = 1 + (int)floor((float)WidthMap / (float)Size);
    Height = 1 + (int)floor((float)HeightMap / (float)Size);

    Grid = vector(Width, vector(Height, set<ActorPtr>()));
}

void AreaManager::PlaceActor(ActorPtr &Actor)
{
    int X = (int) floor(Actor->Transform.p.x / Size);
    int Y = (int) floor(Actor->Transform.p.y / Size);

    Actor->GridX = X;
    Actor->GridY = Y;

    Grid[X][Y].insert(Actor);
}

void AreaManager::PlaceCamera(ActorPtr& Actor)
{
    Camera = Actor;
}

void AreaManager::MoveActor(ActorPtr &Actor)
{
    int X = (int) floor(Actor->Transform.p.x / Size);
    int Y = (int) floor(Actor->Transform.p.y / Size);

    if ((Actor->GridX == X) && (Actor->GridY == Y))
        return;

    Grid[Actor->GridX][Actor->GridY].erase(Actor);

    Actor->GridX = X;
    Actor->GridY = Y;

    Grid[X][Y].insert(Actor);

}

std::set<ActorPtr> AreaManager::GetActors()
{
    int XArea = (int)floor(Camera->Transform.p.x / Size);
    int YArea = (int)floor(Camera->Transform.p.y / Size);

    return  Grid[XArea][YArea];
}
