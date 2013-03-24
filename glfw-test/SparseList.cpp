#include "stdafx.h"
#include "SparseList.h"

template <class T>
CSparseList<T>::CSparseList(UINT32 capacity):m_size(0), m_capacity(capacity)
{
	m_data = (T**)malloc(sizeof(T*)*capacity);
	//m_data = new (T*)[capacity];
	for(UINT32 i = 0; i < capacity; i++)
	{
		m_data[i] = NULL;
	}
	m_free_slots = new UINT32[capacity];
	for(UINT32 i = 0; i < capacity; i++)
	{
		m_free_slots[i] = i;
	}
}

template <class T>
CSparseList<T>::~CSparseList(void)
{
}

template <class T>
UINT32 CSparseList<T>::Capacity() const
{
	return m_capacity;
}

template <class T>
UINT32 CSparseList<T>::Size() const 
{
	return m_size;
}

template <class T>
T* CSparseList<T>::Get(UINT32 slot)
{
	if(slot >= m_capacity)
		throw;

	return m_data[slot];
}

template <class T>
T* CSparseList<T>::Set(UINT32 slot, T* data)
{
	if(slot >= m_capacity)
		throw;
	
	T* previous = Get(slot);
	m_data[slot] = data;
	return previous;
}

template <class T>
UINT32 CSparseList<T>::Resize(UINT32 capacity)
{
	cout << "[SparseList]: resizing to "<<capacity<<endl;
	/*cout << "previous content: "<<endl<<"--------"<<endl;
	for(int i = 0; i < m_capacity; i++)
	{
		cout << m_data[i] << endl;
	}*/
	UINT32 previous_capacity = m_capacity;
	m_capacity = capacity;
	//m_data = (T**)realloc(m_data,m_capacity);
	T** m_data2 = (T**)malloc(sizeof(T*)*m_capacity);
	m_free_slots = (UINT32*)realloc(m_free_slots,m_capacity);
	if(m_capacity == NULL || m_free_slots == NULL)
		throw;

	for(UINT32 i = 0; i < previous_capacity; i++)
	{
		m_data2[i] = m_data[i];
	}

	//fill the new slots with NULL(if the list grew)
	for(UINT32 i = previous_capacity; i < m_capacity; i++)
	{
		m_data2[i] = NULL;
	}

	free(m_data);
	m_data = m_data2;

	//we have to now recompute the free slots buffer from scratch, just to be sure
	UINT32 free_slot_pos = 0;
	for(UINT32 i = 0; i < m_capacity; i++)
	{
		if(m_data[i] == NULL)
			m_free_slots[free_slot_pos++] = i;
	}

	/*cout << "new content: "<<endl<<"--------"<<endl;
	for(int i = 0; i < m_capacity; i++)
	{
		cout << m_data[i] << endl;
	}*/

	return previous_capacity;
}

template <class T>
UINT32 CSparseList<T>::Push(T* data)
{
	UINT32 free_slot = 0;
	if(m_size < m_capacity)
	{
		free_slot = m_free_slots[m_size];
	}
	else
	{
		free_slot = m_capacity;
		Resize((UINT32)(m_capacity*1.5));
	}
	m_data[free_slot] = data;
	m_size++;
	return free_slot;
}

template <class T>
T* CSparseList<T>::Erase(UINT32 slot)
{
	if(slot >= m_capacity)
		throw;

	m_free_slots[--m_size] = slot;

	T* ret = m_data[slot];
	m_data[slot] = NULL;
	return ret;
}

template <class T> 
void CSparseList<T>::foreach(bool (*callback)(T* ))
{
	for(UINT32 i = 0; i < m_capacity; i++)
	{
		if(m_data[i] != NULL)
		{
			if(callback(m_data[i]))
				break;
		}
	}
}

class CModel;
class IGameObject;
template class CSparseList<CModel>;
template class CSparseList<IGameObject>;