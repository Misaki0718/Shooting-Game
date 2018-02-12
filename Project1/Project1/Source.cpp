#include "DxLib.h"
#include <vector>

using namespace std;


//定数の宣言
const int enemyScore = 100;
const int itemScore = 500;
//Windowのデフォルトサイズ
const int windowX = 640;
const int windowY = 480;

//キャラクター基本クラス
class Character {
protected:
	//各キャラクターの画像
	int image;
	//キャラクターの幅
	int width;
	//キャラクターの高さ
	int height;
	//座標
	int x, y;
	//移動速度
	int xSpeed, ySpeed;
public:
	Character(const char *name, int xSpeed, int ySpeed) {
		image = LoadGraph(name);
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		GetGraphSize(image, &width, &height);
	}
	//初期座標を設定する
	void setPosition(int, int);
	//移動
	virtual void move() = 0;
	//描画
	void draw();
	//x座標を返す
	int getX();
	//y座標を返す
	int getY();
	//画像の幅を返す
	int getWidth();
	//画像の高さを返す
	int getHeight();
	//当たり判定
	bool checkHit(Character*);
	virtual ~Character() {

	}
};

//プレイヤークラス
class Player : public Character {
public:
	Player() : Character("player.png", 8, 0){
	}
	void move();
	~Player() {

	}
};

//弾クラス
class Bullet : public Character {
public:
	Bullet() : Character("bullet.png", 0, 16) {

	}
	void move();
	~Bullet() {

	}
};

//敵基本クラス
class Enemy : public Character {
public:
	Enemy(const char *name,int xs, int ys) : Character(name, xs, ys){

	}
	void move();
};

//敵Aクラス
class EnemyA : public Enemy {
public:
	EnemyA() : Enemy("enemyA.png", 5, 3) {

	}
	~EnemyA() {

	}
};

//敵Bクラス
class EnemyB : public Enemy {
public:
	EnemyB() : Enemy("enemyB.png", -4, 6) {

	}
	~EnemyB() {

	}
};

//敵Cクラス
class EnemyC : public Enemy {
public:
	EnemyC() : Enemy("enemyC.png", 0, 4) {

	}
	~EnemyC() {

	}
};

//アイテムクラス
class Item : public Character {
private:
	int itemSpeed;
public:
	Item() : Character("item.png", 0, 8) {

	}
	void move();
};

//ゲームコントローラークラス
class GameController {
private:
	//スタート画面用の画像
	int startImage;
	//ゲームオーバー用の画像
	int gOverImage;
	//敵リスト
	vector <Enemy*> enemyList;
	//プレイヤー
	Player *player;
	//弾
	Bullet *bullet;
	//アイテム
	Item *item;
	//敵生成タイマー
	int enemyCount;
	//弾を打っているかの判定
	bool shotBullet;
	//アイテム生成タイマー
	int itemCount;
	//アイテムが出現しているかの判定
	bool appearItem;
	//プレイヤーの生死判定
	bool dead;
	//スコア
	int score;
	//フォント
	int startFont;
	int gOverFont;
	int font;
public:
	GameController(){
		startImage = LoadGraph("gameStart.png");
		gOverImage = LoadGraph("gameOver.png");
		startFont = CreateFontToHandle("HG創英角ポップ", 70, 10, DX_FONTTYPE_ANTIALIASING_EDGE);
		gOverFont = CreateFontToHandle("HG創英角ポップ", 70, 10, DX_FONTTYPE_ANTIALIASING);
		font = CreateFontToHandle("MS ゴシック", 25, 7, DX_FONTTYPE_NORMAL);
	}
	//ゲーム準備処理
	void gameInit();
	//スタート画面
	void gameStart();
	//ゲーム画面
	void gameMain();
	//ゲームオーバー画面
	void gameOver();
	void addScore(int);
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ゲーム画面をウィンドウ表示にする
	ChangeWindowMode(true);
	//DXライブラリの初期化
	if (DxLib_Init() == -1) {
		return -1;
	}
	//背景色
	SetBackgroundColor(200, 255, 200);
	//グラフィックの描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	GameController game = GameController();
	game.gameInit();
	game.gameStart();

	//DXライブラリ終了
	DxLib_End();

	return 0;
}

//Characterクラスのメンバ関数
void Character::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}
void Character::draw() {
	DrawGraph(x, y, image, true);
}
int Character::getX() {
	return x;
}
int Character::getY() {
	return y;
}
int Character::getWidth() {
	return width;
}
int Character::getHeight() {
	return height;
}
bool Character::checkHit(Character *c) {
	if (c->getX() < x + width / 2 && x + width / 2 < c->getX() + c->width / 2 || c->getX() < x && x  < c->getX() + c->width / 2) {
		if (c->getY() < y + height / 2 && y + height / 2 < c->getY() + c->height / 2 || c->getY() < y && y  < c->getY() + c->height / 2) {
			return true;
		}
	}
	return false;
}

//Playerクラスのメンバ関数
void Player::move() {
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		x -= xSpeed;
		if (x <= -width / 2) {
			x = -width / 2;
		}
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		x += xSpeed;
		if (x >= windowX - width / 2) {
			x = windowX - width / 2;
		}
	}
}

//Bulletクラスのメンバ関数
void Bullet::move() {
	y -= ySpeed;
}

//Enemyクラスのメンバ関数
void Enemy::move() {
	x += xSpeed;
	y += ySpeed;
	if (x <= 0 || x >= windowX - width / 2) {
		this->xSpeed = -xSpeed;
	}
}

//Itemクラスのメンバ関数
void Item::move() {
	y += ySpeed;
}

