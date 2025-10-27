//ヘッダーファイル
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <stdio.h>
#include <string.h>
// Define macros マクロの定義

#define V_MAX			(25)
#define H_MAX			(80)


// Define constants 定数の定義
const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 26;

const int direction_x[8] = { -1,-1,-1,0,0,1,1,1 };
const int direction_y[8] = { -1,0,1,-1,1,-1,0,1 };
void drawUi();
void SetCursorVisibility(BOOL visible);		// カーソルの表示/非表示を設定する関数
void MoveCursorToTop();		                // カーソルを画面の左上に移動する関数
void clearInputBuffer();                    // キーボード入力バッファのクリア関数
bool changeConsoleSize(int x, int y);       // コンソールサイズを変更する関数
bool CL11Startup();                         // コンソールの初期化関数
bool CL11Cleanup();                         // コンソールのクリーンアップ関数
void HideConsoleTitleBar();                 // コンソールのタイトルバーを隠す関数
void ShowConsoleTitleBar();                 // コンソールのタイトルバーを表示する関数
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY); // コンソールフォントの設定関数
void centerConsoleWindow(int screenWidth, int screenHeight);    // コンソールウィンドウを中央に配置する関数
void EnableVTMode();						// VTシーケンスモードを有効化
void SetTextColorRGB(int r, int g, int b);	//コンソールウィンドウの表示色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b);	//コンソールウィンドウの背景色をRGB形式で変更
bool isValidPosition(int x, int y);

char ui[V_MAX + 1][H_MAX + 1] =
//  0         1         2         3
{//  01234567890123456789012345678901234567890123456789012345678901234567890123456789
	"[                                                                              ]",//0
	"                                                                                ",//1
	"                                                                                ",//2
	"                                                                                ",//3
	"                                                                                ",//4
	"                                                                                ",//5
	"                                                                                ",//6
	"                                                                                ",//7
	"                                                                                ",//8
	"                                                                                ",//9
	"                                                                                ",//0
	"                                                                                ",//1
	"                                                                                ",//2
	"                                                                                ",//3
	"                                                                                ",//4
	"                                                                                ",//5
	"                                                                                ",//6
	"                                                                                ",//7
	"                                                                                ",//8
	 "                                                                               ",//9
	"                                                                                ",//0
	"                                                                                ",//1
	"                                                                                ",//2
	"                                                                                ",//3
	"[                                                                              ]",//4
};

char title[19][80] = {
	"==============================================================================\0",
	" _______  ___   ___      _______  _______  __   __  _______  _______  __   __ \0",
	"|       ||   | |   |    |       ||   _   ||  |_|  ||       ||       ||  | |  |\0",
	"|    ___||   | |   |    |    ___||  |_|  ||       ||    ___||  _____||  |_|  |\0",
	"|   | __ |   | |   |    |   | __ |       ||       ||   |___ | |_____ |       |\0",
	"|   ||  ||   | |   |___ |   ||  ||       ||       ||    ___||_____  ||       |\0",
	"|   |_| ||   | |       ||   |_| ||   _   || ||_|| ||   |___  _____| ||   _   |\0",
	"|_______||___| |_______||_______||__| |__||_|   |_||_______||_______||__| |__|\0",
	"             _______  _______  __   __  _______  ______    __    _            \0",
	"            |       ||   _   ||  | |  ||       ||    _ |  |  |  | |           \0",
	"            |_     _||  |_|  ||  |_|  ||    ___||   | ||  |   |_| |           \0",
	"              |   |  |       ||       ||   |___ |   |_||_ |       |           \0",
	"              |   |  |       ||       ||    ___||    __  ||  _    |           \0",
	"              |   |  |   _   | |     | |   |___ |   |  | || | |   |           \0",
	"              |___|  |__| |__|  |___|  |_______||___|  |_||_|  |__|           \0",
	"                                                                              \0",
	"==============================================================================\0",
	"                      - G I L G A M E S H ' S   T A V E R N -                 \0",
	"==============================================================================\0",
};

