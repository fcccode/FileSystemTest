#include "stdafx.h"
#include "TestMoveFileEx.h"
#include "FileManagement/ApiMoveFileEx.h"
#include "../File.h"
#include "FileManagement/ApiCreateFile.h"

using namespace std;

void Test_MoveFileEx_Basic(DWORD dwFlags)
{
    DEF_TESTLOG_T("Test_MoveFileEx_Basic, ���� dwFlags�� �־��.");

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    BOOL result = apiMoveFileEx(pSrcFileName.c_str(), pDstFileName.c_str(), dwFlags);

    if(result)
    {
        CheckFileData(log, pDstFileName);
    }

    if(!DeleteFileOrCheck(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
    }
    if(!DeleteFileOrCheck(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
        return;
    }

    log.Ok();
}

void Test_MoveFileEx_NoExistingSrc(DWORD dwFlags)
{
    DEF_TESTLOG_T("Test_MoveFileEx_NoExistingSrc, �������� �ʴ� �������Ͽ� ���� ����. ���� dwFlags�� �־��.");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwFlags " << apiMoveFileEx.GetString_dwFlags(dwFlags) << endl;;

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(IsFileExist(pSrcFileName))
    {
        if(!uDeleteFile(pSrcFileName.c_str()))
        {
            log.GetStream(TestLog::MT_ERROR) << L"�����ϴ� ������ ������ �� �����ϴ�." << endl;
            return;
        }
    }

    BOOL result = apiMoveFileEx(pSrcFileName.c_str(), pDstFileName.c_str(), dwFlags);

    if(result)
    {
        CheckFileData(log, pDstFileName);
    }

    if(!DeleteFileOrCheck(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
    }
    if(!DeleteFileOrCheck(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
        return;
    }

    log.Ok();
}

void Test_MoveFileEx_SharingViolationSrcShareMode(DWORD dwShareMode, DWORD dwFlags)
{
    DEF_TESTLOG_T("Test_MoveFileEx_SharingViolationSrc, ���� ������ ���������� �޸��ذ��鼭 �׽�Ʈ");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwShareMode " << apiCreateFile.GetString_dwShareMode(dwShareMode) << endl;
    log.GetStream(TestLog::MT_MESSAGE) << L"dwFlags " << apiMoveFileEx.GetString_dwFlags(dwFlags) << endl;

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    {
        File f(pSrcFileName, GENERIC_READ, dwShareMode, OPEN_EXISTING);
        if(!f.IsValidHandle())
        {
            log.GetStream(TestLog::MT_ERROR) << L"CreateFile" << endl;
            return;
        }

        BOOL result = apiMoveFileEx(pSrcFileName.c_str(), pDstFileName.c_str(), dwFlags);

        f.CloseHandle();
        if(result)
        {
            CheckFileData(log, pDstFileName);
        }
    }

    if(!DeleteFileOrCheck(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
    }
    if(!DeleteFileOrCheck(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
        return;
    }

    log.Ok();
}

void Test_MoveFileEx_SharingViolationSrc(DWORD dwFlags)
{
    Test_MoveFileEx_SharingViolationSrcShareMode(0, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_READ, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_WRITE, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_WRITE | FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, dwFlags);
}

void Test_MoveFileEx_ExistingDst(DWORD dwFlags)
{
    DEF_TESTLOG_T("Test_MoveFileEx_ExistingDst, �̹� �����ϴ� ���������Ͽ� ���� ����. ���� dwFlags�� �־��.");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwFlags " << apiMoveFileEx.GetString_dwFlags(dwFlags) << endl;

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    if(!TouchFile(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    BOOL result = apiMoveFileEx(pSrcFileName.c_str(), pDstFileName.c_str(), dwFlags);

    if(result)
    {
        CheckFileData(log, pDstFileName);
    }

    if(!DeleteFileOrCheck(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
    }
    if(!DeleteFileOrCheck(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
        return;
    }

    log.Ok();
}

void Test_MoveFileEx_ExistingDstSharingViolationShareMode(DWORD dwShareMode, DWORD dwFlags)
{
    DEF_TESTLOG_T("Test_MoveFileEx_ExistingDstSharingViolationShareMode, ������ ������ ���� ���ѿ� ���� �׽�Ʈ");
    log.GetStream(TestLog::MT_MESSAGE) << L"������ ������ Ư�� ���� ���� �÷��׷� ������� ä�� ���������� �������Ϸ� �̵� �׽�Ʈ" << endl;
    log.GetStream(TestLog::MT_MESSAGE) << L"dwShareMode " << apiCreateFile.GetString_dwShareMode(dwShareMode) << endl;
    log.GetStream(TestLog::MT_MESSAGE) << L"dwFlags " << apiMoveFileEx.GetString_dwFlags(dwFlags) << endl;

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    if(!TouchFile(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    File f(pDstFileName, GENERIC_READ, dwShareMode, OPEN_EXISTING);
    if(!f.IsValidHandle())
    {
        log.GetStream(TestLog::MT_ERROR) << L"CreateFile" << endl;
        return;
    }

    BOOL result = apiMoveFileEx(pSrcFileName.c_str(), pDstFileName.c_str(), dwFlags);
    f.CloseHandle();

    if(result)
    {
        CheckFileData(log, pDstFileName);
    }    

    if(!DeleteFileOrCheck(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
    }
    if(!DeleteFileOrCheck(log, pDstFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������� ������ ������ �� �����ϴ�." << endl;
        return;
    }

    log.Ok();
}

void Test_MoveFileEx_ExistingDstSharingViolation(DWORD dwFlags)
{
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(0, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_READ, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_WRITE, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_WRITE | FILE_SHARE_DELETE, dwFlags);
    Test_MoveFileEx_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, dwFlags);
}