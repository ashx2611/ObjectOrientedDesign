#pragma once
#include "XmlWrite.h"
//Test stub for xmlwrite
#ifdef XML_Test
int main()
{
	xml xml;
	NoSqlDb<string> n;
	xml.writexml(n);

}
#endif