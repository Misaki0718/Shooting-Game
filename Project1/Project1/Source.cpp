#include "DxLib.h"
#include <vector>

using namespace std;


//�萔�̐錾
const int enemyScore = 100;
const int itemScore = 500;
//Window�̃f�t�H���g�T�C�Y
const int windowX = 640;
const int windowY = 480;

//�L�����N�^�[��{�N���X
class Character {
protected:
	//�e�L�����N�^�[�̉摜
	int image;
	//�L�����N�^�[�̕�
	int width;
	//�L�����N�^�[�̍���
	int height;
	//���W
	int x, y;
	//�ړ����x
	int xSpeed, ySpeed;
public:
	Character(const char *name, int xSpeed, int ySpeed) {
		image = LoadGraph(name);
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		GetGraphSize(image, &width, &height);
	}
	//�������W��ݒ肷��
	void setPosition(int, int);
	//�ړ�
	virtual void move() = 0;
	//�`��
	void draw();
	//x���W��Ԃ�
	int getX();
	//y���W��Ԃ�
	int getY();
	//�摜�̕���Ԃ�
	int getWidth();
	//�摜�̍�����Ԃ�
	int getHeight();
	//�����蔻��
	bool checkHit(Character*);
	virtual ~Character() {

	}
};

//�v���C���[�N���X
class Player : public Character {
public:
	Player() : Character("player.png", 8, 0){
	}
	void move();
	~Player() {

	}
};

//�e�N���X
class Bullet : public Character {
public:
	Bullet() : Character("bullet.png", 0, 16) {

	}
	void move();
	~Bullet() {

	}
};

//�G��{�N���X
class Enemy : public Character {
public:
	Enemy(const char *name,int xs, int ys) : Character(name, xs, ys){

	}
	void move();
};

//�GA�N���X
class EnemyA : public Enemy {
public:
	EnemyA() : Enemy("enemyA.png", 5, 3) {

	}
	~EnemyA() {

	}
};

//�GB�N���X
class EnemyB : public Enemy {
public:
	EnemyB() : Enemy("enemyB.png", -4, 6) {

	}
	~EnemyB() {

	}
};

//�GC�N���X
class EnemyC : public Enemy {
public:
	EnemyC() : Enemy("enemyC.png", 0, 4) {

	}
	~EnemyC() {

	}
};

//�A�C�e���N���X
class Item : public Character {
private:
	int itemSpeed;
public:
	Item() : Character("item.png", 0, 8) {

	}
	void move();
};

