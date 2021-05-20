#pragma once
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <cstdint>
#include <assert.h>
#include <WinSock2.h>
#include <string>

using bool_t = int; // instead of using BOOL

#pragma pack(push, 1)

	//下载任务设置
	struct DownTaskParam
	{
		DownTaskParam()
		{
			memset(this, 0, sizeof(DownTaskParam));
			nReserved1		 = 5;
			bReserved			 = false;
			DisableAutoRename	 = false;
			IsOnlyOriginal		 = false;
			IsResume			 = true;
		}
		DownTaskParam(const DownTaskParam &) = default;
		int nReserved;
		wchar_t szTaskUrl[2084];          // 任务URL
		wchar_t szRefUrl[2084];           // 引用页
		wchar_t szCookies[4096];          // 浏览器cookie
		wchar_t szFilename[MAX_PATH];     // 下载保存文件名.
		wchar_t szReserved0[MAX_PATH];
		wchar_t szSavePath[MAX_PATH];     // 文件保存目录
		HWND  hReserved;
		bool_t bReserved; 
		wchar_t szReserved1[64];
		wchar_t szReserved2[64];
		bool_t IsOnlyOriginal;            // 是否只从原始地址下载
		uint32_t nReserved1;
		bool_t DisableAutoRename;         // 禁止智能命名
		bool_t IsResume;                  // 是否用续传
		uint32_t reserved[2048];
	};

	//下载状态
	enum class  DOWN_TASK_STATUS
	{
		NOITEM=0,
		TSC_ERROR,
		TSC_PAUSE,
		TSC_DOWNLOAD,
		TSC_COMPLETE,
		TSC_STARTPENDING,
		TSC_STOPPENDING
	};

	//错误信息
	enum class TASK_ERROR_TYPE
	{
		TASK_ERROR_UNKNOWN=					0x00,   // 未知错误
		TASK_ERROR_DISK_CREATE=				0x01,   // 创建文件失败
		TASK_ERROR_DISK_WRITE=				0x02,   // 写文件失败
		TASK_ERROR_DISK_READ=				0x03,   // 读文件失败
		TASK_ERROR_DISK_RENAME=				0x04,   // 重命名失败
		TASK_ERROR_DISK_PIECEHASH=			0x05,   // 文件片校验失败
		TASK_ERROR_DISK_FILEHASH=			0x06,   // 文件全文校验失败
		TASK_ERROR_DISK_DELETE=				0x07,   // 删除文件失败失败
		TASK_ERROR_DOWN_INVALID=			0x10,   // 无效的DOWN地址
		TASK_ERROR_PROXY_AUTH_TYPE_UNKOWN=	0x20,   // 代理类型未知
		TASK_ERROR_PROXY_AUTH_TYPE_FAILED= 	0x21,   // 代理认证失败
		TASK_ERROR_HTTPMGR_NOT_IP=			0x30,   // http下载中无ip可用
		TASK_ERROR_TIMEOUT=					0x40,   // 任务超时
		TASK_ERROR_CANCEL=					0x41,   // 任务取消
		TASK_ERROR_TP_CRASHED=              0x42,   // MINITP崩溃
		TASK_ERROR_ID_INVALID=	            0x43,   // TaskId 非法
	};
	struct DownTaskInfo
	{
		DownTaskInfo()
		{
			memset(this, 0, sizeof(DownTaskInfo));
			stat		= DOWN_TASK_STATUS::TSC_PAUSE;
			fail_code	= TASK_ERROR_TYPE::TASK_ERROR_UNKNOWN;
			fPercent = 0;
			bIsOriginUsable = false;
			fHashPercent = 0;
		}

		DownTaskInfo(const DownTaskInfo &) = default;
		DOWN_TASK_STATUS	stat;
		TASK_ERROR_TYPE		fail_code;
		wchar_t				szFilename[MAX_PATH];
		wchar_t				szReserved0[MAX_PATH];
		int64_t     		nTotalSize;         // 该任务总大小(字节)
		int64_t     		nTotalDownload;     // 下载有效字节数(可能存在回退的情况)
		float				fPercent;           // 下载进度
		int					nReserved0;
		int					nSrcTotal;          // 总资源数
		int					nSrcUsing;          // 可用资源数
		int					nReserved1;
		int					nReserved2;
		int					nReserved3;
		int					nReserved4;
		int64_t     		nReserved5;
		int64_t				nDonationP2P;       // p2p贡献字节数
		int64_t				nReserved6;
		int64_t				nDonationOrgin;		// 原始资源共享字节数
		int64_t				nDonationP2S;		// 镜像资源共享字节数
		int64_t				nReserved7;
		int64_t     		nReserved8;
		int					nSpeed;             // 即时速度(字节/秒)
		int					nSpeedP2S;          // 即时速度(字节/秒)
		int					nSpeedP2P;          // 即时速度(字节/秒)
		bool_t				bIsOriginUsable;    // 原始资源是否有效
		float				fHashPercent;       // 现不提供该值
		int					IsCreatingFile;     // 是否创建文件
		uint32_t			reserved[64];
	};
	
	enum class DOWN_PROXY_TYPE
	{
		PROXY_TYPE_IE	 = 0,
		PROXY_TYPE_HTTP  = 1,
		PROXY_TYPE_SOCK4 = 2,
		PROXY_TYPE_SOCK5 = 3,
		PROXY_TYPE_FTP   = 4,
		PROXY_TYPE_UNKOWN  = 255,
	};

	//代理授权信息
	enum class DOWN_PROXY_AUTH_TYPE
	{
		PROXY_AUTH_NONE =0,
		PROXY_AUTH_AUTO,
		PROXY_AUTH_BASE64,
		PROXY_AUTH_NTLM,
		PROXY_AUTH_DEGEST,
		PROXY_AUTH_UNKOWN,
	};

	//下载代理设置
	struct DOWN_PROXY_INFO
	{
		bool_t					bIEProxy;
		bool_t					bProxy;
		DOWN_PROXY_TYPE			stPType;
		DOWN_PROXY_AUTH_TYPE	stAType;
		wchar_t					szHost[2048];
		uint32_t				nPort;
		wchar_t					szUser[50];
		wchar_t					szPwd[50];
		wchar_t					szDomain[2048];
	};