//GameControllerクラスのメンバ関数
void GameController::gameInit() {
	player = new Player();
	player->setPosition(250, 400);
	enemyList.clear();
	enemyCount = 30;
	shotBullet = false;
	itemCount = 500;
	appearItem = false;
	dead = false;
	score = 0;
}
void GameController::gameStart() {
	//裏画面に描かれた画像を消去
	ClearDrawScreen();
	DrawStringToHandle(130, 100, "Game start", GetColor(0, 0, 0), startFont);
	DrawStringToHandle(100, 250, "Please press a space key!", GetColor(0, 0, 0), font);
	DrawGraph(windowX / 4 * 3, windowY / 2, startImage, true);
	//裏画面を表画面に転送
	ScreenFlip();
	WaitKey();
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		gameMain();
	}
}
void GameController::gameMain() {
	//BGMを流す
	PlayMusic("bgm.wav", DX_PLAYTYPE_LOOP);
	//ウィンドウが閉じられるまでループ
	while (!ProcessMessage()) {
		//裏画面に描かれた画像を消去
		ClearDrawScreen();
		
		//敵を生成
		enemyCount--;
		if (enemyCount == 0) {
			//初期x座標の生成
			int random;
			Enemy *enemy;
			//敵をランダムで生成
			switch (GetRand(2)) {
			case 0: enemy = new EnemyA();
					random = GetRand(windowX - enemy->getWidth());
					enemy->setPosition(random, -enemy->getHeight());
					enemyList.push_back(enemy); 
					break;
			case 1: enemy = new EnemyB();
					random = GetRand(windowX - enemy->getWidth());
					enemy->setPosition(random, -enemy->getHeight());
					enemyList.push_back(enemy);
					break;
			case 2: enemy = new EnemyC();
					random = GetRand(windowX - enemy->getWidth());
					enemy->setPosition(random, -enemy->getHeight());
					enemyList.push_back(enemy);
					break;
			default: break;
			}
			enemyCount = 30;
		}
		//アイテムを生成
		itemCount--;
		if (itemCount <= 0 && !appearItem) {
			item = new Item();
			int random = GetRand(windowX - item->getWidth());
			item->setPosition(random, -item->getHeight());
			appearItem = true;
			itemCount = 500;
		}

		//弾を発射
		if (CheckHitKey(KEY_INPUT_SPACE) && !shotBullet) {
			//弾を生成
			bullet = new Bullet();
			bullet->setPosition(player->getX() + player->getWidth() / 2, player->getY());
			shotBullet = true;
		}

		//移動
		player->move();
		if (shotBullet) {
			bullet->move();
			//画面からはみ出したら消す
			//消えた時のy座標が弾に当たらないようにする
			if (bullet->getY() < -bullet->getHeight()) {
				shotBullet = false;
			}
		}
		if (appearItem) {
			item->move();
			//画面からはみ出したら消す
			if (item->getY() > windowY) {
				appearItem = false;
			}
		}
		for (unsigned int i = 0; i < enemyList.size(); i++) {
			enemyList[i]->move();
			if (enemyList[i]->getY() > windowY) {
				enemyList.erase(enemyList.begin() + i);
				i--;
			}
		}

		//当たり判定
		for (unsigned int i = 0; i < enemyList.size(); i++) {
			//プレイヤーと敵
			if (enemyList[i]->checkHit(player)) {
				//プレイヤーを死亡させる
				dead = true;
				PlaySound("nekoDead.mp3", DX_PLAYTYPE_BACK);
			}
			//弾と敵
			if (shotBullet) {
				if (enemyList[i]->checkHit(bullet)) {
					//弾を消す
					shotBullet = false;
					//敵を死亡させる
					enemyList.erase(enemyList.begin() + i);
					i--;
					//効果音
					PlaySound("neko.mp3", DX_PLAYTYPE_BACK);
					//スコア追加
					addScore(enemyScore);
				}
			}
		}
		
		if (appearItem) {
			//アイテムと弾の当たり判定
			if (shotBullet && bullet->checkHit(item)) {
				appearItem = false;
				shotBullet = false;
				delete bullet;
				PlaySound("itemDead.mp3", DX_PLAYTYPE_BACK);
			}
			//アイテムとプレイヤーの当たり判定
			 if (player->checkHit(item)) {
				appearItem = false;
				addScore(itemScore);
				PlaySound("getItem.mp3", DX_PLAYTYPE_BACK);
			}
		}

		//ゲームオーバーの判定
		if (dead) {
			//BGMを止める
			StopMusic();
			gameOver();
		}

		//描画
		player->draw();
		if (shotBullet) {
			bullet->draw();
		}
		if (appearItem) {
			item->draw();
		}
		for (unsigned int i = 0; i < enemyList.size(); i++) {
			enemyList[i]->draw();
		}
		//スコアを表示
		DrawFormatString(0, 0, GetColor(0, 20, 0), "Score:%d", score);
		//裏画面を表画面に転送
		ScreenFlip();
	}
}
void GameController::gameOver() {
	//裏画面に描かれた画像を消去
	ClearDrawScreen();

	DrawStringToHandle(180, 100, "GameOver", GetColor(0, 0, 0), gOverFont);
	//スコア表示
	DrawFormatStringToHandle(100, 200, GetColor(0, 0, 0), font, "Score:%d", score);
	DrawStringToHandle(100, 250, "Please press a space key \n and play new game.", GetColor(0, 0, 0), font);
	DrawGraph(windowX / 4 * 3, windowY / 2, gOverImage, true);
	//裏画面を表画面に転送
	ScreenFlip();
	WaitKey();
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		gameInit();
		gameMain();
	}
}
void GameController::addScore(int s) {
	score += s;
}