//�Q�[���R���g���[���[�N���X
class GameController {
private:
	//�X�^�[�g��ʗp�̉摜
	int startImage;
	//�Q�[���I�[�o�[�p�̉摜
	int gOverImage;
	//�G���X�g
	vector <Enemy*> enemyList;
	//�v���C���[
	Player *player;
	//�e
	Bullet *bullet;
	//�A�C�e��
	Item *item;
	//�G�����^�C�}�[
	int enemyCount;
	//�e��ł��Ă��邩�̔���
	bool shotBullet;
	//�A�C�e�������^�C�}�[
	int itemCount;
	//�A�C�e�����o�����Ă��邩�̔���
	bool appearItem;
	//�v���C���[�̐�������
	bool dead;
	//�X�R�A
	int score;
	//�t�H���g
	int startFont;
	int gOverFont;
	int font;
public:
	GameController(){
		startImage = LoadGraph("gameStart.png");
		gOverImage = LoadGraph("gameOver.png");
		startFont = CreateFontToHandle("HG�n�p�p�|�b�v", 70, 10, DX_FONTTYPE_ANTIALIASING_EDGE);
		gOverFont = CreateFontToHandle("HG�n�p�p�|�b�v", 70, 10, DX_FONTTYPE_ANTIALIASING);
		font = CreateFontToHandle("MS �S�V�b�N", 25, 7, DX_FONTTYPE_NORMAL);
	}
	//�Q�[����������
	void gameInit();
	//�X�^�[�g���
	void gameStart();
	//�Q�[�����
	void gameMain();
	//�Q�[���I�[�o�[���
	void gameOver();
	void addScore(int);
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�Q�[����ʂ��E�B���h�E�\���ɂ���
	ChangeWindowMode(true);
	//DX���C�u�����̏�����
	if (DxLib_Init() == -1) {
		return -1;
	}
	//�w�i�F
	SetBackgroundColor(200, 255, 200);
	//�O���t�B�b�N�̕`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	GameController game = GameController();
	game.gameInit();
	game.gameStart();

	//DX���C�u�����I��
	DxLib_End();

	return 0;
}

//Character�N���X�̃����o�֐�
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

//Player�N���X�̃����o�֐�
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

//Bullet�N���X�̃����o�֐�
void Bullet::move() {
	y -= ySpeed;
}

//Enemy�N���X�̃����o�֐�
void Enemy::move() {
	x += xSpeed;
	y += ySpeed;
	if (x <= 0 || x >= windowX - width / 2) {
		this->xSpeed = -xSpeed;
	}
}

//Item�N���X�̃����o�֐�
void Item::move() {
	y += ySpeed;
}

//GameController�N���X�̃����o�֐�
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
	//����ʂɕ`���ꂽ�摜������
	ClearDrawScreen();
	DrawStringToHandle(130, 100, "Game start", GetColor(0, 0, 0), startFont);
	DrawStringToHandle(100, 250, "Please press a space key!", GetColor(0, 0, 0), font);
	DrawGraph(windowX / 4 * 3, windowY / 2, startImage, true);
	//����ʂ�\��ʂɓ]��
	ScreenFlip();
	WaitKey();
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		gameMain();
	}
}
void GameController::gameMain() {
	//BGM�𗬂�
	PlayMusic("bgm.wav", DX_PLAYTYPE_LOOP);
	//�E�B���h�E��������܂Ń��[�v
	while (!ProcessMessage()) {
		//����ʂɕ`���ꂽ�摜������
		ClearDrawScreen();
		
		//�G�𐶐�
		enemyCount--;
		if (enemyCount == 0) {
			//����x���W�̐���
			int random;
			Enemy *enemy;
			//�G�������_���Ő���
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
		//�A�C�e���𐶐�
		itemCount--;
		if (itemCount <= 0 && !appearItem) {
			item = new Item();
			int random = GetRand(windowX - item->getWidth());
			item->setPosition(random, -item->getHeight());
			appearItem = true;
			itemCount = 500;
		}

		//�e�𔭎�
		if (CheckHitKey(KEY_INPUT_SPACE) && !shotBullet) {
			//�e�𐶐�
			bullet = new Bullet();
			bullet->setPosition(player->getX() + player->getWidth() / 2, player->getY());
			shotBullet = true;
		}

		//�ړ�
		player->move();
		if (shotBullet) {
			bullet->move();
			//��ʂ���͂ݏo���������
			//����������y���W���e�ɓ�����Ȃ��悤�ɂ���
			if (bullet->getY() < -bullet->getHeight()) {
				shotBullet = false;
			}
		}
		if (appearItem) {
			item->move();
			//��ʂ���͂ݏo���������
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

		//�����蔻��
		for (unsigned int i = 0; i < enemyList.size(); i++) {
			//�v���C���[�ƓG
			if (enemyList[i]->checkHit(player)) {
				//�v���C���[�����S������
				dead = true;
				PlaySound("nekoDead.mp3", DX_PLAYTYPE_BACK);
			}
			//�e�ƓG
			if (shotBullet) {
				if (enemyList[i]->checkHit(bullet)) {
					//�e������
					shotBullet = false;
					//�G�����S������
					enemyList.erase(enemyList.begin() + i);
					i--;
					//���ʉ�
					PlaySound("neko.mp3", DX_PLAYTYPE_BACK);
					//�X�R�A�ǉ�
					addScore(enemyScore);
				}
			}
		}
		
		if (appearItem) {
			//�A�C�e���ƒe�̓����蔻��
			if (shotBullet && bullet->checkHit(item)) {
				appearItem = false;
				shotBullet = false;
				delete bullet;
				PlaySound("itemDead.mp3", DX_PLAYTYPE_BACK);
			}
			//�A�C�e���ƃv���C���[�̓����蔻��
			 if (player->checkHit(item)) {
				appearItem = false;
				addScore(itemScore);
				PlaySound("getItem.mp3", DX_PLAYTYPE_BACK);
			}
		}

		//�Q�[���I�[�o�[�̔���
		if (dead) {
			//BGM���~�߂�
			StopMusic();
			gameOver();
		}

		//�`��
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
		//�X�R�A��\��
		DrawFormatString(0, 0, GetColor(0, 20, 0), "Score:%d", score);
		//����ʂ�\��ʂɓ]��
		ScreenFlip();
	}
}
void GameController::gameOver() {
	//����ʂɕ`���ꂽ�摜������
	ClearDrawScreen();

	DrawStringToHandle(180, 100, "GameOver", GetColor(0, 0, 0), gOverFont);
	//�X�R�A�\��
	DrawFormatStringToHandle(100, 200, GetColor(0, 0, 0), font, "Score:%d", score);
	DrawStringToHandle(100, 250, "Please press a space key \n and play new game.", GetColor(0, 0, 0), font);
	DrawGraph(windowX / 4 * 3, windowY / 2, gOverImage, true);
	//����ʂ�\��ʂɓ]��
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
