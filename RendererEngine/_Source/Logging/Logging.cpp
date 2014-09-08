// Header Files
//=============

#include "Logging.h"

#include <fstream>

// Helper Class Declaration
//=========================

namespace
{
	class cLogging
	{
		// Interface
		//----------

	public:

		// Logging
		bool LogMessage( const std::string& i_messageToLog );

		// Construction / Destruction
		cLogging();
		~cLogging();

		// Data
		//-----

	private:

		std::ofstream m_outputStream;

	};
}

// Static Data Initialization
//===========================

namespace
{
	const char* s_fileName_log = "Renderer.log";
	cLogging s_logger;
}

// Function Definition
//====================

namespace RendererEngine
{
	bool LogMessage( const std::string& i_messageToLog )
	{
		return s_logger.LogMessage( i_messageToLog );
	}
}

// Helper Class Definition
//========================

namespace
{
	// Interface
	//----------

	// Logging
	bool cLogging::LogMessage( const std::string& i_messageToLog )
	{
		// Write the message to the file
		m_outputStream << i_messageToLog << "\n";

		// For performance reasons the ofstream's buffering should be used;
		// in cases of crashes, however, messages can be lost if they haven't been written to file yet.
		// I am flushing the buffer after every message in debug builds,
		// but feel free to change this behavior
#ifdef _DEBUG
		m_outputStream.flush();
#endif

		return true;
	}

	// Construction / Destruction
	cLogging::cLogging()
		:
		m_outputStream( s_fileName_log )
	{

	}
	cLogging::~cLogging()
	{
		m_outputStream.close();
	}
}
