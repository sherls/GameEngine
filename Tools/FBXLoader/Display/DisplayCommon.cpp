#include <fstream>

#include "DisplayCommon.h"

#ifdef DUMP_FBX_INFO

namespace Tools
{
	namespace DisplayCommon
	{
		std::string *dumpInfoFilename;
	}
}

void Tools::DisplayCommon::Initialize( const std::string &i_dumpInfoFilename )
{
	dumpInfoFilename = new std::string( i_dumpInfoFilename );
}

void Tools::DisplayCommon::ShutDown( void )
{
	dumpInfoFilename->clear();
	delete dumpInfoFilename;
}

void Tools::DisplayCommon::DisplayMetaDataConnections( FbxObject* pObject )
{
	int totalMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if( totalMetaData > 0 )
		DisplayString( "    MetaData connections " );

	for( int i = 0; i < totalMetaData; i++ )
	{
		FbxObjectMetaData *metaData = pObject->GetSrcObject<FbxObjectMetaData>( i );
		DisplayString( "        Name: ", (char*)metaData->GetName() );
	}
}

void Tools::DisplayCommon::DisplayString( const char* pHeader, const char* pValue , const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;

	string = pHeader;
	string += pValue;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::DisplayBool( const char* pHeader, bool pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;

	string = pHeader;
	string += pValue ? "true" : "false";
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::DisplayInt( const char* pHeader, int pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;

	string = pHeader;
	string += pValue;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::DisplayDouble( const char* pHeader, double pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;
	FbxString floatValue = (float) pValue;

	floatValue = pValue <= -HUGE_VAL ? "-INFINITY" : floatValue.Buffer();
	floatValue = pValue >=  HUGE_VAL ?  "INFINITY" : floatValue.Buffer();

	string = pHeader;
	string += floatValue;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::Display2DVector( const char* pHeader, FbxVector2 pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : floatValue2.Buffer();

	string = pHeader;
	string += floatValue1;
	string += ", ";
	string += floatValue2;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf(string);
	out << string;

	out.close();
}

void Tools::DisplayCommon::Display3DVector( const char* pHeader, FbxVector4 pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];
	FbxString floatValue3 = (float)pValue[2];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : floatValue2.Buffer();
	floatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : floatValue3.Buffer();
	floatValue3 = pValue[2] >=  HUGE_VAL ?  "INFINITY" : floatValue3.Buffer();

	string = pHeader;
	string += floatValue1;
	string += ", ";
	string += floatValue2;
	string += ", ";
	string += floatValue3;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::Display4DVector( const char* pHeader, FbxVector4 pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];
	FbxString floatValue3 = (float)pValue[2];
	FbxString floatValue4 = (float)pValue[3];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : floatValue2.Buffer();
	floatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : floatValue3.Buffer();
	floatValue3 = pValue[2] >=  HUGE_VAL ?  "INFINITY" : floatValue3.Buffer();
	floatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : floatValue4.Buffer();
	floatValue4 = pValue[3] >=  HUGE_VAL ?  "INFINITY" : floatValue4.Buffer();

	string = pHeader;
	string += floatValue1;
	string += ", ";
	string += floatValue2;
	string += ", ";
	string += floatValue3;
	string += ", ";
	string += floatValue4;
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void DisplayColor( const char* pHeader, FbxPropertyT<FbxDouble3> pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( Tools::DisplayCommon::dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;

	string = pHeader;
	//lString += (float) pValue.mRed;
	//lString += (double)pValue.GetArrayItem(0);
	string += " (red), ";
	//lString += (float) pValue.mGreen;
	//lString += (double)pValue.GetArrayItem(1);
	string += " (green), ";
	//lString += (float) pValue.mBlue;
	//lString += (double)pValue.GetArrayItem(2);
	string += " (blue)";
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}

void Tools::DisplayCommon::DisplayColor( const char* pHeader, FbxColor pValue, const char* pSuffix )
{
	std::ofstream out;

	out.open( dumpInfoFilename->c_str(), std::ios::app );
	if( out.fail() )
		return;

	FbxString string;

	string = pHeader;
	string += (float) pValue.mRed;

	string += " (red), ";
	string += (float) pValue.mGreen;

	string += " (green), ";
	string += (float) pValue.mBlue;

	string += " (blue)";
	string += pSuffix;
	string += "\n";
	//FBXSDK_printf( string );
	out << string;

	out.close();
}
#endif	// #ifdef DUMP_FBX_INFO