#pragma pack(pop)

template<class T>
struct _function_t{using type = T *;};

template<class T>
using function_t = typename _function_t<T>::type;

template<class Tto>
inline Tto cast(HMODULE module,const char *name){
	return (Tto)GetProcAddress(module, name);
} 

namespace{
	using Init_t 		 	 			= 	function_t<bool_t(void)>;
	using UnInit_t		 	 			= 	function_t<bool_t(void)>;
	using CreateTask_t	 	 			=   function_t<HANDLE(DownTaskParam &stParam)>;
	using DeleteTask_t	 	 			=   function_t<bool_t(HANDLE hTask)>;
	using StartTask_t	 	 			=	function_t<bool_t(HANDLE hTask)>;
	using StopTask_t 	 	 			=   function_t<bool_t(HANDLE hTask)>;
	using QueryTask_t		 			=   function_t<bool_t(HANDLE hTask,DownTaskInfo &stTaskInfo)>;
	using QueryTaskEx_t		 			= 	function_t<bool_t(HANDLE hTask,DownTaskInfo &stTaskInfo)>;
	using LimitSpeed_t		 			= 	function_t<void(int32_t nKBps)>;
	using LimitUploadSpeed_t 			=	function_t<void(int32_t nTcpBps,int32_t nOtherBps)>;
	using DelTempFile_t		 			=	function_t<bool_t(DownTaskParam &stParam)>;
	using SetProxy_t		 			=	function_t<bool_t(DOWN_PROXY_INFO &stProxy)>;
	using SetUserAgent_t	 			=  	function_t<void(const wchar_t* pszUserAgent)>;
	using GetFileSizeWithUrl_t			=  	function_t<bool_t(const wchar_t * lpURL, int64_t & iFileSize)>;
	using ParseThunderPrivateUrl_t 		=	function_t<bool_t(const wchar_t *pszThunderUrl, wchar_t *normalUrlBuffer, int32_t bufferLen)>;
	using SetAdditionInfo_t				=   function_t<long(HANDLE task_id, WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, long buf_len )>;
	using SetFileIdAndSize_t			=	function_t<bool_t(HANDLE hTask, char szFileId[40], uint64_t nFileSize)>;
	using SetThreadNum_t     			=	function_t<bool_t(HANDLE hTask, uint32_t origin_thread_num, uint32_t sum_thread_num)>;
	using CreateTaskByURL_t				= 	function_t<HANDLE (const wchar_t *url, const wchar_t *path, const wchar_t *fileName, BOOL IsResume)>;
	using CreateTaskByThunder_t 		= 	function_t<HANDLE (wchar_t *pszUrl, wchar_t *pszFileName, wchar_t *pszReferUrl, wchar_t *pszCharSet, wchar_t *pszCookie)>;
	using CreateBTTaskByThunder_t 		= 	function_t<HANDLE(const wchar_t *pszPath)>;
	
