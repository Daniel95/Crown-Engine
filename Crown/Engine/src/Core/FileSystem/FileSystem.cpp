#include "CrownPCH.h"
#include "FileSystem.h"

namespace Crown
{
	File FileSystem::GetFileFromPath(std::string a_Path)
	{
		CROWN_PROFILE_FUNCTION();
		
		///File 
		File file;
		std::ifstream readFile(a_Path);

		///Reading file content
		std::string line;

		if (readFile.is_open())
		{
			file.m_Result = EFileResult::FileResult_Success;

			//Get file content
			while (std::getline(readFile, line))
			{
				file.m_Content += (line + '\n');
			}
			readFile.close();
		}
		//When content could not be read then we
		//return without setting any other data of the file
		else
		{
			file.m_Result = EFileResult::FileResult_Fail;
			return file;
		}

		///Get and set name and directory of file
		std::size_t LastBackslash = a_Path.find_last_of("/\\");
		file.m_Path = a_Path.substr(0, LastBackslash) + "/";
		file.m_Name = a_Path.substr(LastBackslash + 1);

		return file;
	}

	void FileSystem::ToFile(std::string a_Path, std::string a_Content, EWriteFlags a_WriteFlags)
	{
		CROWN_PROFILE_FUNCTION();
		
		std::ofstream outfile;

		///Open file based on the open flags specified
		if (a_WriteFlags == EWriteFlags::WriteFlags_In)
		{
			outfile.open(a_Path);
		}
		else if (a_WriteFlags == EWriteFlags::WriteFlags_Append)
		{
			outfile.open(a_Path, std::ios_base::app);
		}

		outfile << a_Content << std::endl;
	}
}