
#include <AsyncLogger.hpp>

#if 1

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
using namespace asynclogger;

template<typename ... Args>
std::string& ALOGGER_DLL asynclogger::sformat(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format, args ...);
	unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format, args ...);
	return std::string(buf.get(), buf.get()+size-1);
}

void ALOGGER_DLL asynclogger::mformat(const char* format) {
	std::cout << format;
}

template<typename T, typename... Targs>
void ALOGGER_DLL asynclogger::mprintf(const char* format, T value, Targs... Fargs) {
	for(;*format!= '\0';format++) {
		if(*format=='%') {
			std::cout << value;
			mprintf(format+1, Fargs...);
			return;
		}
		std::cout << *format;
	}
}

namespace asynclogger {



AsyncLogMsg::AsyncLogMsg( const char* _msg, const AsyncLogLevel& level) {
	int len;

	createTimePoint = system_clock::now();
	msg = new char[len = strlen(_msg)];
	memcpy(msg, _msg, len);
	this->level = level;
}

AsyncLogMsg::~AsyncLogMsg() {
	delete[] msg;
}

AsyncLogger::AsyncLogger() : isStart(true), isPause(true), out(std::cout.rdbuf()) {
	sleepInterValMills = milliseconds(3000);
	ts = new char[25];
	logWriter = NULL;
	level = L_INFO;
}

AsyncLogger::~AsyncLogger() {
	stop();
	delete[] ts;
}


void AsyncLogger::AsyncLogger::start() {
	handleSyncLog(L_INFO, "AsyncLogger start initiated ");
	if(!isStart) isStart = true;
	if(isPause) isPause = false;
	if(!logWriter)
		logWriter = new std::thread(AsyncLogger::subTask, this);
	handleSyncLog(L_INFO, "AsyncLogger start completed ");
}

void AsyncLogger::AsyncLogger::stop() {
	handleSyncLog(L_INFO, "AsyncLogger stop initiated ");
	if(!isPause) isPause = true;
	if(isStart) isStart = false;
	cv.notify_all();
	if(logWriter)
		logWriter->join();
	delete logWriter;

	handleSyncLog(L_INFO, "AsyncLogger stop completed");
}

void AsyncLogger::AsyncLogger::resume() {
	handleSyncLog(L_INFO, "AsyncLogger resume initiated");
	start();
	cv.notify_all();

	handleSyncLog(L_INFO, "AsyncLogger resume completed");
}

void AsyncLogger::AsyncLogger::pause() {
	handleSyncLog(L_INFO, "AsyncLogger pause initiated");
	if(!isStart) isStart = true;
	if(!isPause) isPause = true;
	if(!logWriter)
		logWriter = new std::thread(AsyncLogger::subTask, this);

	handleSyncLog(L_INFO, "AsyncLogger pause completed");
}

void AsyncLogger::log(const AsyncLogLevel& level, const std::string& str) {
	std::stringstream ss = std::stringstream();
	ss << " tid="<< std::this_thread::get_id() << ", msg=" << str;
	AsyncLogMsg* msg = new AsyncLogMsg(ss.str().c_str(), level);
	std::unique_lock<std::mutex> lck(mtx);
	msgQueue.push(msg);
	lck.unlock();
	cv.notify_all();
}

void AsyncLogger::debug(const std::string& str) {
	log(L_DEBUG, str.c_str());
}
void AsyncLogger::info(const std::string& str) {
	log(L_INFO, str.c_str());
}
void AsyncLogger::error(const std::string& str) {
	log(L_ERROR, str.c_str());
}
void AsyncLogger::fatal(const std::string& str) {
	log(L_FATAL, str.c_str());
}

void AsyncLogger::debug(const char* msg) {
	log(L_DEBUG, msg);
}
void AsyncLogger::info(const char* msg) {
	log(L_INFO, msg);
}
void AsyncLogger::error(const char* msg) {
	log(L_ERROR, msg);
}
void AsyncLogger::fatal(const char* msg) {
	log(L_FATAL, msg);
}

void AsyncLogger::handleLog(const AsyncLogMsg* msg)  {
	if(!msg) return;
	if(msg->getLevel() < this->getLevel()) return;
	handleLog(msg->getLevel(), msg->getMsg(), msg->getTimePoint());
}

void AsyncLogger::handleLog(const char* cStr, const TimePoint& np = system_clock::now()) {
	handleLog(L_INFO, cStr, np);
}

void AsyncLogger::handleLog(const AsyncLogLevel& level, const char* msg, const TimePoint& np = system_clock::now()) {
	if(!msg) return;
	if(level < this->getLevel()) return;
	outMtx.lock();
	getTS(np, ts);
	out << ts << " : ";
	switch(level) {
	case L_DEBUG:
		out << MSG_DBG; break;
	case L_INFO:
		out << MSG_INF; break;
	case L_ERROR:
		out << MSG_ERR; break;
	case L_FATAL:
		out << MSG_FATAL; break;
	}
	out << " " << msg << std::endl;
	outMtx.unlock();
}

void AsyncLogger::handleSyncLog(const AsyncLogLevel& level, const char* msg) {
	handleLog(level, msg, system_clock::now());
}

const char* AsyncLogger::getTS() {
	TimePoint tnp = system_clock::now();
	getTS(tnp, ts);
	return ts;
}

void AsyncLogger::getTS(const TimePoint& np, char* ts) {
	time_t ct = system_clock::to_time_t(np);
	std::strftime(ts, 25, "%Y-%m-%d %H:%M:%S", localtime(&ct));
	system_clock::duration since_epoch = np.time_since_epoch();
	seconds s = duration_cast<seconds>(since_epoch);
	since_epoch -= s;
	milliseconds milli = duration_cast<milliseconds>(since_epoch);
	sprintf(ts, "%s.%03lld", ts, milli.count());
}

void AsyncLogger::syncDebugLog(const char* msg) {
	handleLog(L_DEBUG, msg, system_clock::now());
}

void AsyncLogger::AsyncLogger::subTask() {
	std::stringstream ss = std::stringstream();
	ss << "AsyncLogger subTask - T[" << std::this_thread::get_id() << "]";
	handleSyncLog(L_INFO, ss.str().c_str());

	while(isStart) {
		try {
			if(isPause) {
				std::unique_lock<std::mutex> lck(mtx);
				try {
					//					cv.wait_for(lck, sleepInterValMills);
					cv.wait(lck, [this](){ return !isPause;});
				}
				catch (...)
				{
					handleSyncLog(L_FATAL, "Exception 1: ");
				}
				lck.unlock();
				continue;
			}

			while (!isPause) {
				std::unique_lock<std::mutex> lck(mtx);
				cv.wait(lck, [this](){ return !msgQueue.empty();});
				AsyncLogMsg* msg = msgQueue.front();
				msgQueue.pop();
				lck.unlock();
				handleLog(msg);
				delete msg;
			}
		}
		catch (std::exception& ex) {
			std::string t = std::string("Exception : ");
			t.append(ex.what());

			handleSyncLog(L_FATAL, t.c_str());
		}
		catch (...) {
			handleSyncLog(L_FATAL, "Unknown Exception : ");
		}
	}
}

void AsyncLogger::join() {
	if(!logWriter) return;
	logWriter->join();
}

}

