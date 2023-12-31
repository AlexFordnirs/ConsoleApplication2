#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream> 
#include "Bullet.h"
#include "map.h"
#include "view.h"
#include <vector>
#include <sstream>
#include <string>

using namespace sf;
using namespace std;

///////////////////////////////////////////////////////////////////////////
int hit(int PlayerV, int PlayerG, int MobV, int MobG, int range)
{
	if ((PlayerV - MobV < range && PlayerV - MobV > -range) && (PlayerG - MobG < range && PlayerG - MobG > -range))
		return 1;
	else
		return 0;
}

int trap(int PlayerV, int PlayerG)
{
	PlayerV /= 32;
	PlayerG /= 32;
	if (TileMap[PlayerV][PlayerG] == 't')
	{
		TileMap[PlayerV][PlayerG] = 'b';
		return 1;
	}
	else
		return 0;
}

int doorScript(int PlayerV, int PlayerG)
{
	PlayerV /= 32;
	PlayerG /= 32;
	if (TileMap[PlayerV][PlayerG] == 's')
	{
		TileMap[PlayerV][PlayerG] == ' ';
		return 1;
	}
	return 0;
}

float distance(Vector2f p1, Vector2f p2)
{
	return sqrtf(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

int dif(int a, int b)
{
	int c = a - b;
	if (c < 0)
		c = c * -1;

	return c;
}

int MeleMob(int PlayerV, int PlayerG, int MobV, int MobG)
{

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);


	if (DifV < 20 && DifG < 16/*Ренджа до которой моб сокращает дистанцию*/)
	{
		return 1; // EBOSH

	}////////////////////////////////сокращаем дистанцию
	else
	{
		if (PlayerV - MobV > 10)
			return 22;//бег вниз
		else if (PlayerV - MobV < -10)
			return 11;//бег вверх
		else if (PlayerG - MobG > 10)
			return 33;//бег вправо
		else
			return 44;//бег влево
	}

}
int OrbitMob(int PlayerV, int PlayerG, int MobV, int MobG)
{
	const int orbit = 60; //внутренняя граница орбиты
	const int exorbit = 80;//внешняя граница орбиты (при зазоре менее 20 его жует)

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);

	if (DifV < exorbit && DifG < exorbit/*Ренджа до которой моб сокращает дистанцию*/)
	{
		if ((MobG - PlayerG) <= orbit && (MobV - PlayerV) <= -orbit)
		{
			return 33;//<<<<<<<<<<
		
		}
		else if ((MobG - PlayerG) >= orbit && (MobV - PlayerV) <= orbit)
		{
			return 22;// ♠♠♠♠♠♠♠♠
		
		}
		else if ((MobG - PlayerG) >= -orbit && (MobV - PlayerV) >= orbit)
		{
			return 44;//>>>>>>>>>>
		
		}
		else if ((MobG - PlayerG) <= -orbit && (MobV - PlayerV) >= -orbit)
		{
			return 11;//♥♥♥♥♥♥♥
		
		}
	}

	////////////////////////////////сокращаем дистанцию
	else
	{
		if (DifV > DifG)
		{
			if (PlayerV > MobV)
				return 22;//бег вниз
			else
				return 11;//бег вверх
		}
		else
		{
			if (PlayerG > MobG)
				return 33;//бег вправо
			else
				return 44;//бег влево
		}
		/*	if (PlayerV - MobV>10)
				return 22;//бег вниз
			else if (PlayerV - MobV <-10)
				return 11;//бег вверх
			else if (PlayerG - MobG >10)
				return 33;//бег вправо
			else
				return 44;//бег влево*/
	}
}
int MageMob(int PlayerV, int PlayerG, int MobV, int MobG)
{

	int DifV = dif(PlayerV, MobV);
	int DifG = dif(PlayerG, MobG);

	if (DifV < 160 && DifG < 160/*Ренджа до которой моб сокращает дистанцию*/)
	{
		if (DifV < 9 || DifG < 9)
		{
			if (DifV > DifG)
			{
				if (PlayerV > MobV)
					return 2;//выстрел вниз
				else
					return 1;//выстрел вверх
			}
			else
			{
				if (PlayerG > MobG)
					return 3;//выстрел вправо
				else
					return 4;//выстрел влево
			}

		}
		else {

			if (DifV > DifG)
			{
				if (PlayerG > MobG)
					return 33;//бег вправо
				else
					return 44;
			}
			else


			{
				if (PlayerV > MobV)
					return 22;//бег вверх
				else
					return 11;//бег вниз
			}
		}

	}////////////////////////////////сокращаем дистанцию
	else
	{
		if (PlayerV - MobV > 10)
			return 22;//бег вниз
		else if (PlayerV - MobV < -10)
			return 11;//бег вверх
		else if (PlayerG - MobG > 10)
			return 33;//бег вправо
		else
			return 44;//бег влево
	}
}




int moveCH(int obV, int obG, int door, int m)
{

	int jir = 32;

	obV += jir;
	obG += jir;

	obV /= 32;
	obG /= 32;


	if (m == 1 && (TileMap[obV - 1][obG] == '0' || (door == 1 && TileMap[obV - 1][obG] == '7')))
		return 0;
	if (m == 2 && (TileMap[obV + 1][obG] == '0' || (door == 1 && TileMap[obV + 1][obG] == '7')))
		return 0;
	if (m == 3 && (TileMap[obV][obG + 1] == '0' || (door == 1 && TileMap[obV][obG + 1] == '7')))
		return 0;
	if (m == 4 && (TileMap[obV][obG - 1] == '0' || (door == 1 && TileMap[obV][obG - 1] == '7')))
		return 0;

	return 1;
}

