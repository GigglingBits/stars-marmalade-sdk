#include "LogManager.h"
#include "s3eDialog.h"
#include "IwDebug.h"
#include "Debug.h"

LogManager* LogManager::s_pxInstance = NULL;

LogManager::LogManager() {
}

LogManager::~LogManager() {
}

void LogManager::Initialize() {
	if (!s_pxInstance) {
		s_pxInstance = new LogManager();
	}

	s3eDialogRegister(S3E_DIALOG_FINISHED, (s3eCallback)DialogFinishedCallback, NULL);
}

void LogManager::Terminate() {
    s3eDialogUnRegister(S3E_DIALOG_FINISHED, (s3eCallback)DialogFinishedCallback);

	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

LogManager& LogManager::GetInstance() {
	IwAssertMsg(MYAPP, s_pxInstance, ("The log manager is not initialized."));
	return *s_pxInstance;
}

void LogManager::WriteMessage(const std::string& msg) {
	if (s3eDialogAvailable() == S3E_FALSE) {
		IwAssertMsg(MYAPP, false, ("Cannot show native dialog box! Msg: %s", msg.c_str()));
	}

	s3eDialogAlertInfo info;
    info.m_Message = msg.c_str();
    info.m_Title = "Message";
    //info.m_Button[0] = "Ok";
    //info.m_Button[1] = "Cancel";
    //info.m_Button[2] = "...";

    s3eDialogAlertBox(&info);
}

int32 LogManager::DialogFinishedCallback(void* sysdata, void* usrdata) {
	return 0;
}
