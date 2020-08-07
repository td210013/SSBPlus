// CppUnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CppUnitTest.h"

#include "cppunit/ui/text/TestRunner.h"
#include "Cppunit/extensions/TestFactoryRegistry.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

using namespace CppUnit;

int _tmain(int argc, CHAR* argv[], CHAR* envp[])
{
	int nSuccessful = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nSuccessful = 1;
	}
	else
	{
        std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

        std::cout << "Running " << testPath;

        TextUi::TestRunner runner;

        runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    
        nSuccessful = runner.run(testPath);
	}

	return nSuccessful;
}