#endif


#if 0
class lazyWorkerIdleTerminate {
    typedef enum state {
        STARTED,
        STOPPING,
        STOPPED
    } state_t;

    std::queue<std::string> queue;
    std::mutex              mutex;
    std::thread             thread;
    std::condition_variable condition;
    std::atomic<state_t>    state = ATOMIC_VAR_INIT(STOPPED);

    void process() {
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);

        while (true) {
            lock.lock();
            while (queue.empty()) {
                if (condition.wait_for(lock, std::chrono::seconds(30)) == std::cv_status::timeout || state == STOPPING) {
                    state = STOPPED;
                    return;
                }
            }
            std::string work = queue.front();
            queue.pop();
            lock.unlock();

            // Some processing here...
	    
	    std::cout << "[" << std::this_thread::get_Id() << "] : " << work << std::endl;
        }
    }

public:
    static lazyWorkerIdleTerminate& instance() {
        static lazyWorkerIdleTerminate instance;
        return instance;
    }

    void doWork(std::string work) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(work);
            if (state == STOPPED) {
                state = STARTED;
                if (thread.joinable()) thread.detach();
                thread = std::thread(&lazyWorkerIdleTerminate::process, this);
                return;
            }
        }
        condition.notify_one();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (state != STARTED) return;
            state = STOPPING;
        }
        condition.notify_one();
        thread.join();
    }

    lazyWorkerIdleTerminate() {}

    ~lazyWorkerIdleTerminate() {
        stop();
    }
};
#endif
