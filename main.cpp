#include <Windows.h>
#include <conio.h>
#include <vector>
using namespace std;

enum EState {
	Init,
	Playing,
	Dead,
};

struct Vec2 {
	int x, y;
};

const int width = 60;
const int height = 28;

static char board[height][width];
static EState state;
static Vec2 pos;
static Vec2 dir;
static vector<Vec2> prevpositions;
static int nApples = 0;
static Vec2 applepos;

bool touchedTail( Vec2 newpos ) {
	for ( int i = 0; i < nApples; ++i ) {
		if ( i >= prevpositions.size() ) {
			break;
		}

		if ( newpos.x == prevpositions[prevpositions.size() - 1 - i].x && newpos.y == prevpositions[prevpositions.size() - 1 - i].y ) {
			return true;
		}
	}

	return false;
}

void spawnSnake() {
	pos = { width / 2, height / 2 };
	dir = { 0, -1 };
	nApples = 0;
	prevpositions.clear();
}

void spawnApple() {
	applepos.x = 2 + (rand() % width - 3);
	applepos.y = 2 + (rand() % height - 3);
}

void StartPlaying() {
	spawnSnake();
	spawnApple();
	state = Playing;
}

void checkTouchApple( Vec2 newpos ) {
	printf( "(%d, %d) (%d, %d)\n", newpos.x, newpos.x, applepos.x, applepos.y );
	if ( newpos.x == applepos.x && newpos.y == applepos.y ) {
		++nApples;

		spawnApple();
	}
}

void Draw() {
	system( "cls" );

	switch ( state ) {
	case Init:
	{
		printf( "Press space to play!\n" );
		break;
	}
	case Playing:
	{
		memset( board, ' ', sizeof( board ) );

		char directionChar = '^';
		if ( dir.y == -1 ) { directionChar = '^'; }
		else if ( dir.y == 1 ) { directionChar = 'v'; }
		else if ( dir.x == -1 ) { directionChar = '<'; }
		else if ( dir.x == 1 ) { directionChar = '>'; }
		board[pos.y][pos.x] = directionChar;

		for ( int i = 0; i < nApples; ++i ) {
			if ( i >= prevpositions.size() ) {
				break;
			}
			else {
				board[prevpositions[prevpositions.size() - 2 - i].y][prevpositions[prevpositions.size() - 2 - i].x] = '0';
			}
		}

		board[applepos.y][applepos.x] = '@';

		for ( int i = 0; i < width; ++i ) {
			board[0][i] = '=';
			board[height - 1][i] = '=';
		}

		for ( int i = 0; i < height; ++i ) {
			board[i][0] = '|';
			board[i][width - 2] = '|';
			board[i][width - 1] = '\n';
		}
		board[height - 1][width - 1] = '\0';
		printf( "%s\n", board );
		break;
	}
	case Dead:
		printf( "You have died of dysentery.\n" );
		printf( "On the bright side you collected %d apples!\n\n", nApples );
		printf( "Press space to play again!\n" );
		break;
	}
}

int main() {
	while ( true ) {
		bool bSpacePressed = false;
		bool bEscPressed = false;
		if ( _kbhit() ) {
			switch ( _getch() ) {
			case 27: // esc
			{
				bEscPressed = true;
				break;
			}
			case ' ':
			{
				bSpacePressed = true;
				break;
			}
			case 224: // Escape sequence
			{
				switch ( _getch() ) {
				case 72: // Up
					if ( dir.y != 1 ) { dir = { 0, -1 }; }
					break;
				case 80: // Down
					if ( dir.y != -1 ) { dir = { 0, 1 }; }
					break;
				case 75: // Left
					if ( dir.x != 1 ) { dir = { -1, 0 }; }
					break;
				case 77: // Right
					if ( dir.x != -1 ) { dir = { 1, 0 }; }
					break;
				}
				break;
			}
			}
		}

		if ( bEscPressed ) {
			break;
		}

		switch ( state ) {
		case Init:
		{
			if ( bSpacePressed ) {
				StartPlaying();
			}
			break;
		}
		case Playing:
		{
			Vec2 newpos = { pos.x += dir.x, pos.y += dir.y };

			if ( newpos.x == 0 || newpos.y == 0 || newpos.x == width - 2 || newpos.y == height - 2 || touchedTail( newpos ) ) {
				state = Dead;
				break;
			}

			checkTouchApple( newpos );

			prevpositions.push_back( pos );
			pos = newpos;

			break;
		}
		case Dead:
		{
			if ( bSpacePressed ) {
				StartPlaying();
			}
			break;
		}
		}

		Draw();
		Sleep( 125 );
	}

	return 0;
}