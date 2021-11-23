#include "stdafx.h"
#include "Octree.h"

#include "math/BoundingSphere.h"

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
    const auto AddCallback = [&Leaf](const ChildPtr& Child) { return Child->Add(Leaf); };
    return std::any_of(Children.begin(), Children.end(), AddCallback);
}

bool Node::RemoveFromChildren(const Leaf& Leaf)
{
    const auto RemoveCallback = [&Leaf](const ChildPtr& Child) { return Child->Remove(Leaf); };
    return std::any_of(Children.begin(), Children.end(), RemoveCallback);
}

Octree::Octree(const BoundingBox& Boundary)
	: Root{ Boundary }
{}

bool Octree::Add(ActorType Actor)
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
    Actors.push_back(std::move(Actor));

    // Insert in the tree
    return Root.Add(Leaf);
}

bool Octree::Remove(const ActorPtr Actor)
{
    // Ignore if not in tree
    const auto PtrMatcher = [&Actor](const ActorType& uptr) { return uptr.get() == Actor; };
    const auto it = std::find_if(begin(Actors), end(Actors), PtrMatcher);
    if (it == Actors.end())
        return true;

    // Create matching leaf
    const Leaf Leaf{static_cast<Leaf::ID>(it - Actors.begin()), Actor->Transform };

    // Remove from actors
    Actors.erase(it);

    // Remove from tree
    Root.Remove(Leaf);

    return true;
}

void Octree::Update()
{
    using NodePtr = Node *;

    std::vector<Leaf> LeafToUpdate;

    std::deque<NodePtr> NodeToExplore;
    NodeToExplore.push_back(&Root);

    while (!NodeToExplore.empty()) {
        const NodePtr Node = NodeToExplore.front();
        NodeToExplore.pop_front();

        // Add node to explore
        if (Node->Subdivided) {
            std::transform(begin(Node->Children),
                end(Node->Children),
                std::back_inserter(NodeToExplore),
                [](const auto& uptr) { return uptr.get(); });
            continue;
        }

        // Iterate on the leafs
        auto It = Node->Leafs.begin();
        for (; It != Node->Leafs.end();) {
            Leaf CurLeaf = *It;
            // The leaf is now invalid
        	if (CurLeaf.Position != Actors[CurLeaf.ActorID]->Transform) {
                // Update position, store to add back and remove from current node
                CurLeaf.Position = Actors[CurLeaf.ActorID]->Transform;
                LeafToUpdate.push_back(CurLeaf);
                It = Node->Leafs.erase(It);
            }
            else {
                ++It;
            }
        }
    }

    // Add Leafs back to their correct places
    const auto AddToRoot = std::bind(&Node::Add, &Root, std::placeholders::_1);
    std::for_each(begin(LeafToUpdate), end(LeafToUpdate), AddToRoot);
}

const Octree::ActorList& Octree::GetActors() const noexcept
{
    return Actors;
}

Octree::ActorPtrList Octree::Find(const Point& Pos, float MaxDistance)
{
	const BoundingSphere Sphere{ MaxDistance, Pos };
    return Find(&Sphere);
}

Octree::ActorPtrList Octree::Find(const BoundingVolume* Volume)
{
    return ActorPtrList();
}
