// 2021_10_07.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2021_10_07.h"

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY20211007, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20211007));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20211007));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY20211007);
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


#define EX_01           0       /// 자유 곡선 그리기 예제
#define EX_02           0       /// X 추가로 그리기 예제
#define EX_03           1       /// 그림판 예제

/*
* 그림판 설계 : 메뉴 (2) - 그리기 종류 선택, 색상 선택
*   --> 모든 CALLBACK 함수는 독립적으로 동작한다.
*/

#if EX_01
int g_x, g_y;       // 가장 최신의 과거 위치 보관
int g_flag;         // 플래그 변수. 0 이면 그리지 않는다. 1이면 그린다.
#endif

#if EX_03
int g_x, g_y;           // 마우스 다운 시의 좌표 기억
int g_color;            // 색상 타입 정보
                        // 1(빨강), 2(녹색), 3(파랑)
int g_draw_type;        // 그리기 종류를 기억할 변수
                        // 1(선 그리기), 2(사각형), 3(타원), 4(자유선)
#endif

/// CALLBACK 함수 : 자신이 아닌 프로세스나 라이브러리에 의해 호출되는 외부 노출 함수
/// word, long 자료형이 존재했었음. word는 1byte(char), long 4byte(int)
/// WPARAM : Word Parameter // LPARAM : Long Parameter
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 각종 ID 값을 구분하는 용도
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
#if EX_03
            case ID_32771:      // 선 그리기
                MessageBox(hWnd, L"선 그리기", L"메뉴 선택", MB_OK);
                g_draw_type = 1;
                break;

            case ID_32772:      // 사각형 그리기
                MessageBox(hWnd, L"사각형 그리기", L"메뉴 선택", MB_OK);
                g_draw_type = 2;
                break;

            case ID_32773:      // 타원 그리기
                MessageBox(hWnd, L"타원 그리기", L"메뉴 선택", MB_OK);
                g_draw_type = 3;
                break;

            case ID_32774:      // 자유선 그리기
                MessageBox(hWnd, L"자유선 그리기", L"메뉴 선택", MB_OK);
                g_draw_type = 4;
                break;

            case ID_32775:      // 빨강
                g_color = 1;
                break;

            case ID_32776:      // 녹색
                g_color = 2;
                break;

            case ID_32777:      // 파랑
                g_color = 3;
                break;
#endif
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
#if EX_03
    case WM_LBUTTONDOWN:
    {
        /// 시작 위치 좌표 값 획득
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        // 현재 위치 좌표 값
        int x, y;
        HDC hdc;
        HPEN newPen, osPen;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        hdc = GetDC(hWnd);

        // 펜 생성 및 설정
        switch (g_color)        // 0
        {
        case 1:         // 빨강
            newPen = CreatePen(PS_SOLID, 15, RGB(255, 0, 0));
            break;

        case 2:         // 녹색
            newPen = CreatePen(PS_SOLID, 15, RGB(0, 255, 0));
            break;

        case 3:         // 파랑
            newPen = CreatePen(PS_SOLID, 15, RGB(0, 0, 255));
            break;

        default:
            newPen = CreatePen(PS_SOLID, 15, RGB(0, 0, 0));
            break;
        }

        // OS의 현재 펜과 생성한 펜을 교체
        osPen = (HPEN)SelectObject(hdc, newPen);

        // 실제로 그리기
        switch (g_draw_type)
        {
        case 1:         /// 선
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);
            break;

        case 2:         /// 사각형
            Rectangle(hdc, g_x, g_y, x, y);
            break;

        case 3:         /// 타원
            Ellipse(hdc, g_x, g_y, x, y);
            break;

        case 4:         /// 자유선
            break;
        }
        
        // OS의 원래 펜으로 복원
        SelectObject(hdc, osPen);
        // 펜 자원 해제
        DeleteObject(newPen);

        ReleaseDC(hWnd, hdc);
    }
        break;
#endif

#if EX_02
    case WM_LBUTTONDOWN:
    {
        int x, y;
        
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        RECT rect;
        HDC hdc;

        rect.left = LOWORD(lParam);
        rect.top = HIWORD(lParam);
        rect.right = rect.left + 150;
        rect.bottom = rect.top + 150;

        hdc = GetDC(hWnd);

        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

        MoveToEx(hdc, rect.left, rect.top, NULL);
        LineTo(hdc, rect.right, rect.bottom);

        MoveToEx(hdc, rect.right, rect.top, NULL);
        LineTo(hdc, rect.left, rect.bottom);

        ReleaseDC(hWnd, hdc);

    }
        break;
#endif
#if EX_01
    case WM_LBUTTONDOWN:
    {
        g_flag = 1;     // 그리기 시작을 알림.
        // 좌표 값 획득
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
        g_flag = 0;     // 그리기 종료를 알림.
        break;

    case WM_MOUSEMOVE:
    {
        int x, y;
        HDC hdc;

        if (0 == g_flag)   // 그리기가 아니라면
            break;

        // 현재 마우스 좌표 값
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        // 1. DC 획득
        hdc = GetDC(hWnd);

        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, g_x, g_y);

        // DC 해제
        ReleaseDC(hWnd, hdc);

        g_x = x;        // 가장 최근의 x좌표를 현재 좌표로 입력
        g_y = y;
    }
        break;
#endif
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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
