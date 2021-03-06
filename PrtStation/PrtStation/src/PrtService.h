#pragma once


enum
{
	WARN_PRINT_FAILED = 1,
	WARN_WRITE_FAILED,
	WARN_PARSE_FAILED,

	WARN_NEED_CLEAR_PRINTER, /*提示清洁打印机*/
	WARN_NEED_CLEAR_PRINTER_END, /*提示打印机任务满*/
};

class PrtService
{
private:
	PrtService(void);
	~PrtService(void);

	void* m_etpEvtThrdPool;

	bool m_isRegistering;

	static void get_task_timer_hdl(void* param1, void* param2, void* param3);

	static void _registerTaskEntity(void* param1, void* param2, void *param3);
	static void _printTaskEntity(void* param1, void* param2, void *param3);

	void _taskHandle(int sessionId, char *taskData, int taskDataLen, char *taskSerialNo);

public:
	int init();
	void cleanup(void);
	
	static PrtService* instance();

	BOOL isRegistering();
	BOOL isTaskHdling();

	int registerTaskHdl();
	int printTaskHdl(PRT_TASK_T *prtInfo);

	unsigned int m_ReqCnt;
	unsigned int m_SevrNoTaskCnt;

	BOOL m_stopGetTask;
	BOOL m_printerStopped;

	int m_printerSdPercent;
	int m_printerPrtedCnt;
};

