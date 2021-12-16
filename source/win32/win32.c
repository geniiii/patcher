/************************************************************************
*                                                                       *
*   Copyright (c) Microsoft Corp. All rights reserved.                  *
*                                                                       *
************************************************************************/

WINBASEAPI _Ret_maybenull_ HANDLE WINAPI CreateFileMappingA(_In_     HANDLE hFile,
                                                            _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
                                                            _In_     DWORD flProtect,
                                                            _In_     DWORD dwMaximumSizeHigh,
                                                            _In_     DWORD dwMaximumSizeLow,
                                                            _In_opt_ LPCSTR lpName);
