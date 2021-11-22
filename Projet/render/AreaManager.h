#pragma once
#include <vector>
#include <set>
#include <string>
#include "../core/Actor.h"
#include "util/Singleton.h"

/// <summary>
/// Represent the differents areas for the render of the scene
/// </summary>
class AreaManager : public Singleton<AreaManager> {

    friend struct Singleton<AreaManager>;

public :
    using ActorPtr = std::shared_ptr<Pitbull::Actor>;

    /// <summary>
    /// Initialize the grid
    /// </summary>
    /// <param name="WidthMap"> total width of the grid </param>
    /// <param name="HeightMap"> total height of the grid </param>
    void Init(int WidthMap, int HeightMap);

    /// <summary>
    /// Place the actor in the grid
    /// </summary>
    /// <param name="Actor"> the actor to be placed </param>
    void PlaceActor(ActorPtr& Actor);

    /// <summary>
    ///  Store the actor camera 
    /// </summary>
    /// <param name="Actor"> the actor Camera </param>
    void PlaceCamera(ActorPtr& Actor);

    /// <summary>
    /// Change the place of the actor in the grid
    /// </summary>
    /// <param name="Actor"> the actor to be moved </param>
    void MoveActor(ActorPtr& Actor);

    /// <summary>
    /// Get the actors who are close to the Camera
    /// </summary>
    /// <returns> the set of the actors to render </returns>
    std::set<ActorPtr> GetActors();

private :

    int Width;
    int Height;
    const int Size = 10;

    std::vector<std::vector<std::set<ActorPtr>>> Grid;
    ActorPtr Camera;

    AreaManager() {};
    ~AreaManager() noexcept {};
        
};