#include "stdafx.h"
#include "Octree.h"

#include <algorithm>
#include <functional>
#include <queue>

Node::Node(const BoundingBox& Boundary)
    : Subdivided{ false }
    , Boundary{ Boundary }
{}

bool Node::Add(const Leaf& Leaf)
{
    // Ignore out of bound for this node
    if (!VolumeContains(Boundary, Leaf.Position))
        return false;

    // This node is not subdivided and still have available leafs
    if (!Subdivided && Leafs.size() < MAX_LEAFS_COUNT) {
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
    if (!VolumeContains(Boundary, Leaf.Position))
        return false;

    // Don't check our leaves if not needed
    if (Subdivided) {
        return RemoveFromChildren(Leaf);
    }

    const auto it = std::find(begin(Leafs), end(Leafs), Leaf);

    if (it != end(Leafs)) {
        Leafs.erase(it);
        return true;
    }

    return false;
}

void Node::Subdivide()
{
    // Combinaisons of coordinate directions scale
    const static float* Pos = new float[24]{
         1.f,  1.f,  1.f,
         1.f,  1.f, -1.f,
         1.f, -1.f,  1.f,
         1.f, -1.f, -1.f,
        -1.f,  1.f,  1.f,
        -1.f,  1.f, -1.f,
        -1.f, -1.f,  1.f,
        -1.f, -1.f, -1.f
    };

    const float HW = Boundary.HalfWidth / 2.f,
        HH = Boundary.HalfHeight / 2.f,
        HD = Boundary.HalfDepth / 2.f;

    // Reserve size to be sure to use just enought memory
    Children.reserve(8);

    // Create childrens
    for (int i = 0, j = 0; i < 8; ++i) {
        Math::Vec3f ChildCenter = Boundary.Center;
        ChildCenter.x += HW * Pos[j++];
        ChildCenter.y += HH * Pos[j++];
        ChildCenter.z += HD * Pos[j++];
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
    const auto AddCallback = [&Leaf](const ChildPtr& Child) { return Child->Add(Leaf); };
    return std::any_of(Children.begin(), Children.end(), AddCallback);
}

bool Node::RemoveFromChildren(const Leaf& Leaf)
{
    const auto RemoveCallback = [&Leaf](const ChildPtr& Child) { return Child->Remove(Leaf); };
    return std::any_of(Children.begin(), Children.end(), RemoveCallback);
}
