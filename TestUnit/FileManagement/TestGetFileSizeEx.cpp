#include "stdafx.h"
#include "TestGetFileSizeEx.h"
#include "FileManagement/ApiGetFileSizeEx.h"
#include "../File.h"
#include "FileManagement/ApiDeleteFile.h"
#include "FileIO/ApiWriteFile.h"

using namespace std;

void Test_GetFileSizeEx()
{
    DEF_TESTLOG_T("GetFileSizeEx, Get a specific file size.");

    wstring fileName = GetTestFileName();
    if(!MakeFile(log, fileName.c_str()))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    LARGE_INTEGER largeInteger;

    File f(fileName);
    BOOL fOk = apiGetFileSizeEx(f, &largeInteger);

    if(f.IsValidHandle())
    {
        f.CloseHandle();
    }

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"GetFileSizeEx API�� �����߽��ϴ�." << endl;
        fOk = uDeleteFile(fileName.c_str());

        if(!fOk)
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
            return;
        }
        return;
    }    

    fOk = uDeleteFile(fileName.c_str());

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }    

    log.Ok();
}

void Test_WriteAndGetFileSize()
{
    DEF_TESTLOG_T("Test_WriteAndGetFileSize, Get a specific file size.");

    wstring fileName = GetTestFileName();
    if(!MakeFile(log, fileName.c_str()))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    File f2(fileName, GENERIC_WRITE, 0);
    BYTE buf[4096] = { 'A', };
    DWORD BytesWritten;
    if(!apiWriteFile(f2, buf, sizeof(buf), &BytesWritten, 0))
    {
        log.GetStream(TestLog::MT_ERROR) << L"WriteFile failed" << endl;
    }

    f2.CloseHandle();

    LARGE_INTEGER largeInteger;

    File f(fileName);
    BOOL fOk = apiGetFileSizeEx(f, &largeInteger);

    if(f.IsValidHandle())
    {
        f.CloseHandle();
    }

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"GetFileSizeEx API�� �����߽��ϴ�." << endl;
        fOk = uDeleteFile(fileName.c_str());

        if(!fOk)
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
            return;
        }
        return;
    }    

    fOk = uDeleteFile(fileName.c_str());

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }    

    log.Ok();
}

void Test_WriteAndGetFileSize2()
{
    DEF_TESTLOG_T("Test_WriteAndGetFileSize, ���� �ڵ��� �ϳ� �� �����ä writefile �� getfilesize");

    wstring fileName = GetTestFileName();
    if(!MakeFile(log, fileName.c_str()))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    File f(fileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ|FILE_SHARE_DELETE, OPEN_EXISTING); // ù��° ����
    File f2(fileName, GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ|FILE_SHARE_DELETE, OPEN_EXISTING); // �ι�° ����
    BYTE buf[4096] = { 'A', };
    DWORD BytesWritten;
    if(!apiWriteFile(f2, buf, sizeof(buf), &BytesWritten, 0))
    {
        log.GetStream(TestLog::MT_ERROR) << L"WriteFile failed" << endl;
    }

    LARGE_INTEGER largeInteger;
    BOOL fOk = apiGetFileSizeEx(f, &largeInteger);

    f2.CloseHandle();    

    fOk = apiGetFileSizeEx(f, &largeInteger);

    if(f.IsValidHandle())
    {
        f.CloseHandle();
    }

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"GetFileSizeEx API�� �����߽��ϴ�." << endl;
        fOk = uDeleteFile(fileName.c_str());

        if(!fOk)
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
            return;
        }
        return;
    }    

    fOk = uDeleteFile(fileName.c_str());

    if(!fOk)
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }    

    log.Ok();
}