	/**
	 * @brief exceptions
	 * 
	 */
	namespace{
		static constexpr auto ModuleLoadFailed = "Module Load Failed!";
		static constexpr auto ModuleNotLoaded = "Module Not Loaded!";
		static constexpr auto CreateTaskFailed = "Create Task Failed!";
		static constexpr auto QueryTaskFailed = "Query Task Failed!";

		using XLException = decltype(ModuleLoadFailed);
	};
};

class Task{
	public:
		template<class TFunc>
		inline void CHECKFUNC(TFunc t){
			if(t == nullptr){
				throw ModuleNotLoaded;
			}
		}
		Task(HANDLE id){
			taskID = id;
		}

		Task(const Task& task){
			taskID = task.GetID();
		}

		Task(Task && task){
			taskID = task.GetID();
			task.taskID = nullptr;
		}

		/**
		 * @brief Get the IDｏf the task
		 * 
		 * @return HANDLE 
		 */
		HANDLE GetID()const{
			return taskID;
		}

		inline bool Start();
		inline bool Stop();
		inline bool Delete();
		inline DownTaskInfo Query();
		inline DownTaskInfo QueryEx(){
			return this->Query();
		}
		/**
		 * @brief Set the number of threads
		 * 
		 * @param origin_thread_num  original source threads
		 * @param sum_thread_num     total threads
		 * @return true 
		 * @return false 
		 */
		inline bool SetThreadNum(uint32_t origin_thread_num, uint32_t sum_thread_num);

		inline long SetAdditionInfo(WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, long buf_len );

		inline bool SetFileIdAndSize(char szFileId[40], uint64_t nFileSize);
	protected:
	HANDLE taskID;
};

/**
 * @brief Thunder Engone Class, ensure that the resources will be freed if it is destructed.
 * 
 */
class ThunderEngine{
public:
	
	friend class Task;

	ThunderEngine() noexcept :
		dllPath("./xldl.dll")
		{
		}
	~ThunderEngine() noexcept{
		if(fnUnInit) fnUnInit();
		if(engineModule) FreeLibraryAndExitThread(engineModule,0);

		fnUnInit 					= nullptr;
		fnInit 	 					= nullptr;
		fnCreateBTTaskByThunder 	= nullptr;
		fnCreateTask 				= nullptr;
		fnDeleteTask 				= nullptr;
		fnStartTask 				= nullptr;
		fnStopTask 					= nullptr;
		fnQueryTask					= nullptr;
		fnQueryTaskEx				= nullptr;
		fnSetThreadNum				= nullptr;
		fnSetUserAgent				= nullptr;
		fnGetFileSizeWithUrl		= nullptr;
		fnSetProxy					= nullptr;
		fnLimitUploadSpeed			= nullptr;
		fnLimitSpeed				= nullptr;
		engineModule 				= nullptr;
		fnDelTempFile				= nullptr;
		fnParseThunderPrivateUrl	= nullptr;
		fnSetAdditionInfo			= nullptr;
		fnSetFileIdAndSize			= nullptr;
	}

