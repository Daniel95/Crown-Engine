#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#pragma comment(lib, "libSceFios2_stub_weak.a")

namespace Crown
{
    //Flags that are available for writing to files
    enum EWriteFlags
    {
        WriteFlags_In,
        WriteFlags_Append
    };

    //Result enum when opening files
    enum EFileResult
    {
        FileResult_Success,
        FileResult_Fail
    };

    class File
    {
    public:
		std::string m_Name;
		std::string m_Path;
		std::string m_Content;

        //Result of file being loaded in
		EFileResult m_Result;
    };

	class FileSystem
	{
	public:
        //Gets a file and its relevant directory data from a path
		static File GetFileFromPath(std::string a_Path);
        
        //Writes to a file from a path
	    static void ToFile(std::string a_Path, std::string a_Content, EWriteFlags a_WriteFlags = EWriteFlags::WriteFlags_In);
	};
}
