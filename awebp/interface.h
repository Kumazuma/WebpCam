#pragma once
#include<wx/wx.h>
#include <optional>
struct IImageStoreBuilder;
//캡처 이미지 데이터를 임시로 보관하는 인터페이스
struct IImageStore
{
	virtual ~IImageStore() {}
	virtual std::pair<wxImage, uint32_t> Get(size_t index)  = 0;
	virtual std::pair<wxInputStream*, uint32_t> GetRawData(size_t index) =0;
	virtual size_t GetCount() const = 0;
	virtual wxSize GetImageSize() const = 0;
	virtual void Clear() = 0;
	virtual void SetFrameDuration(size_t index, uint32_t duration) = 0;
//부가적인 기능
	virtual bool IsSupportedEdit() { return false; }
	virtual IImageStore* RemoveImages(size_t from, size_t to) { return nullptr; }
	virtual bool InsertImages(IImageStore*& store, size_t to) { return false; }
	virtual std::optional<uint32_t> GetFrameDuration(size_t index) {return std::optional<uint32_t>();}

//해당 이미지 저장소와 동일한 빌더를 생성한다.
	virtual IImageStoreBuilder* CreateBuilder(const wxSize& imageSize) = 0;

};
//
struct IImageStoreBuilder
{
	virtual ~IImageStoreBuilder() {};
	virtual void PushBack(const wxImage& image, uint32_t duration) = 0;
	virtual size_t GetSize() const = 0;
	//IImageStore를 반환하고 Builder를 해제한다.
	static IImageStore* BuildStore(IImageStoreBuilder*& builder)
	{
		auto* temp = builder;
		builder = nullptr;
		auto store = temp->BuildStore();
		delete temp;
		return store;
	}
protected:
	virtual IImageStore* BuildStore() = 0;
	
};
//이미지를 애니메이션 이미지로 만드는 인터페이스
//TODO: webp 이외의 이미지 포맷을 지원해보자.
struct IEncoder
{
	virtual void Encode(wxEvtHandler * handler, const wxString filePath, IImageStore& imageStore) = 0;
	virtual wxString GetFileFilter() =0;
	virtual wxString GetFileExtension() = 0;
	virtual void StopEncode() = 0;
	virtual void SetQuality(int) = 0;
	virtual ~IEncoder() {}
};
//화면을 캡쳐하는 인터페이스
//TODO: DirectX를 이용한 캡처 클래스를 만든다.
struct ICapturer
{
	virtual void BeginCapture(wxEvtHandler* handler, const wxRect& rect, uint32_t duration) = 0;
	virtual wxImage CaptureFrame() = 0;
	virtual void EndCapture() = 0;
	virtual ~ICapturer() {}
};

//편집 히스토리 내역을 위한 인터페이스.
struct IHistoryItem
{
	virtual ~IHistoryItem() {}
	virtual void Undo(IImageStore*& imageStore) = 0;
	virtual void Redo(IImageStore*& imageStore) = 0;
	virtual wxString GetDescription() const  = 0;
};
//편집 기능 인터페이스
struct IEditTool
{
	virtual ~IEditTool() {}
	virtual void Execute(IImageStore*& OUT imageStore, size_t start, size_t end, IHistoryItem*& OUT historyItem ) = 0;
};
//
template<typename T, typename FREE_FUNCTION>
class Wrapper
{
private:
	T* m_ptr;
	FREE_FUNCTION m_freeFunction;
public:
	Wrapper(const Wrapper& obj) = delete;
	Wrapper(T* ptr, FREE_FUNCTION free);
	Wrapper(Wrapper&& move);
	T* operator ->() const
	{
		return m_ptr;
	}
	T* get() const
	{
		return m_ptr;
	}
	~Wrapper()
	{
		if(m_ptr)
			m_freeFunction(m_ptr);
	}
};
template<typename T, typename FREE_FUNCTION>
Wrapper<T, FREE_FUNCTION> GetWrapper(T* obj, FREE_FUNCTION func)
{
	return Wrapper<T, FREE_FUNCTION>(obj, func);
}

template<typename T, typename FREE_FUNCTION>
inline Wrapper<T, FREE_FUNCTION>::Wrapper(T* ptr, FREE_FUNCTION free):
	m_ptr(ptr),
	m_freeFunction(free)
{
	
}

template<typename T, typename FREE_FUNCTION>
inline Wrapper<T, FREE_FUNCTION>::Wrapper(Wrapper&& move)
{
	m_ptr = move.m_ptr;
	m_freeFunction = move.m_freeFunction;
}
