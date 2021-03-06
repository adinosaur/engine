#include "XMLFileSerializer.h"
#include "..\Helpers\Helpers.h"
#include "XMLContainer.h"

namespace star
{
	XMLFileSerializer::XMLFileSerializer(
		const tstring & path, DirectoryMode mode)
		: m_File(path, mode)
	{

	}

	XMLFileSerializer::~XMLFileSerializer() {}

	void XMLFileSerializer::Write(
		XMLContainer & container
		)
	{
		tstring buffer = WriteFile(container);
		WriteTextFile(m_File.GetLocalPath(), buffer, m_File.GetDirectoryMode());
	}

	void XMLFileSerializer::Write(
			XMLContainer & container,
			const tstring & binaryPath
			)
	{
		Write(container);
		container.Serialize(binaryPath, m_File.GetDirectoryMode());
	}

	tstring XMLFileSerializer::WriteFile(XMLContainer & container)
	{
		tstringstream strstrResult;
		strstrResult << _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") << std::endl;
		uint32 tabs(0);
		WriteChild(strstrResult, container, tabs);

		return strstrResult.str();
	}

	void XMLFileSerializer::WriteAtributes(tstringstream & strstr, XMLContainer & element)
	{
		auto attributes = element.GetAttributes();
		for(auto attribute : attributes)
		{
			strstr << _T(" ") << attribute.first << _T("=\"") << attribute.second << _T("\"");
		}
	}

	void XMLFileSerializer::WriteChild(tstringstream & strstr, XMLContainer & element, uint32 & tabs)
	{
		strstr << GetTabString(tabs) << _T("<") << element.GetName();
		WriteAtributes(strstr, element);
		if(element.size() == 0 && element.GetValue() == EMPTY_STRING)
		{
			strstr << _T("/>") << std::endl;
		}
		else
		{
			strstr << _T(">");
			if(element.GetValue() != EMPTY_STRING)
			{
				strstr << element.GetValue();
				strstr << _T("</") << element.GetName();
				strstr << _T(">") << std::endl;
			}
			else if(element.size() > 0)
			{
				++tabs;
				strstr << std::endl;
				for(auto child : element)
				{
					XMLContainer child_value;
					child_value = *(child.second);
					WriteChild(strstr, child_value, tabs);
				}
				--tabs;
				strstr << GetTabString(tabs) << _T("</") << element.GetName() << _T(">") << std::endl;
			}
		}
	}

	tstring XMLFileSerializer::GetTabString(uint32 & tabs)
	{
		tstring tab_string(EMPTY_STRING);
		for(uint32 i = 0 ; i < tabs ; ++i)
		{
			tab_string += TAB;
		}
		return tab_string;
	}
}