bool moveCHMob(Vector2f position)
{
	position.x /= 32;
	position.y /= 32;

	return TileMap[int(position.x)][int(position.y)] == ' ';
}


class Mob
{
public:
	int HP;
	float reload;
	Vector2f pos;
	Mob(int HP, Vector2f pos)
	{
		this->HP = HP;
		this->pos = pos;
		
	}

};

//  PlayerV = X           PlayerG = Y

/*class MeleMob {

public:
	Vector2f position;
	int HP;
	int DMG;
	int MS;

	MeleMob(Vector2f position, int HP, int DMG)
	{
		this->position = position;
		this->HP = HP;
		this->DMG = DMG;
	}

	void update(float time,Vector2f playerPosition)
	{
		if( distance(position,playerPosition) <= 20./*Ренджа до которой моб сокращает дистанцию*//*)
		{
			

		}////////////////////////////////сокращаем дистанцию
		else
		{
			if (playerPosition.y - position.y > 10)
				position.y += 0.001 * time;
			else if (playerPosition.y - position.y < -10)
				position.y -= 0.001 * time;
			else if (playerPosition.x - position.x > 10)
				position.x += 0.001 * time;
			else
				position.x -= 0.001 * time;
		}

	}
};*/

////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {

public:
	float x = 0, y = 0;
	float w, h, dx, dy, speed, reload;
	int dir;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;


	int health;
	int mana;
	bool life;

	Player(String F, float X, float Y, float W, float H) {
		dx = 0; dy = 0; speed = 0; dir = 0; health = 100;mana=100;
		//	dir = 0; speed = 0; playerScore = 0; health = 100;
		life = true;
		File = F;
		w = W; h = H;
		image.loadFromFile("img/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		reload = 0;
	}
	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx * time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой

		if (health <= 0) {
			life = false; speed = 0;
		}
		reload -= time / 1000;
		if (reload < 0)
			reload = 0;
	}

	void setReload(float reload)
	{
		if (reload > 0)
			this->reload = reload;
	}

	bool isReload()
	{
		return reload != 0;
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}
			}
	}



	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

};

int main()
{
	int QQ = 0;
	int WW = 0;
	int EE = 0;
	int RR = 0;
	int TT = 0;
	int YY = 0;
	int UU = 0;
	int II = 0;
	int OO = 0;
	int PP = 0;
	int AA = 0;
	int SS = 0;
	int DD = 0;
	int FF = 0;
	int GG = 0;
	int HH = 0;
	int JJ = 0;
	int KK = 0;
	int LL = 0;
	int ZZ = 0;
	int XX = 0;
	int CC = 0;
	int VV = 0;


	float xx1=0,hh,g;
	int bldSwitcher = 0;
	int bldSize = 100;
	int rng = 75;
	int switcher1 = 1;
	int switcher2 = -1;
	int switcher3 = 1;
	int switcher4 = 1;
	int door = 0;
	int mob_left = 0;
	////Text////

	//Font font;//shrift
	Font font;//шрифт 
	font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 25);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
