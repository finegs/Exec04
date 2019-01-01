/*
 * AppHelloWorld.cpp
 *
 *  Created on: 2018. 12. 30.
 *      Author: finegs
 */

#if 0

// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/window.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
enum
{
    ID_Hello = 1
};
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame-> Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

#endif

#if 0

// wx stuff
#include <wx/frame.h>
#include <wx/thread.h>
#include <wx/menu.h>
#include <wx/app.h>

// misc stuff
#include <stdlib.h>
#include <assert.h>
#include <map>
#include <list>

// events for worker thread
//DECLARE_EVENT_TYPE(wxEVT_THREAD, -1)
//DEFINE_EVENT_TYPE(wxEVT_THREAD)

class tJOB
{
public:
  enum tCOMMANDS // list of commands that are currently implemented
  {
    eID_THREAD_EXIT=wxID_EXIT, // thread should exit or wants to exit
    eID_THREAD_NULL=wxID_HIGHEST+1, // dummy command
    eID_THREAD_STARTED, // worker thread has started OK
    eID_THREAD_JOB, // process normal job
    eID_THREAD_JOBERR // process errorneous job after which thread likes to exit
  }; // enum tCOMMANDS

  tJOB() : m_cmd(eID_THREAD_NULL) {}
  tJOB(tCOMMANDS cmd, const wxString& arg) : m_cmd(cmd), m_Arg(arg) {}
  tCOMMANDS m_cmd; wxString m_Arg;
}; // class tJOB

class QUEUE
{
public:
  enum tPRIORITY { eHIGHEST, eHIGHER, eNORMAL, eBELOW_NORMAL, eLOW, eIDLE }; // priority classes
  QUEUE(wxEvtHandler* pParent) : m_pParent(pParent) {}
  void AddJob(const tJOB& job, const tPRIORITY& priority=eNORMAL) // push a job with given priority class onto the FIFO
  {
    wxMutexLocker lock(m_MutexQueue); // lock the queue
    m_Jobs.insert(std::make_pair(priority, job)); // insert the prioritized entry into the multimap
    m_QueueCount.Post(); // new job has arrived: increment semaphore counter
  } // void AddJob(const tJOB& job, const tPRIORITY& priority=eNORMAL)
  tJOB Pop()
  {
    tJOB element;
    m_QueueCount.Wait(); // wait for semaphore (=queue count to become positive)
    m_MutexQueue.Lock(); // lock queue
    element=(m_Jobs.begin())->second; // get the first entry from queue (higher priority classes come first)
    m_Jobs.erase(m_Jobs.begin()); // erase it
    m_MutexQueue.Unlock(); // unlock queue
    return element; // return job entry
  } // tJOB Pop()
  void Report(const tJOB::tCOMMANDS& cmd, const wxString& sArg=wxEmptyString, int iArg=0) // report back to parent
  {
    wxCommandEvent evt(wxEVT_THREAD, cmd); // create command event object
    evt.SetString(sArg); // associate string with it
    evt.SetInt(iArg);
    m_pParent->AddPendingEvent(evt); // and add it to parent's event queue
  } // void Report(const tJOB::tCOMMANDS& cmd, const wxString& arg=wxEmptyString)
  size_t Stacksize() // helper function to return no of pending jobs
  {
    wxMutexLocker lock(m_MutexQueue); // lock queue until the size has been read
    return m_Jobs.size();
  }

private:
  wxEvtHandler* m_pParent;
  std::multimap<tPRIORITY, tJOB> m_Jobs; // multimap to reflect prioritization: values with lower keys come first, newer values with same key are appended
  wxMutex m_MutexQueue; // protects queue access
  wxSemaphore m_QueueCount; // semaphore count reflects number of queued jobs
};

class WorkerThread : public wxThread
{
public:
  WorkerThread(QUEUE* pQueue, int id=0) : m_pQueue(pQueue), m_ID(id) { assert(pQueue); wxThread::Create(); }

private:
  QUEUE* m_pQueue;
  int m_ID;

