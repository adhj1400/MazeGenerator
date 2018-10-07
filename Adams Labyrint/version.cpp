/**
 *	Author:	Adam Hjernquist
 */

#pragma once

#include <iostream>
#include <string>
#include "windows.h"
#include "version.h"

/**
 *	Print the version number of this <lptstrFilename>
 *	filename executable.
 */
void Version::print(LPCWSTR lptstrFilename) 
{
	DWORD					dwLen = 0;
	BYTE					*pbVersionInfo = NULL;
	VS_FIXEDFILEINFO		*Info = NULL;
	UINT					puLenFileInfo = 0;

	// Get size of file version information
	dwLen = GetFileVersionInfoSize(lptstrFilename, NULL);
	pbVersionInfo = new BYTE[dwLen];

	// Structure the information
	GetFileVersionInfo(lptstrFilename, 0, dwLen, pbVersionInfo);

	// Get specific member
	VerQueryValue(pbVersionInfo, TEXT("\\"), (LPVOID*)&Info, &puLenFileInfo);

	// Print version info
	printf("Program version: %d.%d.%d.%d\n",
		(Info->dwFileVersionMS >> 16) & 0xff,
		(Info->dwFileVersionMS >> 0) & 0xff,
		(Info->dwFileVersionLS >> 16) & 0xff,
		(Info->dwFileVersionLS >> 0) & 0xff);
}