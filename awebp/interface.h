#pragma once
#include<wx/wx.h>
struct IImageStoreBuilder;
//캡처 이미지 데이터를 임시로 보관하는 --인터페이스-- 추상클래스
struct IImageStore
{
	virtual ~IImageStore() {}
	virtual  std::pair<wxImage, uint32_t> Get(size_t index)  = 0;
	virtual size_t GetSize() const = 0;
	virtual void Clear() = 0;
//해당 이미지 저장소와 동일한 빌더를 생성한다.
	virtual IImageStoreBuilder* CreateBuilder() = 0;
	size_t size()
	{
		return GetSize();
	}
	const std::pair<wxImage, uint32_t> operator[] (size_t index) 
	{
		return Get(index);
	}
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
wxDECLARE_EVENT(EVT_FINISH_ENCODE, wxCommandEvent);//인코딩을 완료했을 때
wxDECLARE_EVENT(EVT_FAILED_ENCODE, wxCommandEvent);//인코딩을 실패했을 때
wxDECLARE_EVENT(EVT_ADDED_A_FRAME, wxCommandEvent);//프레임 하나를 처리 완료 했을 때
//TODO: webp 이외의 이미지 포맷을 지원해보자.
struct IEncoder
{
	virtual void Encode(wxEvtHandler * handler, const wxString filePath, IImageStore& imageStore) = 0;
	virtual wxString GetFileFilter() =0;
	virtual wxString GetFileExtension() = 0;
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