#include "wx/wxprec.h"
#include "cachedimagestore.h"

CacheStoreImageBuilder::CacheStoreImageBuilder():
	m_fileName(wxFileName::CreateTempFileName(wxT("webcam"))),
	m_memStream(), m_tarStream(&m_memStream), m_fileStream(m_fileName)
{

}
