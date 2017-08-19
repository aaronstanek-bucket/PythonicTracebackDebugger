#ifndef __PYTHONICTRACEBACKDEBUGGER_H_INCLUDED__
#define __PYTHONICTRACEBACKDEBUGGER_H_INCLUDED__

#include <iostream>
#include <vector>
#include <exception>
#include <csignal>
using namespace std;

namespace PythonicTracebackDebugger {

  void setup();

  struct datapoint {
    const char * filename;
    const char * functionName;
    unsigned long long lineNumber;
  };

  class node {
    private:
      vector <datapoint> stack;
      unsigned long long delCount;
    public:
      node() {
        setup();
        delCount = 0;
      };
      void start_function(const char * filename, const char * functionName, unsigned long long ln) {
        while (delCount!=0) {
          stack.resize(stack.size()-delCount);
          delCount = 0;
        };
        datapoint f;
        f.filename = filename;
        f.functionName = functionName;
        f.lineNumber = ln;
        stack.push_back(f);
      };
      void end_function() {
        delCount = delCount+1;
      };
      void update(unsigned long long ln) {
        while (delCount!=0) {
          stack.resize(stack.size()-delCount);
          delCount = 0;
        };
        stack.back().lineNumber = ln;
      };
      void do_traceback() {
        cout << endl;
        cout << "START PTD TRACEBACK" << endl;
        for (unsigned long long i=0;i<stack.size();i++) {
          cout << stack[i].filename << " : " << stack[i].functionName << " : " << stack[i].lineNumber << endl;
        };
        cout << "END PTD TRACEBACK" << endl;
      };
  };

};

PythonicTracebackDebugger::node MY_DEBUGGING_NODE;

namespace PythonicTracebackDebugger {

  class coin {
    public:
      bool one_byte_of_memory;
      coin(const char * filename, const char * functionName, unsigned long long ln) {
        MY_DEBUGGING_NODE.start_function(filename, functionName, ln);
      };
      ~coin() {
        MY_DEBUGGING_NODE.end_function();
      };
  };

  void deal_with_exceptions() {
    MY_DEBUGGING_NODE.do_traceback();
    cout << "..." << endl;
    cout << "UNCAUGHT EXCEPTION" << endl;
    exit(0);
  };

  void deal_with_segfault(int nope) {
    MY_DEBUGGING_NODE.do_traceback();
    cout << "..." << endl;
    cout << "SEGFAULT" << endl;
    exit(0);
  };

  void setup() {
    set_terminate(deal_with_exceptions);
    signal(SIGSEGV,deal_with_segfault);
  };

};

#endif
