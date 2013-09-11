#include "stdafx.h"
#include "TestMoveFile.h"
#include "FileManagement/ApiMoveFile.h"
#include "../File.h"
#include "FileManagement/ApiCreateFile.h"
#include "FileManagement/ApiDeleteFile.h"
#include "DirectoryManagement/ApiCreateDirectory.h"
#include "DirectoryManagement/ApiRemoveDirectory.h"

using namespace std;

void Test_MoveFile_Normal()
{
    DEF_TESTLOG_T("Test_MoveFile_Normal, �⺻ ���� �̵� �׽�Ʈ");

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    BOOL result = apiMoveFile(pSrcFileName.c_str(), pDstFileName.c_str());

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

void Test_MoveFile_NoExistingSrc()
{
    DEF_TESTLOG_T("Test_MoveFile_NoExistingSrc, �������� �ʴ� ������ �̵� �õ�");

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

    BOOL result = apiMoveFile(pSrcFileName.c_str(), pDstFileName.c_str());

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

void Test_MoveFile_SharingViolationSrcShareMode(DWORD dwShareMode)
{
    DEF_TESTLOG_T("Test_MoveFile_SharingViolationSrc, ���������� ���� ���� �׽�Ʈ");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwShareMode " << apiCreateFile.GetString_dwShareMode(dwShareMode);

    const wstring pSrcFileName = GetSrcFileName();
    const wstring pDstFileName = GetDstFileName();

    if(!MakeFile(log, pSrcFileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }

    File f(pSrcFileName, GENERIC_READ, dwShareMode, OPEN_EXISTING);

    if(!f.IsValidHandle())
    {
        log.GetStream(TestLog::MT_ERROR) << L"CreateFile" << endl;
        return;
    }

    BOOL result = apiMoveFile(pSrcFileName.c_str(), pDstFileName.c_str());

    f.CloseHandle();
    if(result)
    {
        CheckFileData(log, pDstFileName);
    }

    uDeleteFile(pSrcFileName.c_str());

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

void Test_MoveFile_SharingViolationSrc()
{
    Test_MoveFile_SharingViolationSrcShareMode(0);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_READ);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_WRITE);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_DELETE);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_DELETE);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_WRITE | FILE_SHARE_DELETE);
    Test_MoveFile_SharingViolationSrcShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
}

void Test_MoveFile_ExistingDst()
{
    DEF_TESTLOG_T("Test_MoveFile_ExistingDst, �����ϴ� ���������� ���� ���");

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

    BOOL result = apiMoveFile(pSrcFileName.c_str(), pDstFileName.c_str());

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

void Test_MoveFile_ExistingDstSharingViolationShareMode(DWORD dwShareMode)
{
    DEF_TESTLOG_T("Test_MoveFile_ExistingDstSharingViolationShareMode, ���������� ���� ���ѿ� ���� �׽�Ʈ");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwShareMode " << apiCreateFile.GetString_dwShareMode(dwShareMode);


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

    BOOL result = apiMoveFile(pSrcFileName.c_str(), pDstFileName.c_str());

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

void Test_MoveFile_ExistingDstSharingViolation()
{
    Test_MoveFile_ExistingDstSharingViolationShareMode(0);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_READ);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_WRITE);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_DELETE);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_DELETE);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_WRITE | FILE_SHARE_DELETE);
    Test_MoveFile_ExistingDstSharingViolationShareMode(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
}

void Test_RenameFile_ShareAll()
{
    Test_RenameFile_Share(FILE_SHARE_READ);
    Test_RenameFile_Share(FILE_SHARE_WRITE);
    Test_RenameFile_Share(FILE_SHARE_DELETE);
    Test_RenameFile_Share(FILE_SHARE_READ | FILE_SHARE_WRITE);
    Test_RenameFile_Share(FILE_SHARE_READ | FILE_SHARE_DELETE);
    Test_RenameFile_Share(FILE_SHARE_WRITE | FILE_SHARE_DELETE);
    Test_RenameFile_Share(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
}

void Test_RenameFile_Share(DWORD dwShareMode)
{
    DEF_TESTLOG_T("Test_RenameFile_Share, ���� ���ѿ� ���� �̸� ���� �׽�Ʈ");
    log.GetStream(TestLog::MT_MESSAGE) << L"dwShareMode " << apiCreateFile.GetString_dwShareMode(dwShareMode);

    wstring fileName = GetTestFileName(L"Test_RenameFile_Share");
    if(!TouchFile(log, fileName))
    {
        log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
        return;
    }
    
    BOOL fOk = FALSE;
    {
        File f1(fileName, dwShareMode);
        if((HANDLE)f1 == INVALID_HANDLE_VALUE)
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ �� �� �����ϴ�." << endl;
            return;
        }

        fOk = apiMoveFile(fileName.c_str(), (L"RE_" + fileName).c_str());
        if(!fOk)
        {
            log.GetStream(TestLog::MT_ERROR) << L"���� �̸��� ������ �� �����ϴ�." << endl;
        }
    }

    if(!fOk)
    {
        if(!uDeleteFile(fileName.c_str()))
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
            return;
        }
    }
    else
    {
        if(!uDeleteFile((L"RE_" + fileName).c_str()))
        {
            log.GetStream(TestLog::MT_ERROR) << L"������ ������ �� �����ϴ�." << endl;
            return;
        }
    }    

    log.Ok();
}