	void SetDllPath(std::string dllPath) noexcept{
		this->dllPath = dllPath;
	}

	const std::string &GetDllPath() noexcept{
		return dllPath;
	}

	template<class TFunc>
	inline void CHECKFUNC(TFunc t){
		if(t == nullptr){
			throw ModuleNotLoaded;
		}
	}

	bool Init(){
		if(fnInit) return true;
		if(engineModule) return true;
		engineModule = LoadLibraryA(dllPath.data());
		if(!engineModule) throw ModuleLoadFailed;

		fnInit   = cast<Init_t>(engineModule, "XL_Init");
		fnUnInit = cast<UnInit_t>(engineModule, "XL_UnInit");
		fnCreateTask = cast<CreateTask_t>(engineModule, "XL_CreateTask");
		fnCreateTaskByThunder = cast<CreateTaskByThunder_t>(engineModule, "XL_CreateTaskByThunder");
		fnCreateTaskByURL = cast<CreateTaskByURL_t>(engineModule, "XL_CreateTaskByURL");
		fnCreateBTTaskByThunder = cast<CreateBTTaskByThunder_t>(engineModule,"XL_CreateBTTaskByThunder");		
		fnStartTask = cast<StartTask_t>(engineModule, "XL_StartTask");
		fnStopTask = cast<StopTask_t>(engineModule,"XL_StopTask");
		fnDeleteTask = cast<DeleteTask_t>(engineModule,"XL_DeleteTask");
		fnQueryTask = cast<QueryTask_t>(engineModule,"XL_QueryTaskInfo");
		fnQueryTaskEx = cast<QueryTaskEx_t>(engineModule,"XL_QueryTaskInfoEx");
		fnSetThreadNum = cast<SetThreadNum_t>(engineModule,"XL_SetThreadNum");
		fnSetUserAgent = cast<SetUserAgent_t>(engineModule,"XL_SetUserAgent");
		fnGetFileSizeWithUrl = cast<GetFileSizeWithUrl_t>(engineModule,"XL_GetFileSizeWithUrl");
		fnSetProxy = cast<SetProxy_t>(engineModule,"XL_SetProxy");
		fnLimitSpeed = cast<LimitSpeed_t>(engineModule,"XL_SetSpeedLimit");
		fnLimitUploadSpeed = cast<LimitUploadSpeed_t>(engineModule,"XL_SetUploadSpeedLimit");
		fnDelTempFile = cast<DelTempFile_t>(engineModule,"XL_DeleteTempFile");
		fnParseThunderPrivateUrl = cast<ParseThunderPrivateUrl_t>(engineModule,"XL_ParseThunderPrivateUrl");
		fnSetAdditionInfo = cast<SetAdditionInfo_t>(engineModule,"XL_SetAdditionInfo");
		fnSetFileIdAndSize = cast<SetFileIdAndSize_t>(engineModule,"XL_SetFileIdAndSize");
		return fnInit() == TRUE;
	}

	/**
	 * @brief Create a Task
	 * 
	 * @param  taskConfig  
	 * @return Task 
	 */
	Task CreateTask(DownTaskParam &taskConfig){
		CHECKFUNC(fnCreateTask);
		HANDLE handle = fnCreateTask(taskConfig);
		if(!handle) throw CreateTaskFailed;
		return Task{handle};
	}

	/**
	 * @brief Create a Task by providing URL
	 * 
	 * @param url 
	 * @param path 
	 * @param fileName 
	 * @param IsResume resume or new
	 * @return Task 
	 */
	Task CreateTask(const wchar_t *url, const wchar_t *path, const wchar_t *fileName, BOOL IsResume = TRUE){
		CHECKFUNC(fnCreateTaskByURL);
		HANDLE handle = fnCreateTaskByURL(url,path,fileName,IsResume);
		if(!handle) throw CreateTaskFailed;

		return Task{handle};
	}