//	text.setColor(Color::Green);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(Text::Bold);//жирный текст.

	////
	int X3 = 50, Y3 = 50;
	int bld = 320; //на сколько далеко моб блядует
	const double mspeed = 0.3;
	RenderWindow window(sf::VideoMode(1920, 1080), "Game");
	Image mage_img;

	Texture mage_texture;
	////34234
	//sf::RectangleShape shape1(sf::Vector2f(50, 100));
	CircleShape shape1(10);
	shape1.setFillColor(Color(0xff,0x0, 0x0));
	////23423
	shape1.setPosition({ 0,0 });

	view.reset(sf::FloatRect(0, 0, 1280, 720));

	Image map_image;
	map_image.loadFromFile("img/mapp.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	////


	////

	Player p("hero.png", 250, 250, 40.0, 50.0);

	mage_img.loadFromFile("mage1/SpVisp_00.png");
	mage_texture.loadFromImage(mage_img);

	//Sprite hero_sprite;
	//hero_sprite.setTexture(hero_texture);
	Sprite mage_sprite;
	mage_sprite.setTexture(mage_texture);

	//hero_sprite.setPosition(X, Y);
	//hero_sprite.setTextureRect(IntRect(0, 210, 90, 80));
	mage_sprite.setPosition(X3, Y3);
	mage_sprite.setTextureRect(IntRect(20, 160, 64, 64));

	
	//death
	Image death1_img;
	//Mob m;
	Texture death1_texture;
	death1_img.loadFromFile("mage1/death1.png");
	death1_texture.loadFromImage(death1_img);
	Sprite death1_sprite;
	death1_sprite.setTexture(death1_texture);
	death1_sprite.setPosition(0, 0);
	death1_sprite.setTextureRect(IntRect(20, 16, 64, 64));
	//

	//myle
	Image myle_img;

	Texture myle_texture;
	myle_img.loadFromFile("mage1/Sp_00.png");
	myle_texture.loadFromImage(myle_img);
	Sprite myle_sprite;
	myle_sprite.setTexture(myle_texture);
	myle_sprite.setPosition(0, 0);
	myle_sprite.setTextureRect(IntRect(20, 16, 64, 64));
	//

	//male
	Image male_img;

	Texture male_texture;
	male_img.loadFromFile("mage1/0.png");
	male_texture.loadFromImage(male_img);
	Sprite male_sprite;
	male_sprite.setTexture(male_texture);
	male_sprite.setPosition(0, 0);
	male_sprite.setTextureRect(IntRect(20, 16, 64, 64));
	//mag
	Image mag_img;

	Texture mag_texture;
	mag_img.loadFromFile("mage1/Spmag_00.png");
	mag_texture.loadFromImage(mag_img);
	Sprite mag_sprite;
	mag_sprite.setTexture(mag_texture);

	//hero_sprite.setPosition(X, Y);
	//hero_sprite.setTextureRect(IntRect(0, 210, 90, 80));
	mag_sprite.setPosition(0, 0);
	mag_sprite.setTextureRect(IntRect(20, 16, 64, 64));
	//*
	
	Sprite mele_sprite;
	mele_sprite.setTexture(mage_texture);

	mele_sprite.setPosition(0, 0);
	mele_sprite.setTextureRect(IntRect(20, 16, 64, 64));

	////
	Image mage1_img;
	Texture mage1_texture;
	mage1_img.loadFromFile("img/Untitled-6.png");
	mage1_texture.loadFromImage(mage1_img);
	Sprite mage1_sprite;
	mage1_sprite.setTexture(mage1_texture);
	mage1_sprite.setTextureRect(IntRect(9, 2, 29, 47));
	////
	vector<FloatRect> blocks;

	float CurrentFrame = 0;
	float CurrentFrame1 = 0;
	Clock clock;

	vector<Bullet> bullet;
	vector<Mob> orbitMobs;
	vector<Mob> MeleMobs;
	vector<Mob> MagMobs;
	vector<Mob> MalleMobs;
	vector<Mob> deathMobs;


	orbitMobs.push_back(Mob(200,Vector2f(100, 100)));
	MeleMobs.push_back(Mob(100, Vector2f(100, 100)));
	MalleMobs.push_back(Mob(90, Vector2f(100, 100)));
	MagMobs.push_back(Mob(50, Vector2f(100, 100)));
	deathMobs.push_back(Mob(50, Vector2f(100, 100)));


	while (window.isOpen())
	{
		blocks.clear();
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		if (time > 100)
			time = 0;

		if (p.mana > 100)
		{
			p.mana = 100;
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float coordinatePlayerX = 100, coordinatePlayerY =100;
		coordinatePlayerX = p.getplayercoordinateX();
		coordinatePlayerY = p.getplayercoordinateY();

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if (p.life == true) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				p.dir = 1; p.speed = 0.9;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;

				p.sprite.setTextureRect(IntRect(30 * int(CurrentFrame), 50, 40, 50));
			}

			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				p.dir = 0; p.speed = 0.9;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;

				p.sprite.setTextureRect(IntRect(30 * int(CurrentFrame) + 40, 50, -40, 50));
			}

			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				p.dir = 3; p.speed = 0.9;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;

				p.sprite.setTextureRect(IntRect(30 * int(CurrentFrame), 150, 40, 50));
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				p.dir = 2; p.speed = 0.9;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				p.sprite.setTextureRect(IntRect(30 * int(CurrentFrame), 0, 40, 50));

			}
		}
		getplayercoordinateforview(coordinatePlayerX, coordinatePlayerY);
		p.update(time);
		hh= coordinatePlayerX; g = coordinatePlayerY;
		
		///////////////////////////////////////////Управление Pulkoi////////////////////////////////////////////////////////////////////////
		if (p.life == true) {
			if (Keyboard::isKeyPressed(Keyboard::A)&&!p.isReload()) {
				bullet.push_back(Bullet(0.0, Vector2f(hh, g), Vector2f(-200, 0)));
				p.setReload(0.5);
			}

			if (Keyboard::isKeyPressed(Keyboard::D) && !p.isReload()) {
				bullet.push_back(Bullet(0.0, Vector2f(hh, g), Vector2f(200, 0)));
				p.setReload(0.5);
			}

			if (Keyboard::isKeyPressed(Keyboard::W) && !p.isReload()) {
				bullet.push_back(Bullet(0.0, Vector2f(hh, g), Vector2f(0, -200)));
				p.setReload(0.5);
			}

			if (Keyboard::isKeyPressed(Keyboard::S) && !p.isReload()) {
				bullet.push_back(Bullet(0.0, Vector2f(hh, g), Vector2f(0, 200)));
				p.setReload(0.5);
			}
			if (Keyboard::isKeyPressed(Keyboard::Q) && !p.isReload()) {
				if ((p.mana > 0)&&(p.health<=75)&&(p.mana<=100) && (p.health > 0))
			{
					p.mana -= 25;
					p.health += 25;
				}
				p.setReload(1.2);
			}
		}
//ROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM


		int cordchG = coordinatePlayerX;
		int cordchV = coordinatePlayerY;


		cordchG += 32;
		cordchV += 32;

		cordchG /= 32;
		cordchV /= 32;

		

		if (QQ == 0 && TileMap[cordchV][cordchG] == 'Q')
		{
			QQ = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			//mob_left = /*Укажи количество мобов, которое спавнишь*/ 1;

			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'q')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;

		}

	if (WW == 0 && TileMap[cordchV][cordchG] == 'W')
		{
			WW = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'w')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (EE == 0 && TileMap[cordchV][cordchG] == 'E')
		{
			EE = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'e')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (RR == 0 && TileMap[cordchV][cordchG] == 'R')
		{
			RR = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'r')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (TT == 0 && TileMap[cordchV][cordchG] == 'T')
		{
			TT = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 't')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (YY == 0 && TileMap[cordchV][cordchG] == 'Y')
		{
			YY = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'y')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (UU == 0 && TileMap[cordchV][cordchG] == 'U')
		{
			UU = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'u')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (II == 0 && TileMap[cordchV][cordchG] == 'I')
		{
			II = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'i')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (OO == 0 && TileMap[cordchV][cordchG] == 'O')
		{
			OO = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'o')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (PP == 0 && TileMap[cordchV][cordchG] == 'P')
		{
			PP = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'p')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (AA == 0 && TileMap[cordchV][cordchG] == 'A')
		{
			AA = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'a')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (SS == 0 && TileMap[cordchV][cordchG] == 'S')
		{
			SS = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 's')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (DD == 0 && TileMap[cordchV][cordchG] == 'D')
		{
			DD = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'd')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (FF == 0 && TileMap[cordchV][cordchG] == 'F')
		{
			FF = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'f')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (GG == 0 && TileMap[cordchV][cordchG] == 'G')
		{
			GG = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'g')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (HH == 0 && TileMap[cordchV][cordchG] == 'H')
		{
			HH = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'h')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (JJ == 0 && TileMap[cordchV][cordchG] == 'J')
		{
			JJ = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'j')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (KK == 0 && TileMap[cordchV][cordchG] == 'K')
		{
			KK = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'k')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (LL == 0 && TileMap[cordchV][cordchG] == 'L')
		{
			LL = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'l')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (ZZ == 0 && TileMap[cordchV][cordchG] == 'Z')
		{
			ZZ = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'z')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (XX == 0 && TileMap[cordchV][cordchG] == 'X')
		{
			XX = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'x')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (CC == 0 && TileMap[cordchV][cordchG] == 'C')
		{
			CC = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'c')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}

	if (VV == 0 && TileMap[cordchV][cordchG] == 'V')
		{
			VV = 1;
			//тута пиши для указанной комнаты, ЗАХОДИТ ТОЛЬКО 1 РАЗ
			door = 1;
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					if (TileMap[i][j] == 'v')
						orbitMobs.push_back(Mob(100, Vector2f(j * 32, i * 32)));
			if (orbitMobs.size() < 1 || MeleMobs.size() < 1 || MalleMobs.size() < 1 || MagMobs.size() < 1 || deathMobs.size() < 1)
				door = 0;
		}








	//suichidnik
	for (int i = 0; i < deathMobs.size(); i++)
	{
		switch (MeleMob(coordinatePlayerY, coordinatePlayerX, deathMobs[i].pos.y, deathMobs[i].pos.x))
		{
		case 1:
		{
			p.health -= 20;
			deathMobs[i].HP -= 100;
			break;
		}
		case 11:
		{
			if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 1)
				deathMobs[i].pos.y += -mspeed * time;
			else
			{
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 0)
					switcher1 = 1;
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 0)
					switcher1 = -1;



				if (switcher1 < 0 || (dif(coordinatePlayerY, deathMobs[i].pos.y) < rng && dif(coordinatePlayerX, deathMobs[i].pos.x) < rng))
				{
					if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 1)
						deathMobs[i].pos.x += mspeed * time;
				}
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 1)
					deathMobs[i].pos.x += -mspeed * time;
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 1)
					deathMobs[i].pos.y += mspeed * time;

			}
			break;
		}
		case 22:
		{
			if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 1)
				deathMobs[i].pos.y += mspeed * time;
			else
			{

				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 0)
					switcher2 = 1;
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 0)
					switcher2 = -1;




				if (switcher2 > 0 || (dif(coordinatePlayerY, deathMobs[i].pos.y) < rng && dif(coordinatePlayerX, deathMobs[i].pos.x) < rng))
				{
					if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 1)
						deathMobs[i].pos.x += -mspeed * time;
				}
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 1)
					deathMobs[i].pos.x += mspeed * time;
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 1)
					deathMobs[i].pos.y += -mspeed * time;
			}
			break;
		}
		case 33:
		{
			if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 1)
				deathMobs[i].pos.x += mspeed * time;
			else
			{
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 0)
					switcher3 = 1;
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 0)
					switcher3 = -1;



				if (switcher3 < 0 || (dif(coordinatePlayerY, deathMobs[i].pos.y) < rng && dif(coordinatePlayerX, deathMobs[i].pos.x) < rng))
				{
					if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 1)
						deathMobs[i].pos.y += mspeed * time;
				}
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 1)
					deathMobs[i].pos.y += -mspeed * time;
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 1)
					deathMobs[i].pos.x += -mspeed * time;
			}
			break;
		}
		case 44:
		{
			if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 4) == 1)
				deathMobs[i].pos.x += -mspeed * time;
			else
			{

				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 0)
					switcher4 = 1;
				if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 0)
					switcher4 = -1;


				if (switcher4 > 0 || (dif(coordinatePlayerY, deathMobs[i].pos.y) < rng && dif(coordinatePlayerX, deathMobs[i].pos.x) < rng))
				{
					if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 1) == 1)
						deathMobs[i].pos.y += -mspeed * time;
				}
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 2) == 1)
					deathMobs[i].pos.y += mspeed * time;
				else if (moveCH(deathMobs[i].pos.y, deathMobs[i].pos.x, door, 3) == 1)
					deathMobs[i].pos.x += mspeed * time;

			}
			break;
		}
		}
	}
	//range
	for (int i = 0; i < MagMobs.size(); i++)
	{
		switch (MageMob(coordinatePlayerY, coordinatePlayerX, MagMobs[i].pos.y, MagMobs[i].pos.x))
		{
		case 1:
		{
			if (!p.isReload())
		{
			p.setReload(3);
			p.health -= 15;
		}
		
			break;
		}
		case 2:
		{
			if (!p.isReload())
			{
				p.setReload(3);
				p.health -= 15;
			}
			break;

		}
		case 3:
		{
			if (!p.isReload())
			{
				p.setReload(3);
				p.health -= 15;
			}
			break;

		}
		case 4:
		{
			if (!p.isReload())
			{
				p.setReload(3);
				p.health -= 15;
			}
			break;

		}
		case 11:
		{
			if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 1)
				MagMobs[i].pos.y += -mspeed * time;
			else
			{
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 0)
					switcher1 = 1;
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 0)
					switcher1 = -1;



				if (switcher1 < 0 || (dif(coordinatePlayerY, MagMobs[i].pos.y) < rng && dif(coordinatePlayerX, MagMobs[i].pos.x) < rng))
				{
					if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 1)
						MagMobs[i].pos.x += mspeed * time;
				}
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 1)
					MagMobs[i].pos.x += -mspeed * time;
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 1)
					MagMobs[i].pos.y += mspeed * time;

			}
			break;
		}
		case 22:
		{
			if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 1)
				MagMobs[i].pos.y += mspeed * time;
			else
			{

				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 0)
					switcher2 = 1;
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 0)
					switcher2 = -1;




				if (switcher2 > 0 || (dif(coordinatePlayerY, MagMobs[i].pos.y) < rng && dif(coordinatePlayerX, MagMobs[i].pos.x) < rng))
				{
					if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 1)
						MagMobs[i].pos.x += -mspeed * time;
				}
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 1)
					MagMobs[i].pos.x += mspeed * time;
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 1)
					MagMobs[i].pos.y += -mspeed * time;
			}
			break;
		}
		case 33:
		{
			if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 1)
				MagMobs[i].pos.x += mspeed * time;
			else
			{
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 0)
					switcher3 = 1;
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 0)
					switcher3 = -1;



				if (switcher3 < 0 || (dif(coordinatePlayerY, MagMobs[i].pos.y) < rng && dif(coordinatePlayerX, MagMobs[i].pos.x) < rng))
				{
					if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 1)
						MagMobs[i].pos.y += mspeed * time;
				}
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 1)
					MagMobs[i].pos.y += -mspeed * time;
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 1)
					MagMobs[i].pos.x += -mspeed * time;
			}
			break;
		}
		case 44:
		{
			if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 4) == 1)
				MagMobs[i].pos.x += -mspeed * time;
			else
			{

				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 0)
					switcher4 = 1;
				if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 0)
					switcher4 = -1;


				if (switcher4 > 0 || (dif(coordinatePlayerY, MagMobs[i].pos.y) < rng && dif(coordinatePlayerX, MagMobs[i].pos.x) < rng))
				{
					if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 1) == 1)
						MagMobs[i].pos.y += -mspeed * time;
				}
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 2) == 1)
					MagMobs[i].pos.y += mspeed * time;
				else if (moveCH(MagMobs[i].pos.y, MagMobs[i].pos.x, door, 3) == 1)
					MagMobs[i].pos.x += mspeed * time;

			}
			break;
		}
		}
	}
	//mili2
	for (int i = 0; i < MalleMobs.size(); i++)
	{
		switch (MeleMob(coordinatePlayerY, coordinatePlayerX, MalleMobs[i].pos.y, MalleMobs[i].pos.x))
		{
		case 1:
		{
			if(!p.isReload())
			{
				p.setReload(1);
				p.health -= 4;
			}
			break;
		}
		
		case 11:
		{
			if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 1)
				MalleMobs[i].pos.y += -mspeed * time;
			else
			{
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 0)
					switcher1 = 1;
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 0)
					switcher1 = -1;



				if (switcher1 < 0 || (dif(coordinatePlayerY, MalleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MalleMobs[i].pos.x) < rng))
				{
					if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 1)
						MalleMobs[i].pos.x += mspeed * time;
				}
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 1)
					MalleMobs[i].pos.x += -mspeed * time;
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 1)
					MalleMobs[i].pos.y += mspeed * time;

			}
			break;
		}
		case 22:
		{
			if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 1)
				MalleMobs[i].pos.y += mspeed * time;
			else
			{

				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 0)
					switcher2 = 1;
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 0)
					switcher2 = -1;




				if (switcher2 > 0 || (dif(coordinatePlayerY, MalleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MalleMobs[i].pos.x) < rng))
				{
					if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 1)
						MalleMobs[i].pos.x += -mspeed * time;
				}
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 1)
					MalleMobs[i].pos.x += mspeed * time;
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 1)
					MalleMobs[i].pos.y += -mspeed * time;
			}
			break;
		}
		case 33:
		{
			if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 1)
				MalleMobs[i].pos.x += mspeed * time;
			else
			{
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 0)
					switcher3 = 1;
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 0)
					switcher3 = -1;



				if (switcher3 < 0 || (dif(coordinatePlayerY, MalleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MalleMobs[i].pos.x) < rng))
				{
					if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 1)
						MalleMobs[i].pos.y += mspeed * time;
				}
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 1)
					MalleMobs[i].pos.y += -mspeed * time;
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 1)
					MalleMobs[i].pos.x += -mspeed * time;
			}
			break;
		}
		case 44:
		{
			if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 4) == 1)
				MalleMobs[i].pos.x += -mspeed * time;
			else
			{

				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 0)
					switcher4 = 1;
				if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 0)
					switcher4 = -1;


				if (switcher4 > 0 || (dif(coordinatePlayerY, MalleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MalleMobs[i].pos.x) < rng))
				{
					if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 1) == 1)
						MalleMobs[i].pos.y += -mspeed * time;
				}
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 2) == 1)
					MalleMobs[i].pos.y += mspeed * time;
				else if (moveCH(MalleMobs[i].pos.y, MalleMobs[i].pos.x, door, 3) == 1)
					MalleMobs[i].pos.x += mspeed * time;

			}
			break;
		}
		}
	}
	//Mili1
	for (int i = 0; i < MeleMobs.size(); i++)
	{
		switch (MeleMob(coordinatePlayerY, coordinatePlayerX, MeleMobs[i].pos.y, MeleMobs[i].pos.x))
		{
		case 1:
		{
			if (!p.isReload())
			{
				p.setReload(1);
				p.health -= 2;
			}
			break;
		}
		
		case 11:
		{
			if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 1)
				MeleMobs[i].pos.y += -mspeed * time;
			else
			{
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 0)
					switcher1 = 1;
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 0)
					switcher1 = -1;



				if (switcher1 < 0 || (dif(coordinatePlayerY, MeleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MeleMobs[i].pos.x) < rng))
				{
					if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 1)
						MeleMobs[i].pos.x += mspeed * time;
				}
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 1)
					MeleMobs[i].pos.x += -mspeed * time;
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 1)
					MeleMobs[i].pos.y += mspeed * time;

			}
			break;
		}
		case 22:
		{
			if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 1)
				MeleMobs[i].pos.y += mspeed * time;
			else
			{

				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 0)
					switcher2 = 1;
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 0)
					switcher2 = -1;




				if (switcher2 > 0 || (dif(coordinatePlayerY, MeleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MeleMobs[i].pos.x) < rng))
				{
					if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 1)
						MeleMobs[i].pos.x += -mspeed * time;
				}
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 1)
					MeleMobs[i].pos.x += mspeed * time;
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 1)
					MeleMobs[i].pos.y += -mspeed * time;
			}
			break;
		}
		case 33:
		{
			if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 1)
				MeleMobs[i].pos.x += mspeed * time;
			else
			{
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 0)
					switcher3 = 1;
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 0)
					switcher3 = -1;



				if (switcher3 < 0 || (dif(coordinatePlayerY, MeleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MeleMobs[i].pos.x) < rng))
				{
					if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 1)
						MeleMobs[i].pos.y += mspeed * time;
				}
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 1)
					MeleMobs[i].pos.y += -mspeed * time;
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 1)
					MeleMobs[i].pos.x += -mspeed * time;
			}
			break;
		}
		case 44:
		{
			if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 4) == 1)
				MeleMobs[i].pos.x += -mspeed * time;
			else
			{

				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 0)
					switcher4 = 1;
				if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 0)
					switcher4 = -1;


				if (switcher4 > 0 || (dif(coordinatePlayerY, MeleMobs[i].pos.y) < rng && dif(coordinatePlayerX, MeleMobs[i].pos.x) < rng))
				{
					if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 1) == 1)
						MeleMobs[i].pos.y += -mspeed * time;
				}
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 2) == 1)
					MeleMobs[i].pos.y += mspeed * time;
				else if (moveCH(MeleMobs[i].pos.y, MeleMobs[i].pos.x, door, 3) == 1)
					MeleMobs[i].pos.x += mspeed * time;

			}
			break;
		}
		}
	}
	//Orbit
	for (int i = 0; i < orbitMobs.size(); i++)
	{
		switch (OrbitMob(coordinatePlayerY, coordinatePlayerX, orbitMobs[i].pos.y, orbitMobs[i].pos.x))
		{
		/*case 1:
		{
			if (!p.isReload())
			{
				p.setReload(1);
				p.health -= 1;
			}
			break;
		}*/
		case 11:
		{
			if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 1) {
				orbitMobs[i].pos.y += -mspeed * time;
			}
			else
			{
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 0)
					switcher1 = 1;
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 0)
					switcher1 = -1;



				if (switcher1 < 0 || (dif(coordinatePlayerY, orbitMobs[i].pos.y) < rng && dif(coordinatePlayerX, orbitMobs[i].pos.x) < rng))
				{
					if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 1)
						orbitMobs[i].pos.x += mspeed * time;
				}
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 1) {
					orbitMobs[i].pos.x += -mspeed * time;
				}

				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 1)
					orbitMobs[i].pos.y += mspeed * time;

			}
			break;
		}
		case 22:
		{
			if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 1) {
				orbitMobs[i].pos.y += mspeed * time;
			}
			else
			{

				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 0)
					switcher2 = 1;
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 0)
					switcher2 = -1;




				if (switcher2 > 0 || (dif(coordinatePlayerY, orbitMobs[i].pos.y) < rng && dif(coordinatePlayerX, orbitMobs[i].pos.x) < rng))
				{
					if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 1) {
						orbitMobs[i].pos.x += -mspeed * time;
					}
				}
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 1)
					orbitMobs[i].pos.x += mspeed * time;
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 1)
					orbitMobs[i].pos.y += -mspeed * time;
			}
			break;
		}
		case 33:
		{
			if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 1) {
				orbitMobs[i].pos.x += mspeed * time;
			}
			else
			{
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 0)
					switcher3 = 1;
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 0)
					switcher3 = -1;



				if (switcher3 < 0 || (dif(coordinatePlayerY, orbitMobs[i].pos.y) < rng && dif(coordinatePlayerX, orbitMobs[i].pos.x) < rng))
				{
					if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 1)
						orbitMobs[i].pos.y += mspeed * time;
				}
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 1)
					orbitMobs[i].pos.y += -mspeed * time;
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 1)
					orbitMobs[i].pos.x += -mspeed * time;
			}
			break;
		}
		case 44:
		{
			if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 4) == 1) {
				orbitMobs[i].pos.x += -mspeed * time;
			}
			else
			{

				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 0)
					switcher4 = 1;
				if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 0)
					switcher4 = -1;


				if (switcher4 > 0 || (dif(coordinatePlayerY, orbitMobs[i].pos.y) < rng && dif(coordinatePlayerX, orbitMobs[i].pos.x) < rng))
				{
					if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 1) == 1)
						orbitMobs[i].pos.y += -mspeed * time;
				}
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 2) == 1)
					orbitMobs[i].pos.y += mspeed * time;
				else if (moveCH(orbitMobs[i].pos.y, orbitMobs[i].pos.x, door, 3) == 1)
					orbitMobs[i].pos.x += mspeed * time;

			}
			break;
		}
		}
	}

	/*for (int i = 0; i < meleMobs.size(); i++)
	{
		switch ((OrbitMob(coordinatePlayerY, coordinatePlayerX, meleMobs[i].y, meleMobs[i].x)))
		{
		case 22:
			meleMobs[i].y -= 0.01 * time; break;
		case 11:
			meleMobs[i].y += 0.01 * time; break;
		case 44:
			meleMobs[i].x += 0.01 * time; break;
		case 33:
			meleMobs[i].x -= 0.01 * time; break;

		}


	}*/






		//ROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM

		window.setView(view);
		window.clear();

		if (doorScript(coordinatePlayerY, coordinatePlayerX) == 1)
			door = 1;



		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(230, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(197, 0, 32, 32));
				if ((TileMap[i][j] == 'Q'))s_map.setTextureRect(IntRect(197, 0, 32, 32));
				if ((TileMap[i][j] == 'm')) {


						/*switch (OrbitMob(coordinatePlayerY, coordinatePlayerX, mage_sprite.getPosition().y, mage_sprite.getPosition().x))
						{
						case 1:
						{

							mage_sprite.move(-0, 0.001 * time);
							break;
						}
						case 2:
						{
							mage_sprite.move(0, 0.001 * time);
							break;

						}
						case 3:
						{
							mage_sprite.move(0.001 * time, 0);
							break;

						}
						case 4:
						{
							mage_sprite.move(-0.001 * time, 0);
							break;

						}
						case 11:
						{
							if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
								mage_sprite.move(0, -mspeed * time);
							else
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 0)
									switcher1 = 1;
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 0)
									switcher1 = -1;



								if (switcher1 < 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
								{
									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
										mage_sprite.move(mspeed*time, 0);
								}
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
									mage_sprite.move(-mspeed * time, 0);
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
									mage_sprite.move(0, mspeed*time);

							}
							break;
						}
						case 22:
						{
							if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
								mage_sprite.move(0, mspeed*time);
							else
							{

									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 0)
										switcher2 = 1;
									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 0)
										switcher2 = -1;




								if (switcher2 > 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
								{
									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
										mage_sprite.move(-mspeed * time, 0);
								}
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
									mage_sprite.move(mspeed*time, 0);
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
									mage_sprite.move(0, -mspeed * time);
							}
							break;
						}
						case 33:
						{
							if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
								mage_sprite.move(mspeed*time, 0);
							else
							{

								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 0)
									switcher3 = 1;
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 0)
									switcher3 = -1;



								if (switcher3 < 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
								{
									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
										mage_sprite.move(0, mspeed*time);
								}
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
									mage_sprite.move(0, -mspeed * time);
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
									mage_sprite.move(-mspeed * time, 0);
							}
							break;
						}
						case 44:
						{
							if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 4) == 1)
								mage_sprite.move(-mspeed * time, 0);
							else
							{
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 0)
									switcher4 = 1;
								if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 0)
									switcher4 = -1;


								if (switcher4 > 0 || (dif(coordinatePlayerY, mage_sprite.getPosition().y) < rng && dif(coordinatePlayerX, mage_sprite.getPosition().x) < rng))
								{
									if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 1) == 1)
										mage_sprite.move(0, -mspeed * time);
								}
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 2) == 1)
									mage_sprite.move(0, mspeed*time);
								else if (moveCH(mage_sprite.getPosition().y, mage_sprite.getPosition().x, door, 3) == 1)
									mage_sprite.move(mspeed*time, 0);

							}
							break;
						}

				}*/
				////*	if (xx1 == 1)
				//	{
				//	
				//		/*if(h - coordinatePlayerX <50)
				//		{
				//			auto rect = make_unique<RectangleShape>(Vector2f(h, g));
				//			rect.reset();
				//		}
				//		shape1.move(-15, 0);*/
				//		CurrentFrame1 += 0.005*time;
				//		if (CurrentFrame1 > 5) CurrentFrame1 -= 5;
				//		mage1_sprite.setTextureRect(IntRect(295 -10 * int(CurrentFrame1), 9, -29, 47));
				//		mage1_sprite.move(-10, 0);
				//	}
				//	else if (xx1 == 2)
				//	{
				//		/*if (h + coordinatePlayerX > 70)
				//		{
				//			auto rect = make_unique<RectangleShape>(Vector2f(h, g));
				//			rect.reset();
				//		//	shape1.move(-15, 0);
				//		}
				//		else
				//		{ */
				//		//CurrentFrame1 = 0;
				//		//window.draw(shape1);
				//		CurrentFrame1 += 0.003*time;
				//		if (CurrentFrame1 > 5) CurrentFrame1 -= 5;
				//		mage1_sprite.setTextureRect(IntRect(295 -10 * int(CurrentFrame1), 9, 29, 47));
				//		//mage1_sprite.setTextureRect(IntRect()
				//		//shape1.move(+15, 0);
				//		mage1_sprite.move(+10, 0);
				//		//cout<<
				//	}
				//	else if (xx1 == 3)
				//	{
				//		/*if (g - coordinatePlayerY < 50)
				//		{
				//			auto rect = make_unique<RectangleShape>(Vector2f(h, g));
				//			rect.reset();
				//		}
				//		//window.draw(shape1);
				//		shape1.move(0, -15);*/
				//		CurrentFrame1 += 0.005*time;
				//		if (CurrentFrame1 > 5) CurrentFrame1 -= 5;
				//		mage1_sprite.setTextureRect(IntRect(295 -10 * int(CurrentFrame1), 5, 29, 47));
				//		mage1_sprite.move(0, -5);
				//	}
				//	else if (xx1 == 4)
				//	{
				//		/*if (g + coordinatePlayerY > 100)
				//		{
				//			auto rect = make_unique<RectangleShape>(Vector2f(h, g));
				//			rect.reset();
				//		}
				//		//window.draw(shape1);
				//		mage1_sprite.move(0, +15);*/

				//		CurrentFrame1 += 0.005*time;
				//		if (CurrentFrame1 > 5) CurrentFrame1 -= 5;
				//		mage1_sprite.setTextureRect(IntRect(295 -10 * int(CurrentFrame1), 5, 29, 47));
				//		mage1_sprite.move(0, +5);
				//	}
				//	//}
			
				}
				s_map.setPosition(j * 32, i * 32);
				//	if ((TileMap[i][j] == '0'))	blocks.push_back(s_map.getGlobalBounds());

			
				window.draw(s_map);
			}

