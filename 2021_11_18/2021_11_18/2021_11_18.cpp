// 2021_11_18.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2021_11_18.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 1. 시작 인수로부터 notepad의 PID를 획득
//  --> 실행 인수(문자열)로부터 획득. --> 숫자로 변환
//  --> 지역변수로 선언될 경우, WndProc 등에서 사용 불가

int g_pid;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //int value = 0;
    WCHAR buf[128] = { 0, };

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 2. 시작 인수의 값을 문자열에서 숫자로 변환하여 획득
    g_pid = _wtoi(lpCmdLine);

    wsprintfW(buf, L"Recv value: %d", g_pid);
    MessageBox(NULL, buf, buf, MB_OK);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY20211118, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20211118));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20211118));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY20211118);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// OS가 프로세스를 생성한 다음 프로세스의 정보를 반환 받기 위한 용도
// 생성 후 반환되는 프로세스의 정보
PROCESS_INFORMATION g_pi, pi;
// 실행되는 프로세스의 초기 상태 값
// 생성 전 필요한 설정 정보
STARTUPINFO g_si, si;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
        HANDLE handle;
        //TerminateProcess(g_pi.hProcess, 0);

        if (0 == g_pid)
            break;

        handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_pid);
        TerminateProcess(handle, 0);

        //TerminateProcess(GetCurrentProcess(), 0);
        //ExitProcess(0);
    }
        break;
    case WM_KEYDOWN:
    {
        // notepad의 base thread 일시 정지
        SuspendThread(g_pi.hThread);
        // 5초간 정지
        Sleep(5000);
        // base thread 재개
        ResumeThread(g_pi.hThread);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        WCHAR note[128] = L"c:\\windows\\system32\\notepad.exe";
        WCHAR me[128] = L"D:\\2021_wikim\\2021_vcpp2\\2021_11_18\\Debug\\2021_11_18.exe";
        WCHAR mbuf[128] = { 0, };
        if (0 != g_pid)
            break;

        // 3. notepad 실행 ==> notepad 프로세스 정보를 획득
        CreateProcess(NULL, note, NULL, NULL, FALSE, 0, NULL, NULL, &g_si, &g_pi);
        // 4. 실행 프로그램 경로 + 시작 인수 문자열 생성
        wsprintfW(mbuf, L"%s %d", me, g_pi.dwProcessId);
        // 5. 자식 프로세스를 생성
        CreateProcess(NULL, mbuf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    }

        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            WCHAR buf[128] = { 0, };
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            
            wsprintfW(buf, L"Child - PID[ %d ] TID[ %d ]", g_pi.dwProcessId, g_pi.dwThreadId);
            TextOut(hdc, 10, 10, buf, lstrlenW(buf));
            wsprintfW(buf, L"My Information - PID[ %d ] TID[ %d ]",
                GetCurrentProcessId(), GetCurrentThreadId( ));
            TextOut(hdc, 10, 30, buf, lstrlenW(buf));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