  virtual wxThread::ExitCode Entry()
  {
    Sleep(1000); // sleep a while to simulate some time-consuming init procedure
    tJOB::tCOMMANDS iErr;
    m_pQueue->Report(tJOB::eID_THREAD_STARTED, wxEmptyString, m_ID); // tell main thread that worker thread has successfully started
    try { while(true) OnJob(); } // this is the main loop: process jobs until a job handler throws
    catch(tJOB::tCOMMANDS& i) { m_pQueue->Report(iErr=i, wxEmptyString, m_ID); } // catch return value from error condition
    return (wxThread::ExitCode)iErr; // and return exit code
  } // virtual wxThread::ExitCode Entry()

  virtual void OnJob()
  {
    tJOB job=m_pQueue->Pop(); // pop a job from the queue. this will block the worker thread if queue is empty
    switch(job.m_cmd)
    {
    case tJOB::eID_THREAD_EXIT: // thread should exit
      Sleep(1000); // wait a while
      throw tJOB::eID_THREAD_EXIT; // confirm exit command
    case tJOB::eID_THREAD_JOB: // process a standard job
      Sleep(2000);
      m_pQueue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s done."), job.m_Arg.c_str()), m_ID); // report successful completion
      break;
    case tJOB::eID_THREAD_JOBERR: // process a job that terminates with an error
      m_pQueue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s errorneous."), job.m_Arg.c_str()), m_ID);
      Sleep(1000);
      throw tJOB::eID_THREAD_EXIT; // report exit of worker thread
      break;
    case tJOB::eID_THREAD_NULL: // dummy command
    default: break; // default
    } // switch(job.m_cmd)
  } // virtual void OnJob()
}; // class WorkerThread : public wxThread


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------
class MyFrame : public wxFrame
{
  enum { eQUIT=wxID_CLOSE, eSTART_THREAD=wxID_HIGHEST+100 };
  DECLARE_DYNAMIC_CLASS(MyFrame)
public:
  MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title), m_pQueue(NULL)
  {
    wxMenu *fileMenu=new wxMenu;
    fileMenu->Append(eSTART_THREAD,   _T("&Start a thread\tAlt-S"),     _T("Starts one worker thread"));
    fileMenu->Append(tJOB::eID_THREAD_EXIT,     _T("Sto&p a thread\tAlt-P"), _T("Stops one worker thread"));
    fileMenu->Append(tJOB::eID_THREAD_JOB,      _T("&Add job to thread\tAlt-A"),  _T("Adds a job to the worker thread"));
    fileMenu->Append(tJOB::eID_THREAD_JOBERR,   _T("Add &errorneous job to thread\tAlt-E"), _T("Adds am errorneous job to the worker thread"));
    fileMenu->Append(eQUIT,      _T("E&xit\tAlt-X"),  _T("Exit this program"));
    wxMenuBar *menuBar=new wxMenuBar();
    menuBar->Append(fileMenu, _T("&Options"));
    SetMenuBar(menuBar);
    EnableThreadControls(false); // disable thread controls since worker thread isn't running yet
    fileMenu->Enable(eSTART_THREAD, true); // starting threads should always be possible
    CreateStatusBar();
    SetStatusText(_T("Worker thread sample"));
    m_pQueue=new QUEUE(this);
  } // MyFrame(const wxString& title)
  ~MyFrame() { delete m_pQueue; }
  void EnableThreadControls(bool bEnable)
  {
    wxMenu* pMenu=GetMenuBar()->GetMenu(0);
    static const int MENUIDS[]={eSTART_THREAD, tJOB::eID_THREAD_EXIT, tJOB::eID_THREAD_JOB, tJOB::eID_THREAD_JOBERR};
    for(unsigned int i=0; i<WXSIZEOF(MENUIDS); pMenu->Enable(MENUIDS[i++], bEnable));
  }
  void OnStart(wxCommandEvent& WXUNUSED(event)) // start one worker thread
  {
    int id=m_Threads.empty()?1:m_Threads.back()+1;
    m_Threads.push_back(id);
    WorkerThread* pThread=new WorkerThread(m_pQueue, id); // create a new worker thread, increment thread counter (this implies, thread will start OK)
    pThread->Run();
    SetStatusText(wxString::Format(wxT("[%i]: Thread started."), id));
  }
  void OnStop(wxCommandEvent& WXUNUSED(event)) // stop one worker thread
  {
    if(m_Threads.empty()) { EnableThreadControls(false); Destroy(); return; } // no thread(s) running: frame can be destroyed right away
    m_pQueue->AddJob(tJOB(tJOB::eID_THREAD_EXIT, wxEmptyString), QUEUE::eHIGHEST); // add eID_THREAD_EXIT notification with highest priority to bypass other running jobs
    SetStatusText(_T("Stopping thread..."));
  }
  void OnJob(wxCommandEvent& event) // handler for launching a job for worker thread(s)
  {
    int iJob=rand();
    m_pQueue->AddJob(tJOB((tJOB::tCOMMANDS)event.GetId(), wxString::Format(wxT("%u"), iJob)));
    SetStatusText(wxString::Format(wxT("Job #%i started."), iJob)); // just set the status text
  }
  void OnThread(wxCommandEvent& event) // handler for thread notifications
  {
    switch(event.GetId())
    {
    case tJOB::eID_THREAD_JOB:
      SetStatusText(wxString::Format(wxT("[%i]: %s"), event.GetInt(), event.GetString().c_str())); // progress display
      break;
    case tJOB::eID_THREAD_EXIT:
      SetStatusText(wxString::Format(wxT("[%i]: Stopped."), event.GetInt()));
      m_Threads.remove(event.GetInt()); // thread has exited: remove thread ID from list
      if(m_Threads.empty()) { EnableThreadControls(false); Destroy(); } // destroy main window if no more threads
      break;
    case tJOB::eID_THREAD_STARTED:
      SetStatusText(wxString::Format(wxT("[%i]: Ready."), event.GetInt()));
      EnableThreadControls(true); // at least one thread successfully started: enable controls
      break;
    default: event.Skip();
    }
  }
  void OnQuit(wxCommandEvent& WXUNUSED(event))
  {
    if(m_Threads.empty()) { Destroy(); return; } // no thread(s) running - exit right away
    for(size_t t=0; t<m_Threads.size(); ++t) m_pQueue->AddJob(tJOB(tJOB::eID_THREAD_EXIT, wxEmptyString), QUEUE::eHIGHEST); } // send all running threads the "EXIT" signal
  void OnClose(wxCloseEvent& WXUNUSED(event))  { wxCommandEvent e; OnQuit(e); } // just run OnQuit() which will terminate worker threads and destroy the main frame
