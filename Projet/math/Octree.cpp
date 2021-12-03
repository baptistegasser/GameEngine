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

Octree::Octree(const BoundingBox& Boundary)
	: Root{ Boundary }
{}

bool Octree::Add(ActorType Actor)
{
    // Prepare leaf
    Leaf Leaf{ Actors.size(), Actor->Transform.Position };

    // Ignore out of bound for this tree
    if (!VolumeContains(Root.Boundary, Leaf.Position))
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
    const Leaf Leaf{static_cast<Leaf::ID>(it - Actors.begin()), Actor->Transform.Position };

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
        	if (CurLeaf.Position != Actors[CurLeaf.ActorID]->Transform.Position) {
                // Update position, store to add back and remove from current node
                CurLeaf.Position = Actors[CurLeaf.ActorID]->Transform.Position;
                LeafToUpdate.push_back(CurLeaf);
                It = Node->Leafs.erase(It);
            }
            else {
                ++It;
            }
        }
    }

    // Update leafs and handle out of bound as unrecoverable error
    for (const Leaf& L : LeafToUpdate) {
        if (!Root.Add(L)) {
            throw "Out of bound object !";
        }
    }
}

const Octree::ActorList& Octree::GetActors() const noexcept
{
    return Actors;
}

Octree::ActorPtrList Octree::Find(const Math::Vec3f& Pos, float MaxDistance) const
{
	return Find(BoundingSphere{ MaxDistance, Pos });
}

Octree::ActorPtrList Octree::Find(const BoundingVolume Volume) const
{
    using NodePtr = const Node*;

    ActorPtrList ActorsFound{};

    std::deque<NodePtr> NodeToExplore;
    NodeToExplore.push_back(&Root);

    while (!NodeToExplore.empty()) {
        NodePtr Node = NodeToExplore.front();
        NodeToExplore.pop_front();

        // Ignore non intersected nodes
        if (!VolumesIntersect(Node->Boundary, Volume))
            continue;

        // This node is subdivided add node to queue
        if (Node->Subdivided) {
            std::transform(begin(Node->Children),
                end(Node->Children),
                std::back_inserter(NodeToExplore),
                [](const auto& uptr) { return uptr.get(); });
            continue;
        }

        // Add matching actors to the list
        for (const Leaf& Leaf : Node->Leafs) {
	        if (VolumeContains(Volume, Leaf.Position)) {
                ActorsFound.push_back(Actors[Leaf.ActorID].get());
	        }
        }
    }

    return ActorsFound;
}
