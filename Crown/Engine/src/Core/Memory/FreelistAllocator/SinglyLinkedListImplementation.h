#pragma once
#include "Core/Utilities/Defines.h"
namespace Crown
{
	template <class T>
	void SinglyLinkedList<T>::Insert(Node* a_PreviousNode, Node* a_NewNode)
	{
		CROWN_PROFILE_FUNCTION();
		if (a_PreviousNode == nullptr)
		{
			// Is the first node
			if (m_Head != nullptr)
			{
				// The list has more elements
				a_NewNode->m_Next = m_Head;
			}
			else
			{
				a_NewNode->m_Next = nullptr;
			}
			m_Head = a_NewNode;
		}
		else
		{
			if (a_PreviousNode->m_Next == nullptr)
			{
				// Is the last node
				a_PreviousNode->m_Next = a_NewNode;
				a_NewNode->m_Next = nullptr;
			}
			else
			{
				// Is a middle node
				a_NewNode->m_Next = a_PreviousNode->m_Next;
				a_PreviousNode->m_Next = a_NewNode;
			}
		}
	}

	template <class T>
	void SinglyLinkedList<T>::Remove(Node* a_PreviousNode, Node* a_DeleteNode)
	{
		CROWN_PROFILE_FUNCTION();
		if (a_PreviousNode == nullptr)
		{
			// Is the first node
			if (a_DeleteNode->m_Next == nullptr)
			{
				// List only has one element
				m_Head = nullptr;
			}
			else
			{
				// List has more elements
				m_Head = a_DeleteNode->m_Next;
			}
		}
		else
		{
			a_PreviousNode->m_Next = a_DeleteNode->m_Next;
		}
	}
}
