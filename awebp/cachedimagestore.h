#pragma once
#include<wx/wx.h>
#include"interface.h"
#include<wx/mstream.h>
#include<wx/tarstrm.h>
#include<wx/wfstream.h>
//메모리에 적재하다가 적당한 때에 적당하게 파일로 밀어버린다.
//
class CacheStoreImageBuilder :public IImageStoreBuilder
{
private:
	wxMemoryOutputStream m_memStream;
	wxTarOutputStream m_tarStream;
	wxFileOutputStream m_fileStream;
	wxString m_fileName;
public:
	CacheStoreImageBuilder();
};