/*
			for (int i = 0; i < bullet.size(); i++)
			{
				
				bullet[i].update(time/1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{
					
					window.draw(bullet[i].getSprite());
					for (int j = 0; j < orbitMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), orbitMobs[j].pos) < 20)
						{
							
							orbitMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}
					
				}

			}

			*/

////


			for (int i = 0; i < bullet.size(); i++)
			{

				bullet[i].update(time / 1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{

					window.draw(bullet[i].getSprite());
					for (int j = 0; j < MagMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), MagMobs[j].pos) < 40)
						{

							MagMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}
				}
			}
			for (int i = 0; i < bullet.size(); i++)
			{

				bullet[i].update(time / 1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{

					window.draw(bullet[i].getSprite());
					for (int j = 0; j < MalleMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), MalleMobs[j].pos) < 35)
						{

							MalleMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}
				}
			}
			for (int i = 0; i < bullet.size(); i++)
			{

				bullet[i].update(time / 1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{

					window.draw(bullet[i].getSprite());
					for (int j = 0; j < MeleMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), MeleMobs[j].pos) < 35)
						{

							MeleMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}
				}
			}
			for (int i = 0; i < bullet.size(); i++)
			{

				bullet[i].update(time / 1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{

					window.draw(bullet[i].getSprite());
					for (int j = 0; j < deathMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), deathMobs[j].pos) < 40)
						{

							deathMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}
				}
			}
			for (int i = 0; i < bullet.size(); i++)
			{

				bullet[i].update(time / 1000);
				if (bullet[i].getPassed() > 10000.)
				{
					bullet.erase(bullet.begin() + i);
					i--;
				}
				else
				{

					window.draw(bullet[i].getSprite());
					for (int j = 0; j < orbitMobs.size(); j++)
					{
						if (distance(bullet[i].getPosition(), orbitMobs[j].pos) < 30)
						{

							orbitMobs[j].HP -= 25;
							bullet.erase(bullet.begin() + i);
							i--;
							break;
						}
					}

				}

			}



			////


