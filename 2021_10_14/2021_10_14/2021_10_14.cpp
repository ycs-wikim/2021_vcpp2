// 2021_10_14.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2021_10_14.h"

#include <stdlib.h>             // 랜덤(무작위) 관련 함수
#include <time.h>               // SEED 변경. 현재 시간 값이 SEED로 사용되면 항상 다른 값.

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
    LoadStringW(hInstance, IDC_MY20211014, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY20211014));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20211014));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY20211014);
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

// 나와 상대의 좌표를 보관할 변수 선언
RECT g_me, g_you[ 3 ];
// 아이템 변수 선언
RECT g_item;

// 상대 난이도 조절을 위한 시간 값
int g_level;

// 제한 시간. 기본 10초. 아이템을 먹으면 10초 리셋. 0이되면 게임
int g_time;

// 점수(아이템을 먹었을때 증가. WM_PAINT 표시) 누적
int g_score;

// 무적키
int g_inf;

// 타이머 ID 값을 선언
#define TIMER_ID_1      1       // 상대의 움직임을 위한 주기적인 호출 ID
#define TIMER_ID_2      2       // 제한 시간 타이머

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

    // 나의 이동
    case WM_KEYDOWN:
        RECT dst;       // 겹침 영역 확인용 자료형

        switch (wParam)
        {
        case VK_LEFT:   // 나의 x 좌표 값은 감소
            g_me.left -= 10;
            g_me.right -= 10;
            break;

        case VK_RIGHT:  // 나의 x 좌표 값은 증가
            g_me.left += 10;
            g_me.right += 10;
            break;

        case VK_UP:     // 나의 y 좌표 값은 감소
            g_me.top -= 10;
            g_me.bottom -= 10;
            break;

        case VK_DOWN:   // 나의 x 좌표 값은 증가
            g_me.top += 10;
            g_me.bottom += 10;
            break;

        case VK_F6:
            g_inf = 1;
            break;
        }
        // 좌표에 대한 계산이 모두 완료된 상태

        // item을 먹었는지 확인
        if (TRUE == IntersectRect(&dst, &g_me, &g_item))
        {
            // 제한 시간 리셋
            g_time = 10;
            // 난이도도 초기화
            g_level = 1000;

            // 점수 증가
            g_score += 100;
            /*
            // 1. 아이템 제거
            g_item.left = -10;
            g_item.top = -10;
            g_item.right = -5;
            g_item.bottom = -5;
            */
            // 2. 상대방 정지
            //KillTimer(hWnd, TIMER_ID_1);

            // 4. 아이템 위치 재설정
            g_item.left = rand() % 400;
            g_item.top = rand() % 400;
            g_item.right = g_item.left + 40;
            g_item.bottom = g_item.top + 40;

            // 3. 상대방이 느려진다
            KillTimer(hWnd, TIMER_ID_1);
            SetTimer(hWnd, TIMER_ID_1, 1000, NULL);
        }

        // 화면 무효화 -> WM_PAINT를 호출
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    // 프로그램 초기화
    case WM_CREATE:
        // 나의 좌표를 설정. 40 * 40 정사각형
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = g_me.left + 40;
        g_me.bottom = g_me.top + 40;

        // 상대의 좌표를 설정. 40 * 40
        g_you[ 0 ].left = 400;
        g_you[0].top = 400;
        g_you[0].right = g_you[0].left + 40;
        g_you[0].bottom = g_you[0].top + 40;

        g_you[1].left = rand() % 400;
        g_you[1].top = rand() % 400;
        g_you[1].right = g_you[1].left + 40;
        g_you[1].bottom = g_you[1].top + 40;

        g_you[2].left = rand() % 400;
        g_you[2].top = rand() % 400;
        g_you[2].right = g_you[2].left + 40;
        g_you[2].bottom = g_you[2].top + 40;


        // 무작위 SEED 값을 현재 시간으로 설정
        srand(time(NULL));

        // 아이템의 좌표를 설정. 40 * 40
        g_item.left = rand() % 400;         // 0 ~ 399
        g_item.top = rand() % 400;
        g_item.right = g_item.left + 40;
        g_item.bottom = g_item.top + 40;

        // 제한 시간 설정
        g_time = 10;
        SetTimer(hWnd, TIMER_ID_2, 1000, NULL);

        // 최초 타이머 설정
        g_level = 1000;
        // 타이머를 설정. 1초마다 주기적으로 호출
        // 설정한 시간마다 WM_TIMER 호출 -> OS에 요청
        SetTimer(hWnd, TIMER_ID_1, g_level, NULL);
        break;

    case WM_TIMER:      // timer ID 값을 가지는 변수: 
    {
        int i = 0;

        if (TIMER_ID_2 == wParam)       // 제한 시간 ID 값에 의한 주기적인 호출인지 확인
        {
            // 제한 시간 감소
            //g_time--;

            // 제한 시간이 만료되면.
            if (0 == g_time)
            {
                // 게임 종료
                KillTimer(hWnd, TIMER_ID_1);
                KillTimer(hWnd, TIMER_ID_2);
                MessageBox(hWnd, L"타이머에 의한 종료", L"흐흐", MB_OK);
                return 0;
            }

            break;
        }
        // 상대가 움직이도록 구성

        for (i = 0; i < 3; i++)
        {
            // x 좌표에 대해 비교
            if (g_me.left < g_you[i].left)     // 나는 상대의 왼쪽
            {
                // 상대의 좌표를 변경 -> 왼쪽으로
                g_you[i].left -= 10;
                g_you[i].right -= 10;
            }
            else
            {
                // 상대의 좌표를 변경 -> 오른쪽으로
                g_you[i].left += 10;
                g_you[i].right += 10;
            }

            // y 좌표에 대해 비교
            if (g_me.top < g_you[i].top)           // 참 : 나는 상대의 위쪽
            {
                // 상대의 좌표를 변경 -> 위쪽으로
                g_you[i].top -= 10;
                g_you[i].bottom -= 10;
            }
            else
            {
                // 상대의 좌표를 변경 -> 아래쪽으로
                g_you[i].top += 10;
                g_you[i].bottom += 10;
            }
            // 상대의 좌표 변경이 완료된 시점

            if (1 != g_inf)
            {
                // 상대와 나의 겹침 영역 확인
                if (TRUE == IntersectRect(&dst, &g_me, &g_you[i]))    // 겹침 영역이 있다!
                {
                    // 주기적인 호출을 중단
                    KillTimer(hWnd, TIMER_ID_1);
                    // 부모 윈도우에 종속적인 윈도우
                    MessageBox(hWnd, L"겹쳐부러써~", L"아이고", MB_OK);
                    // WndProc 함수를 종료
                    return 0;
                }
            }
        }

        // 주기적 호출 시간을 확인
        if (100 != g_level)
        {
            g_level -= 100;     // 1초당 0.1초씩 줄어든다.

            KillTimer(hWnd, TIMER_ID_1);                    // 1번을 정지
            SetTimer(hWnd, TIMER_ID_1, g_level, NULL);      // 1번으로 새로운 타이머 생성
        }

        // 화면 무효화
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_PAINT:
        {
            int i = 0;
            WCHAR string[64] = { 0, };
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            // 점수 정보 출력
            wsprintfW(string, L"점수: %d    제한 시간: %d초", g_score, g_time);
            TextOut(hdc, 500, 20, string, lstrlenW(string));

            // 나 그리기
            Ellipse(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            // 상대 그리기
            for (i = 0; i < 3; i++)
            {
                Rectangle(hdc, g_you[i].left, g_you[i].top, g_you[i].right, g_you[i].bottom);
            }
            // item draw
            Ellipse(hdc, g_item.left, g_item.top, g_item.right, g_item.bottom);
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
