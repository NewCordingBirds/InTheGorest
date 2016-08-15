
#pragma once
#include <Windows.h>
#include <cassert>

template <class T, int ALLOC_BLOCK_SIZE = 50>

class CMemoryPool{
public:
	static void* operator new(std::size_t allocLength){
		assert(sizeof(T) == allocLength);				//assert : ()안의 조건을 만족해야 통과)
		assert(sizeof(T) >= sizeof(UCHAR*));			//만족 못할 시 오류		-> 디버깅용이므로 릴리즈에서 사용가능한 예외처리로 바꿀것 요망

		if (!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}
		static void operator delete(void* deletePointer){
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		mFreePointer = static_cast<UCHAR*>(deletePointer);
	}
private:
	static void allocBlock(){
		mFreePointer = new UCHAR[sizeof(T)* ALLOC_BLOCK_SIZE];
		UCHAR **Current = reinterpret_cast<UCHAR**>(mFreePointer);
		UCHAR * Next = mFreePointer;

		for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; i++){
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = 0;
	}
	static UCHAR *mFreePointer;

protected:
	~CMemoryPool(){

	}
};

template < class T, int ALLOC_BLOCK_SIZE >
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;