//0
			for (int i = 0; i < orbitMobs.size(); i++)
			{
				if (orbitMobs[i].HP <= 0)
				{
					orbitMobs.erase(orbitMobs.begin() + i);
					i--;
					if(p.mana < 100)
					p.mana += 15;
				}
				else
				{
					mele_sprite.setPosition(orbitMobs[i].pos);
					window.draw(mele_sprite);
				}
			}
			//1
			for (int i = 0; i < MagMobs.size(); i++)
			{
				if (MagMobs[i].HP <= 0)
				{
					MagMobs.erase(MagMobs.begin() + i);
					i--;
					if (p.mana < 100)
						p.mana += 15;
				}
				else
				{
					mag_sprite.setPosition(MagMobs[i].pos);
					window.draw(mag_sprite);
				}
			}
			//2
			for (int i = 0; i < MalleMobs.size(); i++)
			{
				if (MalleMobs[i].HP <= 0)
				{
					MalleMobs.erase(MalleMobs.begin() + i);
					i--;
					if (p.mana < 100)
						p.mana += 15;
				}
				else
				{
					myle_sprite.setPosition(MalleMobs[i].pos);
					window.draw(myle_sprite);
				}
			}
			//3
			for (int i = 0; i < MeleMobs.size(); i++)
			{
				if (MeleMobs[i].HP <= 0)
				{
					MeleMobs.erase(MeleMobs.begin() + i);
					i--;
					if (p.mana < 100)
						p.mana += 15;
				}
				else
				{
					male_sprite.setPosition(MeleMobs[i].pos);
					window.draw(male_sprite);
				}
			}
			//4
			for (int i = 0; i < deathMobs.size(); i++)
			{
				if (deathMobs[i].HP <= 0)
				{
					deathMobs.erase(deathMobs.begin() + i);
					i--;
					if (p.mana < 100)
						p.mana += 15;
				}
				else
				{
					death1_sprite.setPosition(deathMobs[i].pos);
					window.draw(death1_sprite);
				}
			}
			//
			
			std::ostringstream playerHealthString, playerManaString;    // объявили переменную
			playerHealthString << p.health;		//занесли в нее число здоровья, то есть формируем строку
			playerManaString << p.mana;		//занесли в нее число здоровья, то есть формируем строку
			text.setString("Здоровье:" + playerHealthString.str()+ "Мана:" + playerManaString.str());
			//text.setString("Мана:" + playerManaString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
			text.setPosition(view.getCenter().x - 565, view.getCenter().y - 300);//задаем позицию текста, отступая от центра камеры
			window.draw(text);//рисую этот текст

		window.draw(mage_sprite);
		window.draw(mage1_sprite);
		window.draw(p.sprite);
		window.display();
	}
	//
	
	return 0;
}