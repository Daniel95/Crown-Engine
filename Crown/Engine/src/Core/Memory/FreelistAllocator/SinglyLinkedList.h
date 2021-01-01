#pragma once

namespace Crown
{
	template <class T>
	class SinglyLinkedList
	{
	public:
		struct Node
		{
			T m_Data{};
			Node* m_Next{};
		};
		Node* m_Head{};

		SinglyLinkedList() = default;

		void Insert(Node* a_PreviousNode, Node* a_NewNode);
		void Remove(Node* a_PreviousNode, Node* a_DeleteNode);
	};
}
#include "Core/Memory/FreelistAllocator/SinglyLinkedListImplementation.h"
