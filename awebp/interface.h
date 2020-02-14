#pragma once
#include<wx/wx.h>
wxDECLARE_EVENT(EVT_CompleteRecord, wxCommandEvent);
//캡처 이미지 데이터를 임시로 보관하는 --인터페이스-- 추상클래스
struct IImageStore
{
	virtual ~IImageStore() {}
	virtual size_t Add(const wxImage& image, uint32_t duration) = 0;
	virtual const std::pair<wxImage, uint32_t> Get(size_t index) const = 0;
	virtual size_t GetSize() const = 0;
	virtual void Clear() = 0;
	size_t size()
	{
		return GetSize();
	}
	const std::pair<wxImage, uint32_t> operator[] (size_t index) const
	{
		return Get(index);
	}
};
//이미지를 애니메이션 이미지로 만드는 인터페이스
wxDECLARE_EVENT(EVT_FINISH_ENCODE, wxCommandEvent);//인코딩을 완료했을 때
wxDECLARE_EVENT(EVT_FAILED_ENCODE, wxCommandEvent);//인코딩을 실패했을 때
wxDECLARE_EVENT(EVT_ADDED_A_FRAME, wxCommandEvent);//프레임 하나를 처리 완료 했을 때
//TODO: webp 이외의 이미지 포맷을 지원해보자.
struct IEncoder
{
	virtual void Encode(wxEvtHandler * handler, const wxString filePath, const IImageStore& imageStore) = 0;
	virtual wxString GetFileFilter() =0;
	virtual wxString GetFileExtension() = 0;
	virtual ~IEncoder() {}
};
//화면을 캡쳐하는 인터페이스
//TODO: DirectX를 이용한 캡처 클래스를 만든다.
struct ICapturer
{
	virtual void BeginCapture(wxEvtHandler* handler, const wxRect& rect, uint32_t duration) = 0;
	virtual std::pair<wxImage, uint32_t> CaptureFrame() = 0;
	virtual void EndCapture() = 0;
	virtual ~ICapturer() {}
};