private:
  MyFrame() : wxFrame(), m_pQueue(nullptr) {}
  QUEUE* m_pQueue;
  std::list<int> m_Threads;
  DECLARE_EVENT_TABLE()
}; // class MyFrame : public wxFrame
IMPLEMENT_DYNAMIC_CLASS(MyFrame, wxFrame)
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(tJOB::eID_THREAD_JOB, MyFrame::OnJob)
EVT_MENU(tJOB::eID_THREAD_JOBERR, MyFrame::OnJob)
EVT_MENU(eSTART_THREAD, MyFrame::OnStart)
EVT_MENU(tJOB::eID_THREAD_EXIT, MyFrame::OnStop)
EVT_COMMAND(wxID_ANY, wxEVT_THREAD, MyFrame::OnThread)
EVT_MENU(eQUIT, MyFrame::OnQuit)
EVT_CLOSE(MyFrame::OnClose)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// the application
// ----------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
  virtual bool OnInit()
  {
    if(!wxApp::OnInit()) return false;
    MyFrame *frame = new MyFrame(_T("Minimal wxWidgets App"));
    frame->SetSize(200, 300);
    frame->Show(true);
    return true;
  }
}; // class MyApp : public wxApp
IMPLEMENT_APP(MyApp)


#endif


#if 1

#include <iostream>

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
//    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = 1
};
//wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//    EVT_MENU(ID_Hello,   MyFrame::OnHello)
//    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
//    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
//wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{

	std::cout << "#### MyApp::OnInit #### " << std::endl;

    MyFrame *frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

#endif
