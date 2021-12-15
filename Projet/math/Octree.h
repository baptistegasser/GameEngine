#pragma once

#include "math/BoundingVolume.h"

#include <vector>
#include <deque>
#include <memory>

constexpr unsigned int MAX_LEAFS_COUNT = 6;

struct Leaf {
	using ID = std::size_t;

	ID DataID;
	Math::Vec3f Position;

	bool operator==(const Leaf& lhr) const noexcept { return DataID == lhr.DataID; };
};

struct Node {
	using ChildPtr = std::unique_ptr<Node>;

	Node(const BoundingBox& Boundary);

	bool Add(const Leaf& Leaf);
	bool Remove(const Leaf& Leaf);
	void Subdivide();

	bool Subdivided;
	BoundingBox Boundary;
	std::vector<Leaf> Leafs;
	std::vector<ChildPtr> Children;

private:
	bool AddToChildren(const Leaf& Leaf);
	bool RemoveFromChildren(const Leaf& Leaf);
};

template <class T, class ToPos, bool Smart = true>
class Octree {
public:
	using DataPtr = T*;
	using DataType = std::conditional_t<Smart, std::unique_ptr<T>, DataPtr>;
	using DataList = std::vector<DataType>;
	using DataPtrList = std::vector<DataPtr>;

	Octree(const BoundingBox& Boundary);

	static DataPtr GetInstance(const DataType& Data);

	bool Add(DataPtr Data);
	bool Remove(const DataPtr Data);
	void Update();

	const DataList& GetDatas() const noexcept;
	DataPtrList Find(const Math::Vec3f& Pos, float MaxDistance) const;
	DataPtrList Find(const BoundingVolume Volume) const;

protected:
	Node Root;
	DataList Datas;
	ToPos ToPos;
};

template <class T, class ToPos, bool Smart>
Octree<T, ToPos, Smart>::Octree(const BoundingBox& Boundary)
	: Root{ Boundary }
{}

template <class T, class ToPos, bool Smart>
typename Octree<T, ToPos, Smart>::DataPtr Octree<T, ToPos, Smart>::GetInstance(const DataType& Data)
{
	if constexpr (Smart) return Data.get();
	else return Data;
}

template <class T, class ToPos, bool Smart>
bool Octree<T, ToPos, Smart>::Add(DataPtr Data)
{
	// Prepare leaf
	Leaf Leaf{ Datas.size(), ToPos(Data) };

	// Ignore out of bound for this tree
	if (!VolumeContains(Root.Boundary, Leaf.Position))
		return false;

	// Ignore if already have this Data in the tree
	const auto PtrMatcher = [&Data](const DataType& uptr) { return GetInstance(uptr) == Data; };
	const auto it = std::find_if(begin(Datas), end(Datas), PtrMatcher);
	if (it != Datas.end())
		return false;

	// Insert the data in the array
	if constexpr (Smart) Datas.push_back(std::unique_ptr<T>(Data));
	else Datas.push_back(Data);

	// Insert in the tree
	return Root.Add(Leaf);
}

template <class T, class ToPos, bool Smart>
bool Octree<T, ToPos, Smart>::Remove(const DataPtr Data)
{
	// Ignore if not in tree
	const auto PtrMatcher = [&Data](const DataType& uptr) { return GetInstance(uptr) == Data; };
	const auto it = std::find_if(begin(Datas), end(Datas), PtrMatcher);
	if (it == Datas.end())
		return true;

	// Create matching leaf
	const Leaf Leaf{ static_cast<Leaf::ID>(it - Datas.begin()), Data(Data) };

	// Remove from Datas
	Datas.erase(it);

	// Remove from tree
	Root.Remove(Leaf);

	return true;
}

template <class T, class ToPos, bool Smart>
void Octree<T, ToPos, Smart>::Update()
{
	using NodePtr = Node*;

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
			if (CurLeaf.Position != ToPos(GetInstance(Datas[CurLeaf.DataID]))) {
				// Update position, store to add back and remove from current node
				CurLeaf.Position = ToPos(GetInstance(Datas[CurLeaf.DataID]));
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

template <class T, class ToPos, bool Smart>
const typename Octree<T, ToPos, Smart>::DataList& Octree<T, ToPos, Smart>::GetDatas() const noexcept
{
	return Datas;
}

template <class T, class ToPos, bool Smart>
typename Octree<T, ToPos, Smart>::DataPtrList Octree<T, ToPos, Smart>::Find(const Math::Vec3f& Pos, float MaxDistance) const
{
	return Find(BoundingSphere{ MaxDistance, Pos });
}

template <class T, class ToPos, bool Smart>
typename Octree<T, ToPos, Smart>::DataPtrList Octree<T, ToPos, Smart>::Find(const BoundingVolume Volume) const
{
	using NodePtr = const Node*;

	DataPtrList DatasFound{};

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

		// Add matching data to the list
		for (const Leaf& Leaf : Node->Leafs) {
			if (VolumeContains(Volume, Leaf.Position)) {
				DatasFound.push_back(GetInstance(Datas[Leaf.DataID]));
			}
		}
	}

	return DatasFound;
}