	/**
	 * @brief Create a Task by URL (Thunder://)
	 * 
	 * @param pszUrl 
	 * @param pszFileName 
	 * @param pszReferUrl 
	 * @param pszCharSet 
	 * @param pszCookie 
	 * @return Task 
	 */
	Task CreateTask(wchar_t *pszUrl, wchar_t *pszFileName, wchar_t *pszReferUrl, wchar_t *pszCharSet, wchar_t *pszCookie){
		CHECKFUNC(fnCreateTaskByThunder);
		HANDLE handle = fnCreateTaskByThunder(pszUrl,pszFileName,pszReferUrl,pszCharSet,pszCookie);
		if(!handle) throw CreateTaskFailed;

		return Task{handle};
	}

	/**
	 * @brief Create a Task by uisng a BT file
	 * 
	 * @param pszBTPath 
	 * @return Task 
	 */
	Task CreateTask(const wchar_t *pszBTPath){
		CHECKFUNC(fnCreateBTTaskByThunder);
		HANDLE handle = fnCreateBTTaskByThunder(pszBTPath);
		if(!handle) throw CreateTaskFailed;

		return Task{handle};
	}

	void SetUserAgent(const wchar_t* pszUserAgent){
		CHECKFUNC(fnSetUserAgent);
		return fnSetUserAgent(pszUserAgent);
	}

	bool SetProxy(DOWN_PROXY_INFO &stProxy){
		CHECKFUNC(fnSetProxy);
		return fnSetProxy(stProxy);
	}

	bool GetFileSizeWithUrl(const wchar_t * lpURL, int64_t & iFileSize){
		CHECKFUNC(fnGetFileSizeWithUrl);
		return fnGetFileSizeWithUrl(lpURL,iFileSize);
	}

	void LimitSpeed(int32_t nKBps){
		CHECKFUNC(fnLimitSpeed);
		return fnLimitSpeed(nKBps);
	}	

	void LimitUploadSpeed(int32_t nTcpBps,int32_t nOtherBps){
		CHECKFUNC(fnLimitUploadSpeed);
		return fnLimitUploadSpeed(nTcpBps, nOtherBps);
	}

	bool DeleteTempFile(DownTaskParam &param){
		CHECKFUNC(fnDelTempFile);
		return fnDelTempFile(param);
	}

	bool ParseThunderPrivateUrl(const wchar_t *pszThunderUrl, wchar_t *normalUrlBuffer, int32_t bufferLen){
		CHECKFUNC(fnParseThunderPrivateUrl);
		return fnParseThunderPrivateUrl(pszThunderUrl,normalUrlBuffer,bufferLen);
	}
protected:
	std::string dllPath;
	static HMODULE engineModule;

