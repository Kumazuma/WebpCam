#pragma once
#include<wx/wx.h>
wxDECLARE_EVENT(EVT_RefreshView, wxCommandEvent);
wxDECLARE_EVENT(EVT_FINISH_ENCODE, wxCommandEvent);//인코딩을 완료했을 때
wxDECLARE_EVENT(EVT_FAILED_ENCODE, wxCommandEvent);//인코딩을 실패했을 때
wxDECLARE_EVENT(EVT_ADDED_A_FRAME, wxCommandEvent);//프레임 하나를 처리 완료 했을 때
wxDECLARE_EVENT(EVT_ANIM_PROCESS_A_FRAME, wxCommandEvent);//재생이 한 프레임 진행되었다는 알림
wxDECLARE_EVENT(EVT_FINISH_JOB, wxCommandEvent);