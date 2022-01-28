// Elliot Karlsson te19a JGY Wargentin
//-----------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>
#include <vector>
//Nyare utseende på fönstret
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// Globala variabler -----------------------------------------------------------------
LPCTSTR ClsName = "Bumchess goes brrrrr";
LPCTSTR WndName = "A Simple Window";
HWND    hWnd;
HWND    edit;
HDC     hdc;
HPEN    myPens[10]; 
HPEN    orginalPen;
int     timerID = 1;
int     app_Wid = 1000, app_Hei = 500;
bool    wich = true;
//------------------------------------------------------------------------------------
struct pos {
    int x;
    int y;
    bool taken = false;
    char marker = 'N';
    int place = -1;
};

std::vector<pos> circle;
std::vector<pos> kryss;
std::vector<pos> gridpos;

// Funktioner ------------------------------------------------------------------------
void             Grid(HWND);
void             createwin(HWND, HINSTANCE);
char             CheckWin(char);
void             Marker(int, int);
void             Circle(int, int);
void             Print(int, int, char);
void	         init(HWND);
void	         cleanUp(HWND);
ATOM             doRegister(HINSTANCE);
BOOL 	         initInstance(HINSTANCE, int);
LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hi, _In_opt_ HINSTANCE hp, _In_ LPSTR lp, _In_ int n) {

    UNREFERENCED_PARAMETER(hp);
    UNREFERENCED_PARAMETER(lp);
    if (!(doRegister(hi)) || !(initInstance(hi, n))) {
        MessageBox(NULL, (LPCSTR)"Fel, Kan ej registrera eller skapa fönstret i windows",
            (LPCSTR)"ERROR", MB_ICONERROR | MB_OK);
        return 0;
    }
    createwin(hWnd, hi);

    //Sparar meddelanden till applikationen 
    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
//------------------------------------------------------------------------------------
ATOM doRegister(HINSTANCE hi) {
    WNDCLASSEX wincl;
    wincl.hInstance = hi;
    wincl.lpszClassName = ClsName;
    wincl.lpfnWndProc = winProc;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    return RegisterClassEx(&wincl);
}
//------------------------------------------------------------------------------------
BOOL initInstance(HINSTANCE hInstance, int nCmdShow) {
    hWnd = CreateWindowEx(
        0,                  //Här kan man sätta olika variationsvärden för utseendet på fönstret
        ClsName,            //(Klassnamn) 
        (LPCSTR)ClsName,    //Namnet på Fönstret
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER, // Olika utseende som man kan ställa in
        ((GetSystemMetrics(SM_CXSCREEN) - app_Wid) >> 1),
        ((GetSystemMetrics(SM_CYSCREEN) - app_Hei) >> 1),
        app_Wid,            // Bredd på fönstret
        app_Hei,            // Höjd på fönstret
        NULL,               //Hanterare till parent-window
        NULL,               //Ingen meny (än)
        hInstance,          //Programmets instance hanterare
        NULL                //Ingen Window Creation Data
    );

    if (!hWnd) {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
//------------------------------------------------------------------------------------
LRESULT CALLBACK winProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    HDC dc;
    PAINTSTRUCT ps;
    static std::string text = "1       2      3        4       5       6      7       8      9      10     11     12     13    14    15     16     17     18     19     20     21     22    23    24     25     26     27";
    std::stringstream output;
    std::string tmp;
    static int x = 0 , y = 0, pen = 0, antal = 0;
    static bool click = false;
    bool no;
    static char win = 'N';
    int yeah, placement = 0;
    switch (Msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_CREATE:	                        // Körs endast en gång, vid skapandet av applikationen
            init(hWnd);    
            yeah = 0;
            for (int n = 0; n < 12; n++) {
                for (int i = 0; i < 27; i++) {
                    gridpos.push_back(pos());
                    gridpos[yeah].x = 35 * i + 35;
                    gridpos[yeah].y = 35 * n + 35;
                    yeah++;
                }
            }
            break;
        /*
        case WM_TIMER:		                    // Körs när en timerperiod fyllts
            
            if (wParam == 1) {	                // Den identifierare som satts på den timern
                pen = ++pen % 10;
                SelectObject(hdc, myPens[pen]);
            }
      
            break;
        */
        case WM_MOUSEMOVE:
            output << (x = LOWORD(lParam)) << ", " << (y = HIWORD(lParam));
            /*
            text = output.str();
            InvalidateRect(hWnd, NULL, false);
            UpdateWindow(hWnd);
            */
            break;
        case WM_LBUTTONDOWN:
            no = false; 
            for (int n = 0; n < 324; n++) {
                if (x - gridpos[n].x < 17.5 && y - gridpos[n].y < 17.5) {
                    x = gridpos[n].x; 
                    y = gridpos[n].y;

                    if (gridpos[n].taken == true) {
                        no = true;
                        break;
                    }
                    gridpos[n].taken = true;
                    gridpos[n].marker = 'A';
                    placement = n + 1;
                    gridpos[n].place = placement;
                    break;
                }
            }
            if (no == false) {
                kryss.push_back(pos());
                kryss[antal].x = x; 
                kryss[antal].y = y;
                kryss[antal].place = placement;
                kryss[antal].marker = 'X';
                /*
                if (wich == true) {
                    kryss[antal].marker = 'X';
                    wich = false;
                }
                else {
                    kryss[antal].marker = 'O';
                    wich = true;
                }
                */
                win = CheckWin(kryss[antal].marker);
                antal++;

            }
            InvalidateRect(hWnd, NULL, false);
            UpdateWindow(hWnd);
            break;
            
        case WM_RBUTTONDOWN:
            kryss.clear();
            antal = 0;
            for (int n = 0; n < 324; n++) {
                gridpos[n].taken = false;
            }
            for (int n = 0; n < kryss.size(); n++) {
                kryss[n].marker = 'N';
                kryss[n].place = -1;
            }
            wich = true;
            InvalidateRect(hWnd, NULL, false);
            UpdateWindow(hWnd);
            break;
            
        case WM_PAINT:
            dc = BeginPaint(hWnd, &ps);
            tmp += win;
            TextOut(dc, 30, 0, text.c_str(), lstrlen(text.c_str()));
            Grid(hWnd);
            for (int n = 0; n < kryss.size(); n++) {
                Print(kryss[n].x, kryss[n].y, kryss[n].marker);
            }
            EndPaint(hWnd, &ps);
            SetWindowText(edit, LPCSTR(tmp.c_str()));    // skriver ut det för användaren
            break;
        case WM_DESTROY:
            cleanUp(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}
//---------------------------------------------------------------------
void init(HWND hWnd) {
    srand(time(NULL));
    //SetTimer(hWnd, timerID, 100, NULL);
    hdc = GetDC(hWnd);
    /*random pen
    for (int i = 0; i < 10; i++) {
        myPens[i] = CreatePen(PS_SOLID, (rand() % 7 + 1),
            RGB(rand() % 256, rand() % 256, rand() % 256));
    }
    */
    myPens[0] = CreatePen(PS_SOLID, 5, RGB(4, 118, 208));
    myPens[1] = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
    orginalPen = (HPEN)SelectObject(hdc, myPens[0]);
}
//---------------------------------------------------------------------
void cleanUp(HWND hWnd) {
    KillTimer(hWnd, timerID);
    SelectObject(hdc, orginalPen);
    for (int i = 0; i < 10; i++) {
        DeleteObject(myPens[i]);
    }
    ReleaseDC(hWnd, hdc);
    PostQuitMessage(WM_QUIT);
}
//---------------------------------------------------------------------
void Marker(int x, int y)
{
    MoveToEx(hdc, x - 10, y -10, (LPPOINT)NULL);
    LineTo(hdc, x + 10, y + 10);
    MoveToEx(hdc, x +10, y - 10, (LPPOINT)NULL);
    LineTo(hdc, x - 10, y + 10);
}
//---------------------------------------------------------------------
void Circle(int x, int y)
{
    Arc(hdc, x - 15, y - 15, x + 15, y + 15, 0, 0, 0, 0);
}
//---------------------------------------------------------------------
void Grid(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    HBITMAP grid;
    grid = (HBITMAP)LoadImage(NULL, "gridMiddle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HBRUSH rBrush = CreatePatternBrush(grid);
    HBRUSH olBrush = (HBRUSH)SelectObject(hdc, rBrush);
    Rectangle(hdc, 17, 17, (app_Wid - 37), (app_Hei - 62));
    SelectObject(hdc, olBrush);
    DeleteObject(rBrush);
    DeleteObject(grid);
    ReleaseDC(hWnd, hdc);
}
//---------------------------------------------------------------------
void Print(int x, int y, char marker) {
    if (marker == 'X') {
        SelectObject(hdc, myPens[0]);
        Marker(x, y);
    }
    else if (marker == 'O'){
        SelectObject(hdc, myPens[1]);
        Circle(x, y);
    }
}
//---------------------------------------------------------------------
char CheckWin(char wich) {
    // GÖR 2 OLIKA EN VECTOR FÖR KRYSS POSITION EN FÖR CIRCLE POSITION
    char win = 'F';
    int tmppos;
    int langd = kryss.size() - 4;
    for (int n = 0; n < langd; n++) {
        tmppos = kryss[n].place;
        win = 'W';
        for (int i = 1; i < 4; i++) {
            if (kryss[tmppos + i].place != tmppos + i) {
                win = 'N';
                break;
            }
        }
        if (win == 'W') {
            return win;
        }

        /*
        for (int i = 1; i < 3; i++) {
            if (kryss[n + i].place == tmppos + 1) {
                win = 'Y';
            }
        }
        */
        /*if (kryss[n].marker == 'X') {
            tmppos = kryss[n].place;
            for (int i = 1; i < 5; i++) {
                win = 'X';
                if (kryss[n + i].place != tmppos + i || kryss[n + i].marker != 'X') {
                    win = 'N';
                    break;
                }
            }
        }*/
    }
    return win;
}
//---------------------------------------------------------------------
void createwin(HWND hWnd, HINSTANCE hInstance) {
    edit = CreateWindowEx(WS_EX_CLIENTEDGE, //border style för textbox
        (LPCSTR)"EDIT", (LPCSTR)"Start text", WS_VISIBLE | WS_CHILD | ES_RIGHT,
        2, 440, 300, 50, hWnd, (HMENU)100, hInstance, NULL);
}
//---------------------------------------------------------------------