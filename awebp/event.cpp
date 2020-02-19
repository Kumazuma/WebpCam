#include "wx/wxprec.h"
#include "event.h"
wxDEFINE_EVENT(EVT_RefreshView, wxCommandEvent);
wxDEFINE_EVENT(EVT_FINISH_ENCODE, wxCommandEvent);
wxDEFINE_EVENT(EVT_FAILED_ENCODE, wxCommandEvent);
wxDEFINE_EVENT(EVT_ADDED_A_FRAME, wxCommandEvent);
wxDEFINE_EVENT(EVT_ANIM_PROCESS_A_FRAME, wxCommandEvent);//재생이 한 프레임 진행되었다는 알림