	static Init_t 					fnInit;
	static UnInit_t					fnUnInit;
	static CreateTask_t 			fnCreateTask;
	static CreateTaskByThunder_t    fnCreateTaskByThunder;
	static CreateTaskByURL_t		fnCreateTaskByURL;
	static CreateBTTaskByThunder_t  fnCreateBTTaskByThunder;
	static StartTask_t				fnStartTask;
	static StopTask_t				fnStopTask;
	static DeleteTask_t				fnDeleteTask;
	static QueryTask_t				fnQueryTask;
	static QueryTaskEx_t			fnQueryTaskEx;
	static SetThreadNum_t			fnSetThreadNum;
	static SetUserAgent_t			fnSetUserAgent;
	static GetFileSizeWithUrl_t		fnGetFileSizeWithUrl;
	static SetProxy_t				fnSetProxy;
	static LimitSpeed_t				fnLimitSpeed;
	static LimitUploadSpeed_t		fnLimitUploadSpeed;
	static DelTempFile_t			fnDelTempFile;
	static ParseThunderPrivateUrl_t fnParseThunderPrivateUrl;
	static SetAdditionInfo_t		fnSetAdditionInfo;
	static SetFileIdAndSize_t		fnSetFileIdAndSize;
};
#ifndef THUNDER_STATIC_INITED
	#define THUNDER_STATIC_INITED
	#define INIT_STATIC_MEMBER(type,name) type ThunderEngine::name = nullptr;
		INIT_STATIC_MEMBER(HMODULE, engineModule)
		INIT_STATIC_MEMBER(Init_t, fnInit)
		INIT_STATIC_MEMBER(UnInit_t, fnUnInit)
		INIT_STATIC_MEMBER(CreateTask_t, fnCreateTask)
		INIT_STATIC_MEMBER(CreateTaskByThunder_t, fnCreateTaskByThunder)
		INIT_STATIC_MEMBER(CreateTaskByURL_t, fnCreateTaskByURL)
		INIT_STATIC_MEMBER(StartTask_t, fnStartTask)
		INIT_STATIC_MEMBER(StopTask_t, fnStopTask)
		INIT_STATIC_MEMBER(DeleteTask_t, fnDeleteTask)
		INIT_STATIC_MEMBER(CreateBTTaskByThunder_t, fnCreateBTTaskByThunder)
		INIT_STATIC_MEMBER(QueryTask_t, fnQueryTask)
		INIT_STATIC_MEMBER(QueryTaskEx_t, fnQueryTaskEx)
		INIT_STATIC_MEMBER(SetThreadNum_t, fnSetThreadNum)
		INIT_STATIC_MEMBER(SetUserAgent_t, fnSetUserAgent)
		INIT_STATIC_MEMBER(GetFileSizeWithUrl_t, fnGetFileSizeWithUrl)
		INIT_STATIC_MEMBER(SetProxy_t, fnSetProxy)
		INIT_STATIC_MEMBER(LimitUploadSpeed_t, fnLimitUploadSpeed)
		INIT_STATIC_MEMBER(LimitSpeed_t, fnLimitSpeed)
		INIT_STATIC_MEMBER(DelTempFile_t, fnDelTempFile)
		INIT_STATIC_MEMBER(ParseThunderPrivateUrl_t, fnParseThunderPrivateUrl)
		INIT_STATIC_MEMBER(SetAdditionInfo_t, fnSetAdditionInfo)
		INIT_STATIC_MEMBER(SetFileIdAndSize_t, fnSetFileIdAndSize)
	#undef INIT_STATIC_MEMBER
#endif


bool Task::Start(){
	CHECKFUNC(ThunderEngine::fnStartTask);
	return ThunderEngine::fnStartTask(this->taskID);
}

bool Task::Stop(){
	CHECKFUNC(ThunderEngine::fnStopTask);
	return ThunderEngine::fnStopTask(this->taskID);
}

bool Task::Delete(){
	CHECKFUNC(ThunderEngine::fnDeleteTask);
	return ThunderEngine::fnDeleteTask(this->taskID);
}

DownTaskInfo Task::Query(){
	CHECKFUNC(ThunderEngine::fnQueryTaskEx);
	DownTaskInfo di;
	if(!ThunderEngine::fnQueryTaskEx(this->taskID, di)){
		throw QueryTaskFailed;
	}
	return di;
}

bool Task::SetThreadNum(uint32_t origin_thread_num, uint32_t sum_thread_num){
	CHECKFUNC(ThunderEngine::fnSetThreadNum);
	return ThunderEngine::fnSetThreadNum(this->taskID, origin_thread_num, sum_thread_num);
}

long Task::SetAdditionInfo(WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, long buf_len ){
	CHECKFUNC(ThunderEngine::fnSetAdditionInfo);
	return ThunderEngine::fnSetAdditionInfo(this->taskID, sock_info, http_resp_buf, buf_len);
}

bool Task::SetFileIdAndSize(char *szFileId, uint64_t nFileSize){
	CHECKFUNC(ThunderEngine::fnSetFileIdAndSize);

	return ThunderEngine::fnSetFileIdAndSize(this->taskID, szFileId, nFileSize);
}