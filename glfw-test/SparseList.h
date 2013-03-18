#pragma once

template <class T>
class CSparseList
{
public:
	CSparseList(UINT32 capacity);
	~CSparseList(void);
	UINT32 Size() const;
	UINT32 Capacity() const;
	T* Get(UINT32 slot);
	T* Set(UINT32 slot, T* data);
	UINT32 Resize(size_t capacity);
	UINT32 Push(T* data);
	T* Erase(UINT32 slot);
	void foreach(bool (*callback)(T* ));

	

private:
	T** m_data;
	UINT32* m_free_slots;
	UINT32 m_size;
	UINT32 m_capacity;
};
