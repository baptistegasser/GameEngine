#include "stdafx.h"
#include "Octree.h"

#include <algorithm>

Node::Node(const BoundingBox& Boundary)
    : Subdivided{ false }
    , Boundary{ Boundary }
{}

bool Node::Add(const Leaf& Leaf)
{
    // Ignore out of bound for this node
    if (!Boundary.ContainPoint(Leaf.Position))
        return false;

    // This node is not subdivided and still have available leafs
    if (!Subdivided && Leafs.size() < MAX_ACTORS_COUNT) {
        Leafs.push_back(Leaf);
        return true;
    }

    // This is full but not subdivided yet
    if (!Subdivided) {
        Subdivide();
    }

    return AddToChildren(Leaf);
}

bool Node::Remove(const Leaf& Leaf)
{
    // Ignore out of bound for this node
    if (!Boundary.ContainPoint(Leaf.Position))
        return false;

    // Don't check our leaves if not needed
    if (Subdivided) {
        return RemoveFromChildren(Leaf);
    }

    auto it = std::find(begin(Leafs), end(Leafs), Leaf);

    if (it != end(Leafs)) {
        Leafs.erase(it);
        return true;
    }
    else {
        return false;
    }
}

void Node::Subdivide()
{
    // Combinaisons of coordinate directions scale
    const static float* Scales = new float[24]{
        1.f, 1.f, 1.f,
        1.f, 1.f, -1.f,
        1.f, -1.f, 1.f,
        1.f, -1.f, -1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f
    };

    const float HW = Boundary.HalfWidth / 2.f,
        HH = Boundary.HalfHeight / 2.f,
        HD = Boundary.HalfDepth / 2.f;

    // Create childrens
    for (int i = 0, j = 0; i < 8; ++i) {
        Point ChildCenter = Boundary.Center;
        ChildCenter.x += HW * Scales[j++];
        ChildCenter.y += HH * Scales[j++];
        ChildCenter.z += HD * Scales[j++];
        Children.push_back(std::make_unique<Node>(BoundingBox{ HW, HH, HD, ChildCenter }));
    }

    // Move leafs to childs
    for (const auto& Leaf : Leafs) {
        AddToChildren(Leaf);
    }

    // Empty container as we won't store leafs anymore
    Leafs.clear();

    Subdivided = true;
}

bool Node::AddToChildren(const Leaf& Leaf)
{
    for (const auto& Child : Children) {
        if (Child->Add(Leaf))
            return true;
    }

    return false; // Something is wrong I can feel it
}

bool Node::RemoveFromChildren(const Leaf& Leaf)
{
    for (const auto& Child : Children) {
        if (Child->Remove(Leaf))
            return true;
    }
    
    return false;
}


bool SimpleOctree::Add(const ActorType& Actor)
{
    // Prepare leaf
    Leaf Leaf{ Actors.size(), Actor->Transform };

    // Ignore out of bound for this tree
    if (!Root.Boundary.ContainPoint(Leaf.Position))
        return false;

    // Ignore if already have this Actor in the tree
    {
        const auto it = std::find(begin(Actors), end(Actors), Actor);
        if (it != Actors.end())
            return false;
    }

    // Insert the actor in the array
    Actors.push_back(Actor);

    // Insert in the tree
    return Root.Add(Leaf);
}

bool SimpleOctree::Remove(const ActorType& Actor)
{
    // Ignore if not in tree
    const auto it = std::find(begin(Actors), end(Actors), Actor);
    if (it == Actors.end())
        return true;

    // Create matching leaf
    const Leaf Leaf{ it - Actors.begin(), Actor->Transform };

    // Remove from actors
    Actors.erase(it);

    // Remove from tree
    Root.Remove(Leaf);

    return true;
}

SimpleOctree::ActorList SimpleOctree::Find(const BoundingVolume* Volume)
{
    return ActorList();
}