int main() {
	if (!CL11Startup()) {
		perror("CL11Startup error");
		return 0;
	}
	drawUi();
	rewind(stdin);(void)getchar();
	system("cls");
	return 0;
}

bool isValidPosition(int x, int y) {
	return x >= 0 && x < H_MAX && y >= 0 && y < V_MAX;
}

void drawUi() {
	MoveCursorToTop();
	for (int i = 0; i < V_MAX; ++i)
		std::cout << ui[i] << std::endl;
}

void SetCursorVisibility(BOOL visible) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// カーソルを先頭に戻す
void MoveCursorToTop() {
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// コンソールの初期化
bool CL11Startup() {
	// Change console size コンソールサイズの変更
	if (!changeConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT)) {
		return FALSE;
	}
	// コンソールのタイトルバーを隠す
	HideConsoleTitleBar();

	// 画面の幅と高さを取得
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(0, screen);

	// コンソールフォントの設定
	setConsoleFont(CONSOLE_WIDTH, CONSOLE_HEIGHT, screenWidth, screenHeight, dpiX, dpiY);
	// Center the console window コンソールウィンドウを中央に配置
	centerConsoleWindow(screenWidth, screenHeight);
	return TRUE;
}

// コンソールフォントの設定
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY) {
	int fontSizeX = (screenWidth / width) * (dpiX / 96);
	int fontSizeY = (screenHeight / height) * (dpiY / 96);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 0}, FF_DONTCARE, FW_NORMAL, L"MS Gothic" };
	DISPLAY_DEVICE dd = { sizeof(dd) };
	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm = { sizeof(dm) };
	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	// フォントサイズの設定
	if (dm.dmPelsHeight > dm.dmPelsWidth) {
		cfi.dwFontSize.X = static_cast<SHORT>(fontSizeX);
		cfi.dwFontSize.Y = static_cast<SHORT>(floor(fontSizeX * 2));
	}
	else {
		cfi.dwFontSize.X = static_cast<SHORT>(floor(fontSizeY / 2));
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSizeY);
	}

	// コンソールフォントの適用
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width) - 1, static_cast<SHORT>(height) - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

// コンソールウィンドウを中央に配置する関数
void centerConsoleWindow(int screenWidth, int screenHeight) {
	HWND consoleWindow = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleWindow, &rect);
	MoveWindow(consoleWindow, (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// コンソールのクリーンアップ関数
bool CL11Cleanup() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Set console mode コンソールモードの設定
	SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);

	// Set console buffer size コンソールバッファサイズの設定
	COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	SetConsoleScreenBufferSize(hOut, bufferSize);
	CONSOLE_CURSOR_INFO cursorInfo = { 25, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);
	SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 16}, FF_DONTCARE, FW_NORMAL, L"Consolas" };
	SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	// Reset console window position コンソールウィンドウの位置をリセット
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowConsoleTitleBar();

	return TRUE;
}

// コンソールサイズを変更する関数
bool changeConsoleSize(int x, int y) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SMALL_RECT rectConsoleSize = { 0, 0, 1, 1 };

	// Temporarily reduce console window size コンソールウィンドウサイズの一時的な縮小
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);
	if (!SetConsoleScreenBufferSize(hStdout, coord)) {
		return FALSE;
	}
	rectConsoleSize.Right = x - 1;
	rectConsoleSize.Bottom = y - 1;
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);

	return TRUE;
}

// コンソールのタイトルバーを隠す関数
void HideConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// コンソールのタイトルバーを表示する関数
void ShowConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// キーボード入力バッファのクリア関数
void clearInputBuffer() {
	while (_kbhit()) {
		_getch();
	}
}

// VTシーケンスモードを有効化
void EnableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

// コンソールウィンドウの表示色をRGB形式で変更
void SetTextColorRGB(int r, int g, int b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

// コンソールウィンドウの背景色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
}